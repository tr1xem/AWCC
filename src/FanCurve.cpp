#include "FanCurve.h"

#include <GLFW/glfw3.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace {

std::filesystem::path ConfigDir() {
    if (const char *xdg = std::getenv("XDG_CONFIG_HOME");
        xdg != nullptr && xdg[0] != '\0')
        return std::filesystem::path(xdg) / "awcc";
    const char *home = std::getenv("HOME");
    return std::filesystem::path(home != nullptr ? home : ".") / ".config" /
           "awcc";
}

std::filesystem::path AutostartDir() {
    if (const char *xdg = std::getenv("XDG_CONFIG_HOME");
        xdg != nullptr && xdg[0] != '\0')
        return std::filesystem::path(xdg) / "autostart";
    const char *home = std::getenv("HOME");
    return std::filesystem::path(home != nullptr ? home : ".") / ".config" /
           "autostart";
}

std::filesystem::path SocketPath() {
    if (const char *runtime = std::getenv("XDG_RUNTIME_DIR");
        runtime != nullptr && runtime[0] != '\0')
        return std::filesystem::path(runtime) / "awcc-gui.sock";
    return std::filesystem::path("/tmp") /
           ("awcc-gui-" + std::to_string(getuid()) + ".sock");
}

std::string Quote(const std::string &text) {
    if (text.find_first_of(" \t") == std::string::npos)
        return text;
    return "\"" + text + "\"";
}

std::string ReadTrimmed(const std::filesystem::path &path) {
    std::ifstream in(path);
    std::string text;
    if (!std::getline(in, text))
        return {};
    while (!text.empty() && (text.back() == '\n' || text.back() == '\r'))
        text.pop_back();
    return text;
}

int ReadSysInt(const std::filesystem::path &path) {
    std::ifstream in(path);
    int value = 0;
    if (!(in >> value))
        return -1;
    return value;
}

int ExtraFor(const FanCurvePoint *points, int tempC) {
    FanCurvePoint sorted[4];
    std::copy(points, points + 4, sorted);
    std::sort(sorted, sorted + 4,
              [](const FanCurvePoint &a, const FanCurvePoint &b) {
                  return a.tempC < b.tempC;
              });
    if (tempC <= sorted[0].tempC)
        return std::clamp(sorted[0].extra, 0, 100);
    if (tempC >= sorted[3].tempC)
        return std::clamp(sorted[3].extra, 0, 100);
    for (int i = 0; i < 3; ++i) {
        if (tempC > sorted[i + 1].tempC)
            continue;
        const int span = sorted[i + 1].tempC - sorted[i].tempC;
        if (span <= 0)
            return std::clamp(sorted[i + 1].extra, 0, 100);
        const float t =
            static_cast<float>(tempC - sorted[i].tempC) / static_cast<float>(span);
        const float extra =
            static_cast<float>(sorted[i].extra) +
            t * static_cast<float>(sorted[i + 1].extra - sorted[i].extra);
        return std::clamp(static_cast<int>(extra + 0.5F), 0, 100);
    }
    return std::clamp(sorted[3].extra, 0, 100);
}

int gListenFd = -1;

} // namespace

FanSensors ReadFanSensors() {
    FanSensors fans;
    std::error_code error;
    for (const auto &entry :
         std::filesystem::directory_iterator("/sys/class/hwmon", error)) {
        if (ReadTrimmed(entry.path() / "name") != "dell_ddv")
            continue;
        for (int i = 1; i <= 12; ++i) {
            const std::string index = std::to_string(i);
            const std::string tempLabel =
                ReadTrimmed(entry.path() / ("temp" + index + "_label"));
            const std::string fanLabel =
                ReadTrimmed(entry.path() / ("fan" + index + "_label"));
            if (tempLabel == "CPU")
                fans.cpuC =
                    ReadSysInt(entry.path() / ("temp" + index + "_input")) / 1000;
            else if (tempLabel == "Video")
                fans.gpuC =
                    ReadSysInt(entry.path() / ("temp" + index + "_input")) / 1000;
            if (fanLabel == "CPU Fan")
                fans.cpuRpm = ReadSysInt(entry.path() / ("fan" + index + "_input"));
            else if (fanLabel == "Video Fan")
                fans.gpuRpm = ReadSysInt(entry.path() / ("fan" + index + "_input"));
        }
        break;
    }
    return fans;
}

void FanCurve::load() {
    std::ifstream in(ConfigDir() / "fan-curve.json");
    if (!in)
        return;
    nlohmann::json json;
    try {
        in >> json;
    } catch (const nlohmann::json::exception &) {
        return;
    }
    enabled = json.value("enabled", false);
    auto readPoints = [](const nlohmann::json &array, FanCurvePoint *points) {
        if (!array.is_array())
            return;
        const int count = std::min(4, static_cast<int>(array.size()));
        for (int i = 0; i < count; ++i) {
            points[i].tempC = std::clamp(array[i].value("temp", points[i].tempC), 30, 105);
            points[i].extra = std::clamp(array[i].value("extra", points[i].extra), 0, 100);
        }
    };
    if (json.contains("cpu"))
        readPoints(json["cpu"], cpu);
    if (json.contains("gpu"))
        readPoints(json["gpu"], gpu);
}

void FanCurve::save() const {
    nlohmann::json json;
    json["enabled"] = enabled;
    for (const FanCurvePoint &point : cpu)
        json["cpu"].push_back({{"temp", point.tempC}, {"extra", point.extra}});
    for (const FanCurvePoint &point : gpu)
        json["gpu"].push_back({{"temp", point.tempC}, {"extra", point.extra}});
    const std::filesystem::path dir = ConfigDir();
    std::error_code error;
    std::filesystem::create_directories(dir, error);
    std::ofstream out(dir / "fan-curve.json", std::ios::trunc);
    if (out)
        out << json.dump(2) << '\n';
}

void FanCurve::tick(Thermals &thermals, int &cpuBoost, int &gpuBoost) {
    using clock = std::chrono::steady_clock;
    static clock::time_point lastTick{};
    const clock::time_point now = clock::now();
    if (lastTick != clock::time_point{} && now - lastTick < std::chrono::seconds(2))
        return;
    lastTick = now;
    if (!enabled)
        return;

    const FanSensors fans = ReadFanSensors();
    seenCpuC = fans.cpuC;
    seenGpuC = fans.gpuC;
    if (fans.cpuC >= 0) {
        const int target = ExtraFor(cpu, fans.cpuC);
        if (target != cpuBoost) {
            cpuBoost = target;
            thermals.setCpuBoost(target);
        }
    }
    if (fans.gpuC >= 0) {
        const int target = ExtraFor(gpu, fans.gpuC);
        if (target != gpuBoost) {
            gpuBoost = target;
            thermals.setGpuBoost(target);
        }
    }
}

bool NotifyRunningInstance() {
    const std::filesystem::path path = SocketPath();
    const int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
        return false;
    sockaddr_un address{};
    address.sun_family = AF_UNIX;
    const std::string name = path.string();
    if (name.size() >= sizeof(address.sun_path)) {
        close(fd);
        return false;
    }
    std::strncpy(address.sun_path, name.c_str(), sizeof(address.sun_path) - 1);
    const bool connected =
        connect(fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) == 0;
    if (connected) {
        const char byte = 1;
        send(fd, &byte, 1, MSG_NOSIGNAL);
    }
    close(fd);
    return connected;
}

void StartInstanceServer() {
    if (gListenFd >= 0)
        return;
    const std::filesystem::path path = SocketPath();
    std::error_code error;
    std::filesystem::remove(path, error);
    const int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0)
        return;
    sockaddr_un address{};
    address.sun_family = AF_UNIX;
    const std::string name = path.string();
    if (name.size() >= sizeof(address.sun_path)) {
        close(fd);
        return;
    }
    std::strncpy(address.sun_path, name.c_str(), sizeof(address.sun_path) - 1);
    if (bind(fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) != 0 ||
        listen(fd, 4) != 0) {
        close(fd);
        return;
    }
    const int flags = fcntl(fd, F_GETFL, 0);
    if (flags >= 0)
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    gListenFd = fd;
}

void PollInstanceServer(GLFWwindow *window) {
    if (gListenFd < 0 || window == nullptr)
        return;
    const int client = accept(gListenFd, nullptr, nullptr);
    if (client < 0)
        return;
    close(client);
    glfwShowWindow(window);
    glfwFocusWindow(window);
    glfwRequestWindowAttention(window);
}

void StopInstanceServer() {
    if (gListenFd < 0)
        return;
    close(gListenFd);
    gListenFd = -1;
    std::error_code error;
    std::filesystem::remove(SocketPath(), error);
}

bool AutostartEnabled() {
    return std::filesystem::exists(AutostartDir() / "awcc-gui.desktop");
}

void SetAutostartEnabled(bool enabled) {
    const std::filesystem::path path = AutostartDir() / "awcc-gui.desktop";
    if (!enabled) {
        std::error_code error;
        std::filesystem::remove(path, error);
        return;
    }
    char exeBuffer[4096];
    const ssize_t length = readlink("/proc/self/exe", exeBuffer, sizeof(exeBuffer) - 1);
    if (length <= 0)
        return;
    exeBuffer[length] = '\0';

    std::string command;
    if (const char *database = std::getenv("AWCC_DATABASE");
        database != nullptr && database[0] != '\0')
        command += "AWCC_DATABASE=" + Quote(database) + " ";
    if (const char *cli = std::getenv("AWCC_ALIENFX_CLI");
        cli != nullptr && cli[0] != '\0')
        command += "AWCC_ALIENFX_CLI=" + Quote(cli) + " ";
    command += Quote(exeBuffer);
    command += " --gui";

    std::error_code error;
    std::filesystem::create_directories(AutostartDir(), error);
    std::ofstream out(path, std::ios::trunc);
    if (!out)
        return;
    out << "[Desktop Entry]\n"
        << "Type=Application\n"
        << "Name=Alienware Command Center\n"
        << "Comment=Keep the AWCC fan curve running\n"
        << "Exec=env " << command << "\n"
        << "Terminal=false\n"
        << "X-GNOME-Autostart-enabled=true\n";
}

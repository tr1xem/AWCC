#include "Daemon.h"
#include "EffectController.h"
#include "KeyBinder.h"
#include "LightFX.h"
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <loguru.hpp>
#include <pwd.h>
#include <regex>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <utility>

#define BUF_SIZE 1024

namespace {
Daemon *s_instance = nullptr;

constexpr uid_t EXPECTED_UID = 1000;

void daemon_signal_handler(int /*unused*/) {
    if (s_instance != nullptr)
        s_instance->stop();
    exit(0);
}
} // namespace

Daemon::Daemon(EffectController &effectsController, std::string socket_path)
    : m_running(false), m_server_fd(-1), m_socket_path(std::move(socket_path)),
      m_effectsController(effectsController) {
    s_instance = this;
}

// Daemon::~Daemon() { stop(); }

Daemon::~Daemon() { LOG_S(INFO) << "Daemon Module deinitialized"; }
bool Daemon::isDaemonRunning() {
    return (access(m_socket_path.c_str(), F_OK) == 0 &&
            std::filesystem::exists(m_socket_path));
}

void Daemon::m_StopBinder() {
    if (m_binder != nullptr) {
        m_binder->stop();
        if (m_keybinderThread.joinable())
            m_keybinderThread.join();
        delete m_binder;
        m_binder = nullptr;
    }
}

void Daemon::stop() {
    if (!m_running)
        return;
    if (m_server_fd != -1)
        close(m_server_fd);
    unlink(m_socket_path.c_str());
    LOG_S(INFO) << "Cleaned up socket file. Daemon stopped.";
    LOG_S(INFO) << "Stopping KeyBinder Module";
    m_StopBinder();
}

void Daemon::m_onGmodeKey() {
    if (m_onGmodeKeyCallback) {
        m_onGmodeKeyCallback();
    } else
        LOG_S(ERROR) << "GMode Callback Not set";
}

void Daemon::m_onLightKey() {
    switch (m_brightness) {
    case 0:
        m_brightness = 50;
        break;
    case 50:
        m_brightness = 100;
        break;
    case 100:
        m_brightness = 0;
        break;
    default:
        m_brightness = 0;
        break;
    }
    m_effectsController.Brightness(m_brightness);
    std::string deviceName = m_deviceName();
    if (deviceName.contains("Dell G15 Special Edition 5521")) {
        m_effectsController.LightBarBrightness(m_brightness);
    }
}

// TODO: Make it only allow a certain type of commands
void Daemon::init() {
    if (isDaemonRunning()) {
        LOG_S(ERROR)
            << "Socket file exists. Another daemon may be running. Exiting.";
        exit(1);
    }

    // NOTE: Start the key binder module
    auto *binder = new KeyBinder("AT Translated Set 2 keyboard");
    binder->setOnGModeKey([this]() { this->m_onGmodeKey(); });
    binder->setOnLightKey([this]() { this->m_onLightKey(); });

    std::thread keybinderThread([binder]() { binder->run(); });

    signal(SIGINT, daemon_signal_handler);
    signal(SIGTERM, daemon_signal_handler);

    // std::atexit([]() {
    //     if (s_instance)
    //         s_instance->stop();
    // });
    //
    m_server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (m_server_fd < 0) {
        LOG_S(ERROR) << "Failed to create socket: " << strerror(errno);
        exit(1);
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, m_socket_path.c_str(), sizeof(addr.sun_path) - 1);

    if (bind(m_server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        LOG_S(ERROR) << "Failed to bind socket: " << strerror(errno);
        exit(1);
    }

    // Security: set permissions and ownership
    // If running as root, chown to your app user and chmod to 0600
    chown(m_socket_path.c_str(), EXPECTED_UID, EXPECTED_UID);
    chmod(m_socket_path.c_str(), 0600);

    listen(m_server_fd, 5);
    m_running = true;
    LOG_S(INFO) << "Daemon listening on " << m_socket_path;

    while (m_running) {
        if (!keybinderThread.joinable()) {
            LOG_S(ERROR) << "KeyBinder thread has exited!";
            m_StopBinder();
            break;
        }

        int client_fd = accept(m_server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            LOG_S(ERROR) << "Accept failed: " << strerror(errno);
            continue;
        }

        // SO_PEERCRED security check (Linux only)
        struct ucred cred;
        socklen_t len = sizeof(cred);
        if (getsockopt(client_fd, SOL_SOCKET, SO_PEERCRED, &cred, &len) == 0) {
            if (cred.uid != EXPECTED_UID) {
                LOG_S(ERROR)
                    << "Rejected unauthorized client (UID=" << cred.uid << ")";
                close(client_fd);
                continue;
            }
        } else {
            LOG_S(ERROR) << "Failed to get peer credentials: "
                         << strerror(errno);
            close(client_fd);
            continue;
        }

        std::array<char, BUF_SIZE> buf{};
        int n = read(client_fd, buf.data(), buf.size() - 1);
        if (n > 0) {
            buf[n] = '\0';
            // Sanitize input for command check
            std::string cmd(buf.data());
            cmd.erase(cmd.find_last_not_of(" \n\r\t") + 1);
            if (cmd == "stop") {
                LOG_S(INFO) << "Received stop command, shutting down daemon.";
                write(client_fd, "Daemon stopped", 15);
                close(client_fd);
                stop();  // cleanup
                exit(0); // kill process
            } else {
                std::string output = executeFromDaemon(cmd.c_str());
                write(client_fd, output.c_str(), output.size());
            }
        }
        close(client_fd);
    }
}
bool Daemon::m_CommandAllowed(const std::string &cmd) {
    // Allowed command patterns (expand as needed)
    if (cmd == "stop")
        return true;
    static const std::vector<std::regex> allowed_patterns{
        // pkexec ACPI call
        std::regex{
            R"(pkexec sh -c 'echo "\\_SB\..*WMAX 0 0x[0-9a-fA-F]+ \{(?:\s*0x[0-9a-fA-F]+,?)+\s*\}" > /proc/acpi/call && cat /proc/acpi/call')"},
        // Intel turbo
        std::regex{
            R"(echo [01] \| sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo)"},
        // AMD turbo
        std::regex{
            R"(echo [01] \| sudo tee /sys/devices/system/cpu/cpufreq/boost)"}};

    for (const auto &pat : allowed_patterns) {
        if (std::regex_match(cmd, pat))
            return true;
    }
    return false;
}

std::string Daemon::executeFromDaemon(const char *command) {
    if (m_running) {
        if (!m_CommandAllowed(command)) {
            LOG_S(ERROR) << "Rejected command: " << command;
            return "Rejected command";
        }
        LOG_S(INFO) << "Executing command: " << command;
        FILE *fp = popen(command, "r");
        std::string result;
        if (fp == nullptr) {
            result = "Failed to execute command";
            LOG_S(ERROR) << result;
        } else {
            std::array<char, BUF_SIZE> outbuf{};
            while (fgets(outbuf.data(), outbuf.size(), fp) != nullptr) {
                result += outbuf.data();
            }
            pclose(fp);
        }
        return result;
    } else {
        int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sock_fd < 0) {
            LOG_S(ERROR) << "Socket error: " << strerror(errno);
            return "Socket error";
        }

        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, m_socket_path.c_str(),
                sizeof(addr.sun_path) - 1);

        if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            LOG_S(ERROR) << "Connect error: " << strerror(errno);
            close(sock_fd);
            return "Connect error";
        }

        if (write(sock_fd, command, strlen(command)) < 0) {
            LOG_S(ERROR) << "Write error: " << strerror(errno);
            close(sock_fd);
            return "Write error";
        }

        std::array<char, BUF_SIZE> buf{};
        std::string output;
        int n;
        while ((n = read(sock_fd, buf.data(), buf.size() - 1)) > 0) {
            buf[n] = '\0';
            output += buf.data();
        }
        close(sock_fd);
        return output;
    }
}

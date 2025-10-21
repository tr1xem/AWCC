#include "AcpiUtils.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

const char *AcpiUtils::getPrefix() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) {
        LOG_S(ERROR) << "Cannot read /proc/cpuinfo";
        std::exit(1);
    }
    std::string line;
    std::string deviceName = getDeviceName();
    // NOTE: Dell is just doing some weird things with this device only it made
    // amd's perfix like intel's for no reason apparently
    //
    // TODO:
    // Find some other device so that i can modularise it
    if (deviceName.contains("Alienware m18 R1 AMD")) {
        return "AMWW";
    }
    while (std::getline(cpuinfo, line)) {
        if (line.find("vendor_id") != std::string::npos) {
            if (line.find("GenuineIntel") != std::string::npos) {
                LOG_S(INFO) << "Found Intel using Prefix : AMWW";
                return "AMWW";
            } else if (line.find("AuthenticAMD") != std::string::npos) {
                LOG_S(INFO) << "Found AMD using Prefix : AMW3";
                return "AMW3";
            }
        }
    }
    LOG_S(ERROR) << "Cannot get vendor from /proc/cpuinfo";
    std::exit(1);
    return "";
}

const char *AcpiUtils::getDeviceName() {
    static std::string deviceName;
    std::ifstream dmiFile("/sys/class/dmi/id/product_name");
    if (!dmiFile.is_open()) {
        LOG_S(ERROR) << "Cannot read /sys/class/dmi/id/product_name";
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << dmiFile.rdbuf();
    deviceName = buffer.str();

    if (!deviceName.empty() && deviceName.back() == '\n') {
        deviceName.pop_back();
    }

    return deviceName.c_str();
};

int AcpiUtils::m_resolveDevicefromDatabase() {
    m_deviceName = getDeviceName();
    std::string path = "/etc/awcc/database.json";
    std::ifstream file(path);
    if (m_testMode) {
        return 0;
    }

    if (!file) {
        LOG_S(ERROR) << "Failed to find database.json in :" << path;
        return -1;
    }

    json jsonHandel;
    file >> jsonHandel;

    if (!jsonHandel.contains(m_deviceName)) {
        LOG_S(ERROR) << "Device not found: " << m_deviceName;
        deviceInfo(true);
        return -1;
    }

    json &device = jsonHandel[m_deviceName];

    if (!device.empty() && !device.is_null()) {
        bool found = false;

        if (device.contains("featureSet")) {
            m_featureSetBits =
                std::bitset<7>(device["featureSet"].get<std::string>());
            found = true;
        }

        if (device.contains("thermalModes")) {
            m_thermalModeBits =
                std::bitset<8>(device["thermalModes"].get<std::string>());
            found = true;
        }

        if (device.contains("lightingModes")) {
            m_lightingModesBits =
                std::bitset<6>(device["lightingModes"].get<std::string>());
            found = true;
        }

        if (!found) {
            deviceInfo(true);
            return -1;
        } else {
            LOG_S(INFO) << "Device info resolved from database";
            LOG_S(INFO) << "Found Device: " << m_deviceName;
            m_deviceResolved = true;
            return 0;
        }
    } else {
        deviceInfo(true);
        return -1;
    }
}

AcpiUtils::~AcpiUtils() { LOG_S(INFO) << "ACPIUtils Module deinitialized"; }

AcpiUtils::AcpiUtils(Daemon &daemon, bool testMode)
    : m_daemon(daemon), m_testMode(testMode) {
    LOG_S(INFO) << "Initializing ACPIUtils Module";
    m_acpiPrefix = getPrefix();
    int resolveStatus = m_resolveDevicefromDatabase();

    if (!m_deviceResolved || resolveStatus == -1) {
        if (!m_testMode) {
            LOG_S(ERROR) << "Device resolution failed";
            std::exit(1);
        }
    }
    if (!m_testMode) {
        LOG_S(INFO)
            << "AcpiUtils Module initialization completed got device info and "
               "feature supported";
        // LOG_S(INFO) << "FeatureSet: " << m_featureSetBits;
        // LOG_S(INFO) << "ThermalModes: " << m_thermalModeBits;
        // LOG_S(INFO) << "LightingModes: " << m_lightingModesBits;
        // if (m_daemon.isDaemonRunning())
        //     LOG_S(WARNING) << "Daemon is running, commands will be sent to
        //     daemon";
        // else
        //     LOG_S(WARNING) << "Daemon is not running, running in traditional
        //     mode
        //     ";
    } else {
        LOG_S(WARNING) << "Running in test mode - continuing despite device "
                          "resolution failure";
    }
}

// INFO: Any check for ACPI support should be done before calling this function
// it is just a interface to execute a command
// '\_SB.AMWW.WMAX 0 {} {{{}, {}, 0x0, 0x00}}'
int AcpiUtils::executeAcpiCommand(int arg1, int arg2, int arg3, int arg4) {
    std::array<char, 128> buffer{};
    [[maybe_unused]] std::string result;
    std::string command =
        std::format("pkexec sh -c 'echo \"\\_SB.{}.WMAX 0 {:#x} "
                    "{{{:#x},{:#x},{:#x},0x00}}\" "
                    "> /proc/acpi/call && cat /proc/acpi/call'",
                    m_acpiPrefix, arg1, arg2, arg3, arg4);

#ifndef NDEBUG
    LOG_S(INFO) << "Executing command: " << command;
    LOG_S(WARNING) << "In debug mode, not executing command";
    return 0;
#else
    if (std::filesystem::exists("/proc/acpi/call")) {
        if (m_daemon.isDaemonRunning()) {
            result = m_daemon.executeFromDaemon(command.c_str());
            if (!result.contains("error")) {
                if (!result.empty() && result.back() == '\n')
                    result.pop_back();
                try {
                    if (result.starts_with("0x")) { // starts with 0x
                        return std::stoul(result, nullptr, 16);
                    } else {
                        return std::stoul(result); // decimal
                    }
                } catch (const std::exception &e) {
                    LOG_S(ERROR) << "Failed to parse ACPI output: " << e.what();
                    return -1;
                }

            } else {
                LOG_S(ERROR) << "Daemon returned error: " << result;
                return -1;
            }
        } else {
            LOG_S(INFO) << "Executing command: " << command;
            FILE *pipe = popen(command.c_str(), "r");
            if (pipe == nullptr) {
                LOG_S(ERROR) << "Failed to execute command";
                return -1;
            } else {
                while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
                    result += buffer.data();
                }
                int status = pclose(pipe);
                if (status == 0) {
                    if (!result.empty() && result.back() == '\n')
                        result.pop_back();
                    try {
                        if (result.starts_with("0x")) { // starts with 0x
                            return std::stoul(result, nullptr, 16);
                        } else {
                            return std::stoul(result); // decimal
                        }
                    } catch (const std::exception &e) {
                        LOG_S(ERROR)
                            << "Failed to parse ACPI output: " << e.what();
                        return -1;
                    }
                } else {
                    LOG_S(ERROR) << "Command failed with status: " << status;
                    return -1;
                }
            }
        }
    } else {
        LOG_S(ERROR) << "ACPI module not found in kernel";
        return -1;
    }
#endif
}

void AcpiUtils::deviceInfo(bool unknownDevice) {
    if (unknownDevice) {

        LOG_S(ERROR) << "Device Found is currently not supported, please run "
                        "\"awcc test-modes --test-mode\" and send the output "
                        "to the developer";
    }

    if (!unknownDevice) {
        std::cout << "Device Name: " << m_deviceName << "\n";
        std::cout << "Device Prefix: " << m_acpiPrefix << "\n";
        std::cout << "Version: " << VERSION << "\n";
        std::cout << "Features enabled:\n";
        if (hasFeature(FeatureSet::FanBoost))
            std::cout << "  Fan Boost\n";
        if (hasFeature(FeatureSet::ThermalModes))
            std::cout << "  Thermal Modes\n";
        if (hasFeature(FeatureSet::AutoBoost))
            std::cout << "  Auto Boost\n";
        if (hasFeature(FeatureSet::CpuTemp))
            std::cout << "  CPU Temp\n";
        if (hasFeature(FeatureSet::GpuTemp))
            std::cout << "  GPU Temp\n";
        if (hasFeature(FeatureSet::BrightnessControl))
            std::cout << "  Brightness Control\n";
        if (hasFeature(FeatureSet::GModeToggle))
            std::cout << "  GMode Toggle\n";

        std::cout << "Thermal modes enabled:\n";
        if (hasThermalMode(ThermalModeSet::Quiet))
            std::cout << "  Quiet\n";
        if (hasThermalMode(ThermalModeSet::Balanced))
            std::cout << "  Balanced\n";
        if (hasThermalMode(ThermalModeSet::Performance))
            std::cout << "  Performance\n";
        if (hasThermalMode(ThermalModeSet::BatterySaver))
            std::cout << "  Battery Saver\n";
        if (hasThermalMode(ThermalModeSet::Cool))
            std::cout << "  Cool\n";
        if (hasThermalMode(ThermalModeSet::FullSpeed))
            std::cout << "  Full Speed\n";
        if (hasThermalMode(ThermalModeSet::GMode))
            std::cout << "  GMode\n";
        if (hasThermalMode(ThermalModeSet::Manual))
            std::cout << "  Manual\n";

        std::cout << "Lighting modes enabled:\n";
        if (hasLightingMode(LightingSet::StaticColor))
            std::cout << "  Static Color\n";
        if (hasLightingMode(LightingSet::SpectrumEffect))
            std::cout << "  Spectrum Effect\n";
        if (hasLightingMode(LightingSet::BreathingEffect))
            std::cout << "  Breathing Effect\n";
        if (hasLightingMode(LightingSet::RainbowEffect))
            std::cout << "  Rainbow Effect\n";
        if (hasLightingMode(LightingSet::WaveEffect))
            std::cout << "  Wave Effect\n";
        if (hasLightingMode(LightingSet::BackForthEffect))
            std::cout << "  Back Forth Effect\n";
    }
}
bool AcpiUtils::hasLightingMode(LightingSet l) const {
    return (m_lightingModesBits.to_ulong() & static_cast<unsigned long>(l)) !=
           0;
}

bool AcpiUtils::hasThermalMode(ThermalModeSet m) const {
    return (m_thermalModeBits.to_ulong() & static_cast<unsigned long>(m)) != 0;
}

bool AcpiUtils::hasFeature(FeatureSet f) const {
    return (m_featureSetBits.to_ulong() & static_cast<unsigned long>(f)) != 0;
}

bool AcpiUtils::isIntel() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("GenuineIntel") != std::string::npos)
            return true;
    }
    return false;
}

bool AcpiUtils::isAMD() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("AuthenticAMD") != std::string::npos)
            return true;
    }
    return false;
}

bool AcpiUtils::getTurboBoost() const {
    if (isIntel()) {
        std::ifstream file("/sys/devices/system/cpu/intel_pstate/no_turbo");
        int val;
        if (file >> val) {
            // 0 = on, 1 = off
            return val == 0;
        }
    } else if (isAMD()) {
        std::ifstream file("/sys/devices/system/cpu/cpufreq/boost");
        int val;
        if (file >> val) {
            // 1 = on, 0 = off
            return val == 1;
        }
    }
    // Unknown/unsupported: treat as off
    return false;
}

bool AcpiUtils::setTurboBoost(bool enable) {
    std::string cmd;

    if (isIntel()) {
        int writeVal = enable ? 0 : 1;
        cmd = "echo " + std::to_string(writeVal) +
              " | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo";
        LOG_S(INFO) << "Setting Turbo Boost (Intel) to: " << writeVal;
    } else if (isAMD()) {
        int writeVal = enable ? 1 : 0;
        cmd = "echo " + std::to_string(writeVal) +
              " | sudo tee /sys/devices/system/cpu/cpufreq/boost";
        LOG_S(INFO) << "Setting Turbo Boost (AMD) to: " << writeVal;
    } else {
        return false;
    }

    // Assume m_daemon.executeFromDaemon(cmd) returns true on success
    m_daemon.executeFromDaemon(cmd.c_str());
    return true;
}

const char *AcpiUtils::getThermalModeString(int mode) {
    switch (mode) {
    case 0xa0:
        return "Balanced";
    case 0xa1:
        return "Performance";
    case 0xa2:
        return "Cool";
    case 0xa3:
        return "Quiet";
    case 0xa4:
        return "FullSpeed";
    case 0xa5:
        return "BatterySaver";
    case 0xab:
        return "Gmode";
    case 0x0:
        return "Manual";
    default:
        return "Unknown";
    }
}

std::string AcpiUtils::generateThermalModesBitmap() {
    std::bitset<8> bitmap{0};
    if (executeAcpiCommand(0x15, 0x01, static_cast<int>(ThermalModes::Quiet),
                           0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Quiet);
    }
    if (executeAcpiCommand(0x15, 0x01, static_cast<int>(ThermalModes::Balanced),
                           0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Balanced);
    }
    if (executeAcpiCommand(0x15, 0x01,
                           static_cast<int>(ThermalModes::Performance),
                           0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Performance);
    }
    if (executeAcpiCommand(0x15, 0x01,
                           static_cast<int>(ThermalModes::BatterySaver),
                           0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::BatterySaver);
    }
    if (executeAcpiCommand(0x15, 0x01, static_cast<int>(ThermalModes::Cool),
                           0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Cool);
    }
    if (executeAcpiCommand(
            0x15, 0x01, static_cast<int>(ThermalModes::FullSpeed), 0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::FullSpeed);
    }
    if (executeAcpiCommand(0x15, 0x01, static_cast<int>(ThermalModes::Gmode),
                           0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::GMode);
    }
    if (executeAcpiCommand(0x15, 0x01, static_cast<int>(ThermalModes::Manual),
                           0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Manual);
    }
    return bitmap.to_string();
}

void AcpiUtils::testThermalModes() {
    std::cout << "Device Name: " << m_deviceName << "\n";
    std::cout << "Testing thermal modes...\n\n";

    std::vector<int> testModes;

    for (int i = 0xa0; i <= 0xa9; ++i) {
        testModes.push_back(i);
    }

    for (int i = 0xaa; i <= 0xab; ++i) {
        testModes.push_back(i);
    }

    for (int mode : testModes) {
        // std::cout << std::format("Trying mode {:#x}\n", mode);
        // std::cout << std::format(
        //     "\\_SB.{}.WMAX 0 0x15 {{0x01, {:#x}, 0x00, 0x00}}\n",
        //     m_acpiPrefix, mode);

        int result = executeAcpiCommand(0x15, 0x01, mode, 0x00);
        std::cout << std::format("{:#x} -> {:#x} ({})\n", mode, result,
                                 getThermalModeString(mode));
    }

    std::string bitmap = generateThermalModesBitmap();
    std::cout << "\nGenerated thermal modes bitmap for database.json:\n";
    std::cout << std::format("\"thermalModes\": \"{}\"\n", bitmap);

    std::cout << "\nReverting to balanced.\n";
    executeAcpiCommand(0x15, 0x01, 0xa0, 0x00);
}

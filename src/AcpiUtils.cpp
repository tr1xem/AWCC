#include "AcpiUtils.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

auto AcpiUtils::getPrefix() -> const char * {
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open())
        LOG_S(FATAL) << "Cannot read /proc/cpuinfo";

    std::string line;
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
    LOG_S(FATAL) << "Cannot get vendor from /proc/cpuinfo";
    return "";
}

auto AcpiUtils::m_getDeviceName() -> const char * {

    static std::string deviceName;
    std::ifstream dmiFile("/sys/class/dmi/id/product_name");
    if (!dmiFile.is_open()) {
        LOG_S(FATAL) << "Cannot read /sys/class/dmi/id/product_name";
    }

    std::stringstream buffer;
    buffer << dmiFile.rdbuf();
    deviceName = buffer.str();

    if (!deviceName.empty() && deviceName.back() == '\n') {
        deviceName.pop_back();
    }

    return deviceName.c_str();
};

auto AcpiUtils::m_getDeviceAcpiCode() -> int {
    return executeAcpiCommand(0x1a, 0x02, 0x02, 0x00);
};

auto AcpiUtils::m_resolveDevicefromDatabase() -> int {

    m_deviceName = m_getDeviceName();
    const char *home = std::getenv("HOME");
    std::string path = std::string(home) + "/work/awcc-rewrite/database.json";
    std::ifstream file(path);

    if (!file) {
        LOG_S(ERROR) << "Failed to open database.json";
        return -1;
    }
    json jsonHandel;
    file >> jsonHandel;

    if (!jsonHandel.contains(m_deviceName)) {
        LOG_S(ERROR) << "Device not found: " << m_deviceName;
        return -1;
    }

    json &device = jsonHandel[m_deviceName];

    if (!device.empty()) {
        bool found = false;

        for (const auto &[key, entry] : device.items()) {
            if (key == "0x0000") {
                LOG_S(INFO) << m_deviceName << " uses DMI.";
                m_acpiModelId = 0x0000;
                // Use the "0x0000" entry for bitsets if it exists
                if (!entry.is_null()) {
                    if (entry.contains("featureSet"))
                        m_featureSetBits = std::bitset<7>(
                            entry["featureSet"].get<std::string>());

                    if (entry.contains("thermalModes"))
                        m_thermalModeBits = std::bitset<8>(
                            entry["thermalModes"].get<std::string>());

                    if (entry.contains("lightingModes"))
                        m_lightingModesBits = std::bitset<6>(
                            entry["lightingModes"].get<std::string>());
                }
                found = true;
                break;
            } else {
                int deviceHex = std::stoi(key, nullptr, 16);
                int deviceAcpi =
                    m_getDeviceAcpiCode(); // ACPI code of the running system

                if (deviceHex == deviceAcpi) {
                    LOG_S(INFO) << m_deviceName << " uses ACPI key: " << key;
                    m_acpiModelId = deviceHex;

                    // Set bitsets from the matched entry
                    if (!entry.is_null()) {
                        if (entry.contains("featureSet"))
                            m_featureSetBits = std::bitset<7>(
                                entry["featureSet"].get<std::string>());

                        if (entry.contains("thermalModes"))
                            m_thermalModeBits = std::bitset<8>(
                                entry["thermalModes"].get<std::string>());

                        if (entry.contains("lightingModes"))
                            m_lightingModesBits = std::bitset<6>(
                                entry["lightingModes"].get<std::string>());
                    }

                    found = true;
                    break;
                }
            }
        }

        if (!found) {
            LOG_S(ERROR) << m_deviceName << " currently not supported";
            return -1;
        } else {
            LOG_S(INFO) << "Device info resolved from database";
            m_deviceResolved = true;
            return 0;
        }
    } else {
        LOG_S(ERROR) << m_deviceName << " currently not supported";
        return -1;
    }
    return 0;
};

AcpiUtils::AcpiUtils() {
    LOG_S(INFO) << "Initializing ACPIUtils Module";
    m_acpiPrefix = getPrefix();
    int resolveStatus = m_resolveDevicefromDatabase();

    if (!m_deviceResolved || resolveStatus == -1)
        LOG_S(FATAL) << "Device resolution failed";

    LOG_S(INFO)
        << "AcpiUtils Module initialization completed got device info and "
           "feature supported";
    LOG_S(INFO) << "FeatureSet: " << m_featureSetBits;
    LOG_S(INFO) << "ThermalModes: " << m_thermalModeBits;
    LOG_S(INFO) << "LightingModes: " << m_lightingModesBits;
}

// INFO: Any check for ACPI support should be done before calling this function
// it is just a interface to execute a command
auto AcpiUtils::executeAcpiCommand(int arg1, int arg2, int arg3, int arg4)
    -> int {
    std::array<char, 128> buffer{};
    std::string result;
    std::string command =
        std::format("pkexec sh -c 'echo \"\\_SB.{}.WMAX 0 {:#x} "
                    "{{{:#x},{:#x},{:#x},0x00}}\" "
                    "> /proc/acpi/call && cat /proc/acpi/call'",
                    m_acpiPrefix, arg1, arg2, arg3, arg4);

    LOG_S(INFO) << "Executing command: " << command;

#ifndef NDEBUG
    LOG_S(WARNING) << "In debug mode, not executing command";
    return 0;
#else
    if (std::filesystem::exists("/proc/acpi/call")) {
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
                        LOG_S(INFO) << "Command executed successfully";
                        return std::stoi(result, nullptr, 16);
                    } else {
                        LOG_S(INFO) << "Command executed successfully";
                        return std::stoi(result); // decimal
                    }
                } catch (const std::exception &e) {
                    LOG_S(ERROR) << "Failed to parse ACPI output: " << e.what();
                    return -1;
                }
            } else {
                LOG_S(ERROR) << "Command failed with status: " << status;
                return -1;
            }
        }
    } else {
        LOG_S(ERROR) << "ACPI module not found in kernel";
        return -1;
    }
#endif
}

void AcpiUtils::deviceInfo() const {
    if (!m_deviceResolved) {
        return;
    }
    std::cout << "Device Name: " << m_deviceName << "\n";
    std::cout << "ACPI Model ID: 0x" << std::hex << m_acpiModelId << std::dec
              << "\n";

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
auto AcpiUtils::hasLightingMode(LightingSet l) const -> bool {
    return (m_lightingModesBits.to_ulong() & static_cast<unsigned long>(l)) !=
           0;
}

auto AcpiUtils::hasThermalMode(ThermalModeSet m) const -> bool {
    return (m_thermalModeBits.to_ulong() & static_cast<unsigned long>(m)) != 0;
}

auto AcpiUtils::hasFeature(FeatureSet f) const -> bool {
    return (m_featureSetBits.to_ulong() & static_cast<unsigned long>(f)) != 0;
}

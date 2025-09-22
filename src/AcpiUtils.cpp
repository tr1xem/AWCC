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
        throw std::runtime_error("Cannot read /proc/cpuinfo");

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
    throw std::runtime_error("Cannot get vendor from /proc/cpuinfo");
}

AcpiUtils::AcpiUtils() {
    // TODO: Construct a map of supported modes
    // get_devices -> map it with config -> extract that part out of the config
    // to the map -> make it publically accesseble
    std::ifstream file("/home/saumya/work/awcc-rewrite/database.json");
    if (!file) {
        LOG_S(ERROR) << "Failed to open database.json";
        return;
    }
    json jsonHandel;
    file >> jsonHandel;
    json device = jsonHandel["Dell G15 5530"];

    if (device.contains("acpi_model_id")) {
        std::string hexStr = device["acpi_model_id"].get<std::string>();
        acpiModelId = std::stoi(hexStr, nullptr, 16);
    }

    if (device.contains("featureSet"))
        m_featureSetBits =
            std::bitset<7>(device["featureSet"].get<std::string>());

    if (device.contains("thermalModes"))
        m_thermalModeBits =
            std::bitset<8>(device["thermalModes"].get<std::string>());

    if (device.contains("lightingModes"))
        m_lightingModesBits =
            std::bitset<6>(device["lightingModes"].get<std::string>());

    LOG_S(INFO)
        << "AcpiUtils Module initialized got device info and feature supported";
    LOG_S(INFO) << "FeatureSet: " << m_featureSetBits;
    LOG_S(INFO) << "ThermalModes: " << m_thermalModeBits;
    LOG_S(INFO) << "LightingModes: " << m_lightingModesBits;
}

// INFO: Any check for ACPI support should be done before calling this function
// it is just a interface to execute a command
void AcpiUtils::executeAcpiCommand(int arg1, int arg2, int arg3, int arg4) {
    std::string command =
        std::format("echo '\\_SB.{}.WMAX 0 {:#x} {{{:#x}, {:#x},{:#x}, "
                    "0x00}}' | pkexec tee /proc/acpi/call > /dev/null 2>&1",
                    AcpiUtils::getPrefix(), arg1, arg2, arg3, arg4);
    LOG_S(INFO) << "Executing command: " << command;

#ifndef NDEBUG
    LOG_S(WARNING) << "In debug mode, not executing command";
#else
    if (std::filesystem::exists("/proc/acpi/call")) {
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe)
            LOG_S(ERROR) << "Failed to execute command";
        return;
        else {
            int status = pclose(pipe);
            if (status == 0) {
                LOG_S(INFO) << "Command executed successfully";
            } else {
                LOG_S(ERROR) << "Command failed with status: " << status;
            }
        }
    } else {
        LOG_S(ERROR) << "ACPI module not found in kernel";
        return;
    }
#endif
}

void AcpiUtils::deviceInfo() const {
    std::cout << "ACPI Model ID: 0x" << std::hex << acpiModelId << std::dec
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

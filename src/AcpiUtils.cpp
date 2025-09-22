#include "AcpiUtils.h"
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

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
void AcpiUtils::executeAcpiCommand(const char *prefix, int arg1, int arg2,
                                   int arg3, int arg4) {
    std::string command =
        std::format("echo '\\_SB.{}.WMAX 0 {:#x} {{{:#x}, {:#x},{:#x}, "
                    "0x00}}' | pkexec tee /proc/acpi/call > /dev/null 2>&1",
                    prefix, arg1, arg2, arg3, arg4);
    LOG_S(INFO) << "Executing command: " << command;

#ifndef NDEBUG
    LOG_S(WARNING) << "In debug mode, not executing command";
#else
    if (std::filesystem::exists("/proc/acpi/call")) {
        FILE *pipe = popen(command.c_str(), "r");
        if (!pipe)
            LOG_S(ERROR) << "Failed to execute command";
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
    }
#endif
}

void AcpiUtils::deviceInfo() const {
    std::cout << "ACPI Model ID: 0x" << std::hex << acpiModelId << std::dec
              << "\n";

    std::cout << "Features enabled:\n";
    if (hasFeature(b_FanBoost))
        std::cout << "  Fan Boost\n";
    if (hasFeature(b_ThermalModes))
        std::cout << "  Thermal Modes\n";
    if (hasFeature(b_AutoBoost))
        std::cout << "  Auto Boost\n";
    if (hasFeature(b_CpuTemp))
        std::cout << "  CPU Temp\n";
    if (hasFeature(b_GpuTemp))
        std::cout << "  GPU Temp\n";
    if (hasFeature(b_BrightnessControl))
        std::cout << "  Brightness Control\n";
    if (hasFeature(b_GModeToggle))
        std::cout << "  GMode Toggle\n";

    std::cout << "Thermal modes enabled:\n";
    if (hasThermalMode(b_Quiet))
        std::cout << "  Quiet\n";
    if (hasThermalMode(b_Balanced))
        std::cout << "  Balanced\n";
    if (hasThermalMode(b_Performance))
        std::cout << "  Performance\n";
    if (hasThermalMode(b_BatterySaver))
        std::cout << "  Battery Saver\n";
    if (hasThermalMode(b_Cool))
        std::cout << "  Cool\n";
    if (hasThermalMode(b_FullSpeed))
        std::cout << "  Full Speed\n";
    if (hasThermalMode(b_GMode))
        std::cout << "  GMode\n";
    if (hasThermalMode(b_Manual))
        std::cout << "  Manual\n";

    std::cout << "Lighting modes enabled:\n";
    if (hasLightingMode(b_StaticColor))
        std::cout << "  Static Color\n";
    if (hasLightingMode(b_SpectrumEffect))
        std::cout << "  Spectrum Effect\n";
    if (hasLightingMode(b_BreathingEffect))
        std::cout << "  Breathing Effect\n";
    if (hasLightingMode(b_RainbowEffect))
        std::cout << "  Rainbow Effect\n";
    if (hasLightingMode(b_WaveEffect))
        std::cout << "  Wave Effect\n";
    if (hasLightingMode(b_BackForthEffect))
        std::cout << "  Back Forth Effect\n";
}

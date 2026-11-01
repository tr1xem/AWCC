#include "Thermals.h"
#include "database.h"
#include "helper.h"
#include <bitset>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <format>
#include <iostream>
#include <loguru.hpp>
#include <vector>

using std::strcmp;

// AWCC thermal ids follow the same mapping as alienware-wmi's
// awcc_profile_to_pprof(); G-Mode is toggled by the driver on "performance"
static const char *modeToPlatformProfile(ThermalModes mode) {
    switch (mode) {
    case ThermalModes::Quiet:
        return "quiet";
    case ThermalModes::Balanced:
        return "balanced";
    case ThermalModes::Performance:
        return "balanced-performance";
    case ThermalModes::Cool:
        return "cool";
    case ThermalModes::BatterySaver:
        return "low-power";
    case ThermalModes::FullSpeed:
    case ThermalModes::Gmode:
        return "performance";
    default:
        return "custom";
    }
}

static int percentToRawBoost(int percent) {
    return static_cast<int>(std::lround(percent * 255.0 / 100.0));
}

static int rawToPercentBoost(int raw) {
    if (raw < 0)
        return raw;
    return static_cast<int>(std::lround(raw * 100.0 / 255.0));
}

Thermals::~Thermals() { LOG_S(INFO) << "Thermals Module deinitialized"; }
Thermals::Thermals(AcpiUtils &acpiUtils) : m_acpiUtils(acpiUtils) {
    m_useKernel = m_acpiUtils.hasKernelThermal() &&
                  !std::filesystem::exists("/proc/acpi/call");
    if (m_useKernel)
        LOG_S(INFO) << "acpi_call not loaded, using alienware-wmi backend";
    initializeThermalModes();
    patchModes(Helper::getDeviceName());
    m_currentMode = queryThermalMode();
    LOG_S(INFO) << "Thermals Module initialized";
}

void Thermals::setThermalMode(ThermalModes mode) {
    if (!supportsThemeralMode(mode)) {
        LOG_S(ERROR) << m_thermalModeToName(mode)
                     << " mode not supported by device, aborting";
        return;
    } else if (m_useKernel) {
        if (mode == queryThermalMode()) {
            LOG_S(WARNING) << "Current thermal mode is already set to: "
                           << m_thermalModeToName(mode);
            return;
        }
        LOG_S(INFO) << "Setting thermal mode to: " << m_thermalModeToName(mode);
        if (m_acpiUtils.setPlatformProfile(modeToPlatformProfile(mode)))
            m_currentMode = mode;
    } else {

        // NOTE: GMODE handle
        if (mode == ThermalModes::Gmode &&
            m_currentMode != ThermalModes::Gmode) {
            m_acpiUtils.executeAcpiCommand(0x25, 0x01);
        }

        if (m_currentMode == ThermalModes::Gmode) {
            m_acpiUtils.executeAcpiCommand(0x25, 0x00);
        }

        if (mode == m_currentMode) {
            LOG_S(WARNING) << "Current thermal mode is already set to: "
                           << m_thermalModeToName(mode);
            return;
        } else {
            LOG_S(INFO) << "Setting thermal mode to: "
                        << m_thermalModeToName(mode);
            m_acpiUtils.executeAcpiCommand(
                0x15, 0x01, static_cast<int>(m_modeToSetCode[mode]));
            m_currentMode = mode;
        }
    }
}

bool Thermals::supportsThemeralMode(ThermalModes mode) {
    ThermalModeSet bit = m_mapToBitset(mode);
    if (m_useKernel &&
        !m_acpiUtils.hasPlatformProfile(modeToPlatformProfile(mode)))
        return false;
    return m_acpiUtils.hasThermalMode(bit);
}

const char *Thermals::m_thermalModeToName(ThermalModes mode) {
    auto it = m_modeToName.find(mode);
    if (it != m_modeToName.end()) {
        return it->second.c_str();
    }
    return "Unknown";
}

ThermalModeSet Thermals::m_mapToBitset(ThermalModes mode) {
    switch (mode) {
    case ThermalModes::Quiet:
        return ThermalModeSet::Quiet;
    case ThermalModes::Balanced:
        return ThermalModeSet::Balanced;
    case ThermalModes::Performance:
        return ThermalModeSet::Performance;
    case ThermalModes::BatterySaver:
        return ThermalModeSet::BatterySaver;
    case ThermalModes::Cool:
        return ThermalModeSet::Cool;
    case ThermalModes::FullSpeed:
        return ThermalModeSet::FullSpeed;
    case ThermalModes::Gmode:
        return ThermalModeSet::GMode;
    case ThermalModes::Manual:
        return ThermalModeSet::Manual;
    default:
        return ThermalModeSet::Manual; // fallback
    }
}

ThermalModes Thermals::queryThermalMode() {
    if (m_useKernel) {
        std::string profile = m_acpiUtils.getPlatformProfile();
        static const std::unordered_map<std::string, ThermalModes>
            profileToMode = {
                {"quiet", ThermalModes::Quiet},
                {"balanced", ThermalModes::Balanced},
                {"balanced-performance", ThermalModes::Performance},
                {"cool", ThermalModes::Cool},
                {"low-power", ThermalModes::BatterySaver},
                {"custom", ThermalModes::Manual}};
        if (profile == "performance") {
            m_currentMode = m_acpiUtils.hasThermalMode(ThermalModeSet::GMode)
                                ? ThermalModes::Gmode
                                : ThermalModes::FullSpeed;
        } else if (auto it = profileToMode.find(profile);
                   it != profileToMode.end()) {
            m_currentMode = it->second;
        } else {
            LOG_S(ERROR) << "Unknown platform profile: " << profile;
            m_currentMode = ThermalModes::Manual;
        }
        return m_currentMode;
    }

    int result = m_acpiUtils.executeAcpiCommand(0x14, 0x0b, 0x00);

    // Use our map to look up the mode by code
    auto it = m_getCodeToMode.find(static_cast<std::uint8_t>(result));
    if (it != m_getCodeToMode.end()) {
        m_currentMode = it->second;
        return it->second;
    }

    // Fallback for unknown codes
    LOG_S(ERROR) << "Unknown thermal mode returned: 0x" << std::hex << result;
    m_currentMode = ThermalModes::Manual;
    return ThermalModes::Manual;
}

ThermalModes Thermals::getCurrentMode() { return queryThermalMode(); }
const char *Thermals::getCurrentModeName() {
    m_currentMode = queryThermalMode();
    return m_thermalModeToName(m_currentMode);
}

void Thermals::toggleGmode() {
    if (queryThermalMode() == ThermalModes::Gmode) {
        setThermalMode(m_defaultMode);
    } else {
        setThermalMode(ThermalModes::Gmode);
    }
}

int Thermals::getCpuBoost() {
    if (m_useKernel)
        return rawToPercentBoost(m_acpiUtils.getFanBoost("CPU"));
    return m_acpiUtils.executeAcpiCommand(0x14, 0x0c, 0x32);
}
int Thermals::getGpuBoost() {
    if (m_useKernel)
        return rawToPercentBoost(m_acpiUtils.getFanBoost("GPU"));
    return m_acpiUtils.executeAcpiCommand(0x14, 0x0c, 0x33);
}
void Thermals::setCpuBoost(int boost) {
    if (m_useKernel)
        m_acpiUtils.setFanBoost("CPU", percentToRawBoost(boost));
    else
        m_acpiUtils.executeAcpiCommand(0x15, 0x02, 0x32, boost);
    LOG_S(INFO) << "Set CPU Boost to: " << boost << "%";
}
void Thermals::setGpuBoost(int boost) {
    if (m_useKernel) {
        m_acpiUtils.setFanBoost("GPU", percentToRawBoost(boost));
    } else if (strcmp(Helper::getDeviceName(), "Alienware Aurora R11") == 0) {
        // FIXME: Hacky fix for Aurora R11 as it have fans at 0x36 and 0x3a
        m_acpiUtils.executeAcpiCommand(0x15, 0x02, 0x36, boost);
        m_acpiUtils.executeAcpiCommand(0x15, 0x02, 0x3a, boost);
    } else {
        m_acpiUtils.executeAcpiCommand(0x15, 0x02, 0x33, boost);
    }
    LOG_S(INFO) << "Set GPU Boost to: " << boost << "%";
}

void Thermals::initializeThermalModes() {
    m_modeToSetCode = {
        {ThermalModes::Manual, 0x0},        {ThermalModes::Balanced, 0xa0},
        {ThermalModes::Performance, 0xa1},  {ThermalModes::Cool, 0xa2},
        {ThermalModes::Quiet, 0xa3},        {ThermalModes::FullSpeed, 0xa4},
        {ThermalModes::BatterySaver, 0xa5}, {ThermalModes::Gmode, 0xab}};

    m_getCodeToMode = {
        {0x0, ThermalModes::Manual},        {0xa0, ThermalModes::Balanced},
        {0xa1, ThermalModes::Performance},  {0xa2, ThermalModes::Cool},
        {0xa3, ThermalModes::Quiet},        {0xa4, ThermalModes::FullSpeed},
        {0xa5, ThermalModes::BatterySaver}, {0xab, ThermalModes::Gmode}};

    m_modeToName = {{ThermalModes::Manual, "Manual"},
                    {ThermalModes::Balanced, "Balanced"},
                    {ThermalModes::Performance, "Performance"},
                    {ThermalModes::Cool, "Cool"},
                    {ThermalModes::Quiet, "Quiet"},
                    {ThermalModes::FullSpeed, "Full Speed"},
                    {ThermalModes::BatterySaver, "Battery Saver"},
                    {ThermalModes::Gmode, "G-Mode"}};
}

void Thermals::patchModes(const std::string &deviceName) {
    // NOTE: Device-specific overrides with separate set/get codes

    // Example 1: Symmetric mapping (same code for set/get)
    // if (deviceName.contains("Device1")) {
    //     m_modeToSetCode[ThermalModes::Performance] = 0x11;
    //     m_getCodeToMode[0x11] = ThermalModes::Performance;
    //     m_modeToName[ThermalModes::Performance] = "Turbo Mode";
    // }

    // Example 2: Asymmetric mapping (different codes for set/get)
    // if (deviceName.contains("Device2")) {
    //     m_modeToSetCode[ThermalModes::Performance] = 103;  // Send 103 to set
    //     performance mode m_getCodeToMode[98] = ThermalModes::Performance; //
    //     Hardware returns 98 when in performance mode
    //     m_modeToName[ThermalModes::Performance] = "High Performance";
    // }
    if (deviceName.contains("Alienware Aurora R9")) {
        m_modeToSetCode[ThermalModes::Balanced] = 0x197;
        m_getCodeToMode[0x97] = ThermalModes::Balanced;

        m_modeToSetCode[ThermalModes::Performance] = 0x198;
        m_getCodeToMode[0x98] = ThermalModes::Performance;
    }
    if (deviceName.contains("Alienware 17 R5")) {
        m_modeToSetCode[ThermalModes::Quiet] = 0x196;
        m_getCodeToMode[0x96] = ThermalModes::Quiet;

        m_modeToSetCode[ThermalModes::Balanced] = 0x197;
        m_getCodeToMode[0x97] = ThermalModes::Balanced;

        m_modeToSetCode[ThermalModes::Performance] = 0x198;
        m_getCodeToMode[0x98] = ThermalModes::Performance;

        // m_modeToSetCode[ThermalModes::Gmode] = 0x199;
        // m_getCodeToMode[0x99] = ThermalModes::Gmode;
    }
    if (deviceName.contains("Dell G15 5515")) {
        m_modeToSetCode[ThermalModes::Balanced] = 0x97;
        m_getCodeToMode[0x97] = ThermalModes::Balanced;
    }
}

std::uint16_t Thermals::getThermalModeCode(ThermalModes mode) {
    // Static method to provide default thermal mode codes for testing
    // This returns the standard codes without device-specific overrides
    switch (mode) {
    case ThermalModes::Manual:
        return 0x0;
    case ThermalModes::Balanced:
        return 0xa0;
    case ThermalModes::Performance:
        return 0xa1;
    case ThermalModes::Cool:
        return 0xa2;
    case ThermalModes::Quiet:
        return 0xa3;
    case ThermalModes::FullSpeed:
        return 0xa4;
    case ThermalModes::BatterySaver:
        return 0xa5;
    case ThermalModes::Gmode:
        return 0xab;
    default:
        return 0x0; // fallback to Manual
    }
}

void Thermals::testThermalModes() {
    std::cout << "Device Name: " << Helper::getDeviceName() << "\n";
    std::cout << "Testing thermal modes...\n\n";

    std::vector<int> testModes;

    for (int i = 0xa0; i <= 0xa9; ++i) {
        testModes.push_back(i);
    }

    for (int i = 0xaa; i <= 0xab; ++i) {
        testModes.push_back(i);
    }

    for (int mode : testModes) {
        int result = m_acpiUtils.executeAcpiCommand(0x15, 0x01, mode, 0x00);

        // Find the mode name using reverse lookup
        std::string modeName = "Unknown";
        auto it = m_getCodeToMode.find(static_cast<std::uint16_t>(mode));
        if (it != m_getCodeToMode.end()) {
            modeName = m_modeToName[it->second];
        }

        std::cout << std::format("{:#x} -> {:#x} ({})\n", mode, result,
                                 modeName);
    }

    std::string bitmap = generateThermalModesBitmap();
    std::cout << "\nGenerated thermal modes bitmap for database.json:\n";
    std::cout << std::format("\"thermalModes\": \"{}\"\n", bitmap);

    std::cout << "\nReverting to balanced.\n";
    setThermalMode(ThermalModes::Balanced);
}

std::string Thermals::generateThermalModesBitmap() {
    std::bitset<8> bitmap{0};

    if (m_acpiUtils.executeAcpiCommand(
            0x15, 0x01, static_cast<int>(m_modeToSetCode[ThermalModes::Quiet]),
            0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Quiet);
    }
    if (m_acpiUtils.executeAcpiCommand(
            0x15, 0x01,
            static_cast<int>(m_modeToSetCode[ThermalModes::Balanced]),
            0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Balanced);
    }
    if (m_acpiUtils.executeAcpiCommand(
            0x15, 0x01,
            static_cast<int>(m_modeToSetCode[ThermalModes::Performance]),
            0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Performance);
    }
    if (m_acpiUtils.executeAcpiCommand(
            0x15, 0x01,
            static_cast<int>(m_modeToSetCode[ThermalModes::BatterySaver]),
            0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::BatterySaver);
    }
    if (m_acpiUtils.executeAcpiCommand(
            0x15, 0x01, static_cast<int>(m_modeToSetCode[ThermalModes::Cool]),
            0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Cool);
    }
    if (m_acpiUtils.executeAcpiCommand(
            0x15, 0x01,
            static_cast<int>(m_modeToSetCode[ThermalModes::FullSpeed]),
            0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::FullSpeed);
    }
    if (m_acpiUtils.executeAcpiCommand(
            0x15, 0x01, static_cast<int>(m_modeToSetCode[ThermalModes::Gmode]),
            0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::GMode);
    }
    if (m_acpiUtils.executeAcpiCommand(
            0x15, 0x01, static_cast<int>(m_modeToSetCode[ThermalModes::Manual]),
            0x00) == 0) {
        bitmap |= static_cast<uint8_t>(ThermalModeSet::Manual);
    }

    return bitmap.to_string();
}

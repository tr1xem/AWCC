#include "Thermals.h"
#include "database.h"
#include <loguru.hpp>

Thermals::~Thermals() { LOG_S(INFO) << "Thermals Module deinitialized"; }
Thermals::Thermals(AcpiUtils &acpiUtils) : m_acpiUtils(acpiUtils) {
    m_currentMode = queryThermalMode();
    LOG_S(INFO) << "Thermals Module initialized";
}

void Thermals::setThermalMode(ThermalModes mode) {
    if (!supportsThemeralMode(mode)) {
        LOG_S(ERROR) << m_thermalModeToName(mode)
                     << " mode not supported by device, aborting";
        return;
    } else {
        if (mode == ThermalModes::Gmode ||
            m_currentMode == ThermalModes::Gmode) {
            m_acpiUtils.executeAcpiCommand(0x25, 0x01);
        }
        if (mode == m_currentMode) {
            LOG_S(WARNING) << "Current thermal mode is already set to: "
                           << m_thermalModeToName(mode);
            return;
        } else {
            LOG_S(INFO) << "Setting thermal mode to: "
                        << m_thermalModeToName(mode);
            m_acpiUtils.executeAcpiCommand(0x15, 0x01, static_cast<int>(mode));
            m_currentMode = mode;
        }
    }
    // NOTE: ACPI Flag needed to set while setting GMODE
}

bool Thermals::supportsThemeralMode(ThermalModes mode) {
    ThermalModeSet bit = m_mapToBitset(mode);
    return m_acpiUtils.hasThermalMode(bit);
}

const char *Thermals::m_thermalModeToName(ThermalModes mode) {
    switch (mode) {
    case ThermalModes::Quiet:
        return "Quiet";
    case ThermalModes::Balanced:
        return "Balanced";
    case ThermalModes::Performance:
        return "Performance";
    case ThermalModes::BatterySaver:
        return "Battery Saver";
    case ThermalModes::Cool:
        return "Cool";
    case ThermalModes::Gmode:
        return "G-Mode";
    case ThermalModes::Manual:
        return "Manual";
    case ThermalModes::FullSpeed:
        return "Full Speed";
    default:
        return "Unknown";
    }
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
    int result = m_acpiUtils.executeAcpiCommand(0x14, 0x0b, 0x00);
    switch (result) {
    case 0xa3:
        m_currentMode = ThermalModes::Quiet;
        return ThermalModes::Quiet;
    case 0xa0:
        m_currentMode = ThermalModes::Balanced;
        return ThermalModes::Balanced;
    case 0xa1:
        m_currentMode = ThermalModes::Performance;
        return ThermalModes::Performance;
    case 0xa5:
        m_currentMode = ThermalModes::BatterySaver;
        return ThermalModes::BatterySaver;
    case 0xa2:
        m_currentMode = ThermalModes::Cool;
        return ThermalModes::Cool;
    case 0xa4:
        m_currentMode = ThermalModes::FullSpeed;
        return ThermalModes::FullSpeed;
    case 0xab:
        m_currentMode = ThermalModes::Gmode;
        return ThermalModes::Gmode;
    case 0x0:
        m_currentMode = ThermalModes::Manual;
        return ThermalModes::Manual;
    default:
        LOG_S(ERROR) << "Unknown thermal mode returned: 0x" << std::hex
                     << result;
        return ThermalModes::Manual; // fallback
    }
}

ThermalModes Thermals::getCurrentMode() { return queryThermalMode(); }
const char *Thermals::getCurrentModeName() {
    m_currentMode = queryThermalMode();
    return m_thermalModeToName(m_currentMode);
}

void Thermals::toggleGmode() {
    std::string deviceName = AcpiUtils::getDeviceName();
    if (queryThermalMode() == ThermalModes::Gmode) {
        if (deviceName.contains("Dell G15 5515")) {
            setThermalMode(ThermalModes::Manual);
            return;
        }
        setThermalMode(m_defaultMode);
    } else {
        setThermalMode(ThermalModes::Gmode);
    }
}

int Thermals::getCpuBoost() {
    return m_acpiUtils.executeAcpiCommand(0x14, 0x0c, 0x32);
}
int Thermals::getGpuBoost() {
    return m_acpiUtils.executeAcpiCommand(0x14, 0x0c, 0x33);
}
void Thermals::setCpuBoost(int boost) {
    m_acpiUtils.executeAcpiCommand(0x15, 0x02, 0x32, boost);
    LOG_S(INFO) << "Set CPU Boost to: " << boost << "%";
}
void Thermals::setGpuBoost(int boost) {
    m_acpiUtils.executeAcpiCommand(0x15, 0x02, 0x33, boost);
    LOG_S(INFO) << "Set GPU Boost to: " << boost << "%";
}

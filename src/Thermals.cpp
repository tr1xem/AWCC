#include "Thermals.h"
#include <loguru.hpp>

Thermals::Thermals(AcpiUtils &acpiUtils) : m_acpiUtils(acpiUtils) {

    LOG_S(INFO) << "Thermals Module initialized";
}

void Thermals::setThermalMode(ThermalModes mode) {
    // m_acpiUtils.deviceInfo();
    if (!supportsThemeralMode(mode)) {
        LOG_S(ERROR) << m_thermalModeToName(mode)
                     << " mode not supported by device, aborting";
        return;
    } else {
        LOG_S(INFO) << "Setting thermal mode to: " << m_thermalModeToName(mode);
        m_acpiUtils.executeAcpiCommand(0x15, 0x01, static_cast<int>(mode));
    }
}

auto Thermals::supportsThemeralMode(ThermalModes mode) -> bool {
    ThermalModeSet bit = m_mapToBitset(mode);
    return m_acpiUtils.hasThermalMode(bit);
}
auto Thermals::m_thermalModeToName(ThermalModes mode) -> const char * {
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
auto Thermals::m_mapToBitset(ThermalModes mode) -> ThermalModeSet {
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

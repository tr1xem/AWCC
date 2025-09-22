#include "Thermals.h"
#include <loguru.hpp>

Thermals::Thermals() { LOG_S(INFO) << "Thermals Module initialized"; }

void Thermals::setThermalMode(ThermalModes mode) {
    m_acpiUtils.deviceInfo();
    LOG_S(INFO) << "Setting thermal mode to: " << thermalModeToString(mode);
    if (!supportsThemeralMode(mode)) {
        LOG_S(ERROR) << thermalModeToString(mode)
                     << " mode not supported by device, aborting";
        return;
    } else {
        m_acpiUtils.executeAcpiCommand(0x15, 0x01, static_cast<int>(mode));
    }
}

auto Thermals::supportsThemeralMode(ThermalModes mode) -> bool {
    ThermalModeSet bit = m_mapToBitset(mode);
    return m_acpiUtils.hasThermalMode(bit);
}

#include "Thermals.h"
#include <loguru.hpp>

Thermals::Thermals() { LOG_S(INFO) << "Thermals Module initialized"; }

void Thermals::setThermalMode(ThermalModes mode) {
    // TODO: Add CpuCheck
    m_acpiUtils.deviceInfo();
    LOG_S(INFO) << "Setting thermal mode to: " << thermalModeToString(mode);
    if (supportsThemeralMode(mode) == false) {
        LOG_S(ERROR) << thermalModeToString(mode)
                     << " mode not supported by device, aborting";
        return;
    } else {
        m_acpiUtils.executeAcpiCommand("AWMM", 0x15, 0x01, mode);
    }
}

bool Thermals::supportsThemeralMode(ThermalModes mode) {
    ThermalModeSet bit = m_mapToBitset(mode);
    return m_acpiUtils.hasThermalMode(bit);
}

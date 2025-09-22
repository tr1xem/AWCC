#pragma once
#include "AcpiUtils.h"
#include "database.h"

class Thermals {
  private:
    static auto m_mapToBitset(ThermalModes mode) -> ThermalModeSet;
    static auto m_thermalModeToName(ThermalModes mode) -> const char *;
    AcpiUtils &m_acpiUtils; // reference to external AcpiUtils

  public:
    Thermals(AcpiUtils &acpiUtils);
    void setThermalMode(ThermalModes mode);
    auto supportsThemeralMode(ThermalModes mode) -> bool;
};

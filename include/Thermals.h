#pragma once
#include "AcpiUtils.h"
#include "database.h"

class Thermals {
  private:
    static auto m_mapToBitset(ThermalModes mode) -> ThermalModeSet;
    static auto m_thermalModeToName(ThermalModes mode) -> const char *;
    AcpiUtils &m_acpiUtils; // reference to external AcpiUtils
    ThermalModes m_currentMode;
    ThermalModes m_defaultMode{ThermalModes::Performance};

  public:
    Thermals(AcpiUtils &acpiUtils);
    void setThermalMode(ThermalModes mode);
    auto supportsThemeralMode(ThermalModes mode) -> bool;
    auto queryThermalMode() -> ThermalModes;
    auto getCurrentModeName() -> const char *;
    void toggleGmode();
};

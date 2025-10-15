#pragma once
#include "AcpiUtils.h"
#include "database.h"

class Thermals {
  private:
    static ThermalModeSet m_mapToBitset(ThermalModes mode);
    static const char *m_thermalModeToName(ThermalModes mode);
    AcpiUtils &m_acpiUtils; // reference to external AcpiUtils
    ThermalModes m_currentMode;
    ThermalModes m_defaultMode{ThermalModes::Performance};

  public:
    Thermals(AcpiUtils &acpiUtils);
    ~Thermals();
    void setThermalMode(ThermalModes mode);
    bool supportsThemeralMode(ThermalModes mode);
    void setCpuBoost(int boost);
    void setGpuBoost(int boost);
    int getCpuBoost();
    int getGpuBoost();
    ThermalModes getCurrentMode();

    ThermalModes queryThermalMode();
    const char *getCurrentModeName();
    void toggleGmode();
};

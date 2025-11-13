#pragma once
#include "AcpiUtils.h"
#include "database.h"
#include <string>
#include <unordered_map>

class Thermals {
  private:
    static ThermalModeSet m_mapToBitset(ThermalModes mode);
    const char *m_thermalModeToName(ThermalModes mode);

    std::unordered_map<ThermalModes, std::uint16_t> m_modeToSetCode;
    std::unordered_map<std::uint16_t, ThermalModes> m_getCodeToMode;
    std::unordered_map<ThermalModes, std::string> m_modeToName;

    void initializeThermalModes();
    void patchModes(const std::string &deviceName);

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

    // Static method to get thermal mode codes for testing purposes
    static std::uint16_t getThermalModeCode(ThermalModes mode);

    // Thermal mode testing functionality
    void testThermalModes();
    std::string generateThermalModesBitmap();
};

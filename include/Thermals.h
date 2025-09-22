#pragma once
#include "AcpiUtils.h"
#include "database.h"

class Thermals {
  private:
    AcpiUtils m_acpiUtils;

    ThermalModeSet m_mapToBitset(ThermalModes mode) {
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

    const char *thermalModeToString(ThermalModes mode) {
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

  public:
    Thermals();
    void setThermalMode(ThermalModes mode);
    bool supportsThemeralMode(ThermalModes mode);
};

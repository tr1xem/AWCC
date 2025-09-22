#pragma once
#include "AcpiUtils.h"

enum ThermalModes {
    Quiet = 0xa3,
    BatterySaver = 0xa5,
    Balanced = 0xa0,
    Performance = 0xa1,
    Gmode = 0xab,
    Cool = 0xa2,
    Manual = 0x0,
    USTT_FullSpeed = 0xa4, // Legacy
};

enum Prefix {
    Intel,
    AMD,
};
class Thermals {
  private:
    AcpiUtils m_acpiUtils;

    ThermalModeSet m_mapToBitset(ThermalModes mode) {
        switch (mode) {
        case Quiet:
            return b_Quiet;
        case Balanced:
            return b_Balanced;
        case Performance:
            return b_Performance;
        case BatterySaver:
            return b_BatterySaver;
        case Cool:
            return b_Cool;
        case USTT_FullSpeed:
            return b_FullSpeed; // if needed
        case Gmode:
            return b_GMode;
        case Manual:
            return b_Manual;
        default:
            return b_Manual; // fallback
        }
    }

    const char *thermalModeToString(ThermalModes mode) {
        switch (mode) {
        case Quiet:
            return "Quiet";
        case Balanced:
            return "Balanced";
        case Performance:
            return "Performance";
        case BatterySaver:
            return "Battery Saver";
        case Cool:
            return "Cool";
        case Gmode:
            return "GMode";
        case Manual:
            return "Manual";
        case USTT_FullSpeed:
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

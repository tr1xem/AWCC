#pragma once
#include <bitset>

enum FeatureSet {
    b_FanBoost = 0b00000001,          // 1 << 0
    b_ThermalModes = 0b00000010,      // 1 << 1
    b_AutoBoost = 0b00000100,         // 1 << 2
    b_CpuTemp = 0b00001000,           // 1 << 3
    b_GpuTemp = 0b00010000,           // 1 << 4
    b_BrightnessControl = 0b00100000, // 1 << 5
    b_GModeToggle = 0b01000000        // 1 << 6
};

enum ThermalModeSet {
    b_Quiet = 0b00000001,        // 1 << 0
    b_Balanced = 0b00000010,     // 1 << 1
    b_Performance = 0b00000100,  // 1 << 2
    b_BatterySaver = 0b00001000, // 1 << 3
    b_Cool = 0b00010000,         // 1 << 4
    b_FullSpeed = 0b00100000,    // 1 << 5
    b_GMode = 0b01000000,        // 1 << 6
    b_Manual = 0b10000000        // 1 << 7
};

enum LightingSet {
    b_StaticColor = 0b000001,     // 1 << 0
    b_SpectrumEffect = 0b000010,  // 1 << 1
    b_BreathingEffect = 0b000100, // 1 << 2
    b_RainbowEffect = 0b001000,   // 1 << 3
    b_WaveEffect = 0b010000,      // 1 << 4
    b_BackForthEffect = 0b100000  // 1 << 5
};

class AcpiUtils {
  private:
    int acpiModelId{};
    std::bitset<7> m_featureSetBits{"1111111"};
    std::bitset<8> m_thermalModeBits{"1111111"};
    std::bitset<6> m_lightingModesBits{"111111"};

  public:
    AcpiUtils();
    void executeAcpiCommand(const char *prefix, int arg1, int arg2 = 0x00,
                            int arg3 = 0x00, int arg4 = 0x00);
    bool hasFeature(FeatureSet f) const {
        return (m_featureSetBits.to_ulong() & static_cast<unsigned long>(f)) !=
               0;
    }

    bool hasThermalMode(ThermalModeSet m) const {
        return (m_thermalModeBits.to_ulong() & static_cast<unsigned long>(m)) !=
               0;
    }

    bool hasLightingMode(LightingSet l) const {
        return (m_lightingModesBits.to_ulong() &
                static_cast<unsigned long>(l)) != 0;
    }
    void deviceInfo() const;
};

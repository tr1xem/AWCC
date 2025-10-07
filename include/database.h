#pragma once
#include <cstdint>

enum class FeatureSet : std::uint8_t {
    FanBoost = 0b00000001,          // byte 0
    ThermalModes = 0b00000010,      // byte 1
    AutoBoost = 0b00000100,         // byte 2
    CpuTemp = 0b00001000,           // byte 3
    GpuTemp = 0b00010000,           // byte 4
    BrightnessControl = 0b00100000, // byte 5
    GModeToggle = 0b01000000        // byte 6
};

enum class ThermalModes : std::uint8_t {
    Manual = 0x0,
    Balanced = 0xa0,
    Performance = 0xa1,
    Cool = 0xa2,
    Quiet = 0xa3,
    FullSpeed = 0xa4, // Legacy
    BatterySaver = 0xa5,
    Gmode = 0xab,
};

enum class ThermalModeSet : std::uint8_t {
    Quiet = 0b00000001,        // byte 0
    Balanced = 0b00000010,     // byte 1
    Performance = 0b00000100,  // byte 2
    BatterySaver = 0b00001000, // byte 3
    Cool = 0b00010000,         // byte 4
    FullSpeed = 0b00100000,    // byte 5
    GMode = 0b01000000,        // byte 6
    Manual = 0b10000000        // byte 7
};

enum class LightingSet : std::uint8_t {
    StaticColor = 0b000001,     // byte 0
    SpectrumEffect = 0b000010,  // byte 1
    BreathingEffect = 0b000100, // byte 2
    RainbowEffect = 0b001000,   // byte 3
    WaveEffect = 0b010000,      // byte 4
    BackForthEffect = 0b100000  // byte 5
};

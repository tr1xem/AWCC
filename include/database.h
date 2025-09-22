#pragma once

enum Prefix {
    Intel,
    AMD,
};

enum FeatureSet {
    b_FanBoost = 0b00000001,          // byte 0
    b_ThermalModes = 0b00000010,      // byte 1
    b_AutoBoost = 0b00000100,         // byte 2
    b_CpuTemp = 0b00001000,           // byte 3
    b_GpuTemp = 0b00010000,           // byte 4
    b_BrightnessControl = 0b00100000, // byte 5
    b_GModeToggle = 0b01000000        // byte 6
};

enum ThermalModes {
    Quiet = 0xa3,
    Balanced = 0xa0,
    Performance = 0xa1,
    BatterySaver = 0xa5,
    Cool = 0xa2,
    FullSpeed = 0xa4, // Legacy
    Gmode = 0xab,
    Manual = 0x0,
};

enum ThermalModeSet {
    b_Quiet = 0b00000001,        // byte 0
    b_Balanced = 0b00000010,     // byte 1
    b_Performance = 0b00000100,  // byte 2
    b_BatterySaver = 0b00001000, // byte 3
    b_Cool = 0b00010000,         // byte 4
    b_FullSpeed = 0b00100000,    // byte 5
    b_GMode = 0b01000000,        // byte 6
    b_Manual = 0b10000000        // byte 7
};

enum LightingSet {
    b_StaticColor = 0b000001,     // byte 0
    b_SpectrumEffect = 0b000010,  // byte 1
    b_BreathingEffect = 0b000100, // byte 2
    b_RainbowEffect = 0b001000,   // byte 3
    b_WaveEffect = 0b010000,      // byte 4
    b_BackForthEffect = 0b100000  // byte 5
};

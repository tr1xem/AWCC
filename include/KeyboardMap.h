#pragma once

#include <cstdint>

// Shown on the keyboard diagram. Not a Darfon wire id; the deck power button
// is the chassis light, so keyboard color packets skip it.
constexpr uint8_t kKeyboardPowerId = 255;

// effect matches the keyboard menu: 0 static, 1 breathe, 2 spectrum, 3 wave,
// 4 rainbow, 5 back and forth, 6 default blue, 7 pulse.
// SampleEffectColor is a simple preview of that choice. It is not locked to
// the firmware clock. along is 0 at the left and 1 at the right.
void SampleEffectColor(int effect, uint8_t er, uint8_t eg, uint8_t eb,
                       float along, float timeSec, uint8_t out[3]);
void DrawArea51Keyboard(bool selected[256], const uint8_t color[256][3],
                        int effect, uint8_t effectR, uint8_t effectG,
                        uint8_t effectB, bool powerOwnColor, uint8_t powerR,
                        uint8_t powerG, uint8_t powerB, bool trackOwnColor,
                        int trackEffect, uint8_t trackR, uint8_t trackG,
                        uint8_t trackB);

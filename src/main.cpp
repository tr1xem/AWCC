#include <iostream>
#include <thread>

#include "config/autoboost.h"

int main() {
    // --- Build fan boost intervals (temperature -> boost mapping) ---
    AWCCBoostInterval_t cpuFanIntervals[] = {
        {{0, 50}, 0},     // 0-50°C  -> 0% boost
        {{51, 70}, 25},   // 51-70°C -> 25% boost
        {{71, 85}, 50},   // 71-85°C -> 50% boost
        {{86, 100}, 80},  // 86-100°C -> 80% boost
    };

    // --- Build mode intervals (temperature -> power profile mapping) ---
    AWCCModeInterval_t modeIntervals[] = {
        {AlienFan_SDK::BALANCED, {0, 60}},
        {AlienFan_SDK::BALANCED_PERFORMANCE, {61, 80}},
        {AlienFan_SDK::PERFORMANCE, {81, 100}},
    };

    AWCCFanConfig_t cpuFanConfig{};
    cpuFanConfig.Fan = {0, "CPU Fan"};
    cpuFanConfig.UpBoostShift = 10;
    cpuFanConfig.UpBoostShiftTime = 5;
    cpuFanConfig.BoostDownHysteresis = 5;
    cpuFanConfig.MinTimeBeforeBoostDown = 30;
    cpuFanConfig.MinTimeAfterShiftDown = 10;
    cpuFanConfig.PendingTime = 5;
    cpuFanConfig.BoostIntervals = cpuFanIntervals;
    cpuFanConfig._BoostIntervalCount = 4;

    // --- Super boost config ---
    AlienFan_SDK::ALIENFAN_FAN cpuFan(0, "cpu");
    AWCCSuperBoostConfig_t superBoost{};
    superBoost.fan = &cpuFan;
    superBoost.BoostEqualizationZoneMax = 3;
    superBoost.ShiftToLower = {
        .PendingTime = 10, .Time = 15, .IntervalOffset = 1};

    // --- Main config ---
    AutoBoostConfig_t config{};
    config.TemperatureCheckInterval = 2;  // check every 2 seconds
    config.MinTimeBeforeModeDown = 60;
    config.ModeDownHysteresis = 5;
    config.ModePendingTime = 10;
    config.FanConfigs = {cpuFanConfig};
    config.SuperBoostConfig = {superBoost};
    config.ModeIntervals = modeIntervals;
    config._ModeIntervalCount = 3;

    // --- Start autoboost ---
    AutoBoost autoboost;
    autoboost.Start(&config, &config, &config);  // same config for AC & BAT
    return 0;
}

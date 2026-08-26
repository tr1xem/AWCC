#pragma once
#include <array>
#include <ctime>
#include <functional>
#include <vector>

#include "AlienFan-SDK.h"
#include "helper.h"

using AWCCBoost_t = int;
using AWCCTemperature_t = int;
using AWCCFanRpm_t = int;

struct AutoBoost_Config {};

struct AWCCNoBoostConfig_t {
    enum AlienFan_SDK::ALIENFAN_PROFILE Mode;
    AWCCBoost_t* FanBoosts;
};

enum AWCCFanControl_t {
    AWCCFanControlUnchanged,
    AWCCFanControlSetAuto,
    AWCCFanControlSetManual,
};

enum AWCCModeControl_t {
    AWCCModeControlUnchanged,
    AWCCModeControlSetAuto,
    AWCCModeControlSetManual,
};

// NOLINTNEXTLINE(performance-enum-size)
enum class AWCCBoostPhase_t {
    AWCCBoostPhaseInitial,
    AWCCBoostPhaseUpShift,
    AWCCBoostPhaseNormal,
    AWCCBoostPhaseShiftToLower,
    AWCCBoostPhaseHelping,
};

struct AWCCFanConfig_t {             // The fan specific options
    AlienFan_SDK::ALIENFAN_FAN Fan;  //  Fan identifier for which the config is
    int UpBoostShift;      // The amount of the additional boost after boost up
    int UpBoostShiftTime;  // The time interval of being the additional boost
                           // active
    int BoostDownHysteresis;     // Hysteresis for down boosting
    int MinTimeBeforeBoostDown;  // The minimum time of the current boost being
                                 // active before boost down
    int MinTimeAfterShiftDown;   // The minimum time after shift down before
                                 // boost down
    int PendingTime;  // The minimum time of being in a new temperature range
                      // before switching to the corresponding boost level
    struct AWCCBoostInterval_t* BoostIntervals;
    // NOLINTNEXTLINE(cert-dcl51-cpp,bugprone-reserved-identifier,cert-dcl37-c)
    int _BoostIntervalCount;  // Internal variable keeping boost intervals count
                              // to keep the implementation simple
};

struct AWCCSuperBoostShiftToLower_t {
    int PendingTime;
    int Time;
    int IntervalOffset;
};

struct AWCCSuperBoostConfig_t {
    const AlienFan_SDK::ALIENFAN_FAN* fan;
    int BoostEqualizationZoneMax;  // The maximum boost interval to be active to
                                   // equalize the another boost interval if
                                   // it's lower

    // TODO: Skeleton. When BoostDownHysteresis isn't satisfied for a certain
    // amount of time, and the only condition preventing boost down is it, shift
    // up the boost level by the offset for a certain amount of time to drop the
    // temperature and set lower boost level.
    struct AWCCSuperBoostShiftToLower_t ShiftToLower;
};

struct AWCCTemperatureRange_t {
    AWCCTemperature_t Min;
    AWCCTemperature_t Max;
};

struct AWCCBoostInterval_t {  // A temperature range to the desired boost level
                              // mapping
    struct AWCCTemperatureRange_t TemperatureRange;
    AWCCBoost_t Boost;
};

struct AWCCModeInterval_t {  // A temperature range to the desired mode mapping
    AlienFan_SDK::ALIENFAN_PROFILE Profile;
    struct AWCCTemperatureRange_t TemperatureRange;
};

struct AutoBoostConfig_t {
    int TemperatureCheckInterval;  // The time interval in seconds between two
                                   // consecutive temperature checks
    int MinTimeBeforeModeDown;     // The minimum time before mode down happens
                                   // after temperature drop to lower mode zone
    int ModeDownHysteresis;        // Hysteresis for selecting a lower mode
    int ModePendingTime;  // The minimum time of being in a new temperature
                          // range before switching to the corresponding mode
    struct std::vector<AWCCSuperBoostConfig_t> SuperBoostConfig;
    struct std::vector<AWCCFanConfig_t> FanConfigs;
    struct AWCCModeInterval_t* ModeIntervals;
    // NOLINTNEXTLINE(cert-dcl51-cpp,bugprone-reserved-identifier,cert-dcl37-c)
    int _ModeIntervalCount;  // Internal variable keeping mode intervals count
                             // to keep the implementation simple
    struct AWCCNoBoostConfig_t NoBoostConf;
} extern const AWCCDefaultConfig;

struct BoostInfo {
    AlienFan_SDK::ALIENFAN_FAN Fan;  //  Fan identifier for which the config is
    bool Auto = true;
    AWCCTemperature_t Temperature;
    int BoostIntervalCurrent = -1;
    int BoostIntervalToSet;

    AWCCBoostPhase_t BoostPhase = AWCCBoostPhase_t::AWCCBoostPhaseInitial;

    time_t BoostSetTime;
    time_t UpShiftDownTime;
    time_t ShiftToLowerTime;

    enum class PendingState { None, Up, Down } BoostPendingState;

    time_t BoostPendingTime;

    AWCCBoost_t Boost = 0;

    bool PendingHysteresis;
    time_t PendingHysteresisTime;

    int MaxBoost;
};
struct ModeInfo {
    bool Auto;
    AWCCTemperature_t MaxTemp;
    enum AlienFan_SDK::ALIENFAN_PROFILE Mode;
    int ModeInterval;
    // NOLINTNEXTLINE(performance-enum-size)
    enum {
        AWCCModePhaseInitial,
        AWCCModePhaseNormal,
    } ModePhase;
    time_t ModeSetTime;
    // NOLINTNEXTLINE(performance-enum-size)
    enum {
        AWCCModePendingNone,
        AWCCModePendingUp,
        AWCCModePendingDown,
    } ModePendingState;
    time_t ModePendingTime;
};

// TODO: Implementation of diff config for ac and bat
class Internal {
   public:
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init,hicpp-explicit-conversions,hicpp-member-init)
    Internal(AlienFan_SDK::Control* Control)
        : Control(Control) {
              // BoostInfos.reserve(Control->fans.size());
          };
    AlienFan_SDK::Control* Control;
    const AutoBoostConfig_t* Config;
    std::array<AutoBoostConfig_t*, 2> ConfigsForPowerModes;
    std::time_t CurrentTime;
    std::vector<BoostInfo*> BoostInfos;
    ModeInfo ModeInfo;
    enum AWCCPowerState_t PowerState;

    void ResetBoostInfo(const AlienFan_SDK::ALIENFAN_FAN* fan);
    void ResetModeInfo();
    void HandleControl();
    void ManageMode();
    void ManageSuperBoost();
    void ManageFanBoost(const AlienFan_SDK::ALIENFAN_FAN* fan);
    void SetFanBoost(const AlienFan_SDK::ALIENFAN_FAN* fan, int boostInterval,
                     enum AWCCBoostPhase_t boostPhase);
    void SetMode(int modeInterval);

   private:
    const std::vector<std::pair<AWCCBoostPhase_t, const char*>>
        m_BoostPhaseNames{
            {AWCCBoostPhase_t::AWCCBoostPhaseNormal, "Normal"},
            {AWCCBoostPhase_t::AWCCBoostPhaseHelping, "Helping"},
            {AWCCBoostPhase_t::AWCCBoostPhaseInitial, "Initial"},
            {AWCCBoostPhase_t::AWCCBoostPhaseShiftToLower, "ShiftToLower"},
            {AWCCBoostPhase_t::AWCCBoostPhaseUpShift, "UpShift"},
        };
    BoostInfo* m_GetBoostInfo(const AlienFan_SDK::ALIENFAN_FAN* fan);
    const AWCCFanConfig_t* m_GetFanConfig(
        const AutoBoostConfig_t* config, const AlienFan_SDK::ALIENFAN_FAN* fan);
    const AWCCSuperBoostConfig_t* m_GetSuperBoostConfig(
        const AutoBoostConfig_t* config, const AlienFan_SDK::ALIENFAN_FAN* fan);
};

class AutoBoost {
   private:
    AlienFan_SDK::Control m_alienfan;
    Internal m_Internal;

   public:
    // TODO: Implemenet loading of config through constructor
    AutoBoost();
    void Start(const struct AutoBoostConfig_t* config,
               struct AutoBoostConfig_t* config_ac,
               struct AutoBoostConfig_t* config_bat);
};

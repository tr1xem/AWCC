#include "config/autoboost.h"

#include <algorithm>
#include <thread>

#include "helper.h"
#include "loguru.hpp"

AutoBoost::AutoBoost() : m_Internal(&m_alienfan) {
#if DEBUG
    LOG_F(INFO,
          "Intializing autoboost and probing devices for fans and sensors");
#endif
    m_alienfan.Probe();
}
void AutoBoost::Start(const struct AutoBoostConfig_t* config,
                      struct AutoBoostConfig_t* config_ac,
                      struct AutoBoostConfig_t* config_bat) {
    m_Internal.PowerState = Helper::PowerState();
    // TODO: is there a better way to have ac and bat config?
    m_Internal.ConfigsForPowerModes[AWCCPowerStateAC] = config_ac;
    m_Internal.ConfigsForPowerModes[AWCCPowerStateBAT] = config_bat;

    for (int i = 0; i < m_Internal.BoostInfos.size(); i++) {
        if (i < m_Internal.Config->FanConfigs.size()) {
            m_Internal.BoostInfos[i]->MaxBoost =
                m_Internal.Config->FanConfigs[i]._BoostIntervalCount - 1;
        }
    }

#if DEBUG
    LOG_F(INFO, "Starting autoboost for all fans");
#endif

    while (true) {
        for (int i = 0; i < m_Internal.BoostInfos.size(); i++) {
            m_Internal.BoostInfos[i]->Temperature =
                m_alienfan.GetTempValue(m_alienfan.sensors[i]);

            m_Internal.ModeInfo.MaxTemp =
                std::max(m_Internal.ModeInfo.MaxTemp,
                         m_Internal.BoostInfos[i]->Temperature);
        }
        const enum AWCCPowerState_t powerState = Helper::PowerState();
        if (powerState != m_Internal.PowerState) {
            m_Internal.Config = m_Internal.ConfigsForPowerModes[powerState];

            for (int i = 0; i < m_Internal.BoostInfos.size(); i++) {
                m_Internal.ResetBoostInfo(&m_alienfan.fans[i]);
            }

            m_Internal.ModeInfo.ModePhase = ModeInfo::AWCCModePhaseInitial;
            m_Internal.ModeInfo.ModeInterval = -1;
            m_Internal.PowerState = powerState;
        }
        m_Internal.CurrentTime = time(nullptr);
        m_Internal.HandleControl();
        m_Internal.ManageMode();

        if (AlienFan_SDK::ALIENFAN_PROFILE::PERFORMANCE !=
            m_Internal.ModeInfo.Mode) {
            m_Internal.ManageSuperBoost();
            for (int i = 0; i < m_Internal.BoostInfos.size(); i++) {
                m_Internal.ManageFanBoost(&m_alienfan.fans[i]);
            }
        }

        std::this_thread::sleep_for(
            std::chrono::seconds(m_Internal.Config->TemperatureCheckInterval));
    }
}
const AWCCFanConfig_t* Internal::m_GetFanConfig(
    const AutoBoostConfig_t* config, const AlienFan_SDK::ALIENFAN_FAN* fan) {
    if (!config || !fan) {
        return nullptr;
    }

    auto it = std::find_if(config->FanConfigs.begin(), config->FanConfigs.end(),
                           [fan](const AWCCFanConfig_t& fanConfig) {
                               // Adjust member comparison if ALIENFAN_FAN
                               // requires ID matching (e.g., fanConfig.Fan.id
                               // == fan->id)
                               return fanConfig.Fan.id == fan->id;
                           });

    return (it != config->FanConfigs.end()) ? &(*it) : nullptr;
}
BoostInfo* Internal::m_GetBoostInfo(const AlienFan_SDK::ALIENFAN_FAN* fan) {
    if (!fan) return nullptr;

    auto it = std::find_if(
        BoostInfos.begin(), BoostInfos.end(), [fan](const BoostInfo* info) {
            return info && info->Fan.id == fan->id;  // Assumes operator== is
                                                     // defined for ALIENFAN_FAN
        });

    return (it != BoostInfos.end()) ? *it : nullptr;
}

const AWCCSuperBoostConfig_t* Internal::m_GetSuperBoostConfig(
    const AutoBoostConfig_t* config, const AlienFan_SDK::ALIENFAN_FAN* fan) {
    if (!config || !fan) return nullptr;

    auto it = std::find_if(config->SuperBoostConfig.begin(),
                           config->SuperBoostConfig.end(),
                           [fan](const AWCCSuperBoostConfig_t& superBoost) {
                               if (!superBoost.fan) return false;

                               // 1. Address comparison (if pointers point to
                               // the exact same fan instance)
                               if (superBoost.fan == fan) return true;

                               // 2. Value/ID comparison (if pointers point to
                               // different copies of the same fan) Replace
                               // '.id' with your ALIENFAN_FAN struct's ID or
                               // type member if needed
                               return superBoost.fan->id == fan->id;
                           });

    if (it != config->SuperBoostConfig.end()) {
        LOG_F(1, "Found SuperBoostConfig for fan");
        return &(*it);
    }

    LOG_F(1, "No SuperBoostConfig found for fan");
    return nullptr;
}
void Internal::ManageFanBoost(const AlienFan_SDK::ALIENFAN_FAN* fan) {
    auto* currBoostInfo = m_GetBoostInfo(fan);
    auto* currFanConfig = m_GetFanConfig(Config, fan);
    auto* currSuperBoostConfig = m_GetSuperBoostConfig(Config, fan);
    if (currBoostInfo == nullptr || currFanConfig == nullptr) {
        LOG_F(FATAL, "currBoostInfo or currFanConfigs not found");
    }
    if (currBoostInfo->Auto == true) {
        return;
    }
    bool pending = false;
    bool pendingShiftToLower = false;
    if (AWCCBoostPhase_t::AWCCBoostPhaseInitial == currBoostInfo->BoostPhase) {
        SetFanBoost(fan, currBoostInfo->BoostIntervalToSet,
                    AWCCBoostPhase_t::AWCCBoostPhaseUpShift);
    } else if (AWCCBoostPhase_t::AWCCBoostPhaseHelping ==
               currBoostInfo->BoostPhase) {
        if (currBoostInfo->BoostIntervalCurrent !=
            currBoostInfo->BoostIntervalToSet) {
            SetFanBoost(fan, currBoostInfo->BoostIntervalToSet,
                        AWCCBoostPhase_t::AWCCBoostPhaseHelping);
        }
    } else if (currBoostInfo->BoostIntervalToSet >
               currBoostInfo->BoostIntervalCurrent) {
        pending = true;

        if (BoostInfo::PendingState::Up == currBoostInfo->BoostPendingState) {
            if (difftime(CurrentTime, currBoostInfo->BoostPendingTime) >=
                currFanConfig->PendingTime) {
                SetFanBoost(fan, currBoostInfo->BoostIntervalToSet,
                            AWCCBoostPhase_t::AWCCBoostPhaseUpShift);
            }
        } else {
            currBoostInfo->BoostPendingState = BoostInfo::PendingState::Up;
            currBoostInfo->BoostPendingTime = CurrentTime;
        }
    } else if (AWCCBoostPhase_t::AWCCBoostPhaseShiftToLower ==
                   currBoostInfo->BoostPhase &&
               difftime(CurrentTime, currBoostInfo->ShiftToLowerTime) >
                   currSuperBoostConfig->ShiftToLower.Time) {
        // FIXME: avoid this big jump
        SetFanBoost(fan, currBoostInfo->BoostIntervalToSet,
                    AWCCBoostPhase_t::AWCCBoostPhaseNormal);
    } else if (AWCCBoostPhase_t::AWCCBoostPhaseUpShift ==
                   currBoostInfo->BoostPhase &&
               currBoostInfo->BoostIntervalToSet <=
                   currBoostInfo->BoostIntervalCurrent  // alway true if
                                                        // reached this check
    ) {
        if (BoostInfo::PendingState::None == currBoostInfo->BoostPendingState) {
            if (difftime(CurrentTime, currBoostInfo->BoostSetTime) >=
                currFanConfig->UpBoostShiftTime) {
                SetFanBoost(fan, currBoostInfo->BoostIntervalCurrent,
                            AWCCBoostPhase_t::AWCCBoostPhaseNormal);
            }
        }
    } else if (AWCCBoostPhase_t::AWCCBoostPhaseNormal ==
                   currBoostInfo->BoostPhase &&
               currBoostInfo->BoostIntervalToSet <
                   currBoostInfo->BoostIntervalCurrent) {
        pending = true;

        if (BoostInfo::PendingState::Down == currBoostInfo->BoostPendingState) {
            if (difftime(CurrentTime, currBoostInfo->BoostPendingTime) >=
                currFanConfig->PendingTime) {
                if (
                    // difftime (currentTime, BoostInfos
                    // [fan].LastTimeInCurrentTemperatureInterval)
                    difftime(CurrentTime, currBoostInfo->BoostSetTime) >=
                    currFanConfig->MinTimeBeforeBoostDown /
                        (float)(currBoostInfo->BoostIntervalCurrent -
                                currBoostInfo->BoostIntervalToSet)) {
                    if (difftime(CurrentTime, currBoostInfo->UpShiftDownTime) >=
                        currFanConfig->MinTimeAfterShiftDown) {
                        if (currBoostInfo->Temperature <=
                            currFanConfig
                                    ->BoostIntervals[currBoostInfo
                                                         ->BoostIntervalCurrent]
                                    .TemperatureRange.Min -
                                currFanConfig->BoostDownHysteresis) {
                            SetFanBoost(fan,
                                        currBoostInfo->BoostIntervalCurrent - 1,
                                        AWCCBoostPhase_t::AWCCBoostPhaseNormal);
                        } else {
                            if (currBoostInfo->PendingHysteresis) {
                                if (difftime(
                                        CurrentTime,
                                        currBoostInfo->PendingHysteresisTime) >=
                                    currSuperBoostConfig->ShiftToLower
                                        .PendingTime) {
                                    SetFanBoost(
                                        fan, currBoostInfo->BoostIntervalToSet,
                                        AWCCBoostPhase_t::
                                            AWCCBoostPhaseShiftToLower);
                                }
                            } else {
                                pendingShiftToLower = true;
                            }
                        }
                    }
                }
            }
        } else {
            currBoostInfo->BoostPendingState = BoostInfo::PendingState::Down;
            currBoostInfo->BoostPendingTime = CurrentTime;
        }
    }

    if (1 == pendingShiftToLower && 0 == currBoostInfo->PendingHysteresis) {
        currBoostInfo->PendingHysteresis = true;
        currBoostInfo->PendingHysteresisTime = CurrentTime;
    } else if (0 == pendingShiftToLower &&
               1 == currBoostInfo->PendingHysteresis) {
        currBoostInfo->PendingHysteresis = false;
    }

    if (pending) {
        currBoostInfo->BoostPendingState = BoostInfo::PendingState::None;
    }
}
void Internal::ResetBoostInfo(const AlienFan_SDK::ALIENFAN_FAN* fan) {}
void Internal::ResetModeInfo() {}
void Internal::HandleControl() {}
void Internal::ManageMode() {}
void Internal::ManageSuperBoost() {}

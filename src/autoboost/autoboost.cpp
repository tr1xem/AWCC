#include "config/autoboost.h"

#include <algorithm>
#include <ranges>
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

#if DEBUG
        LOG_F(INFO, "Max temperature: %d", m_Internal.ModeInfo.MaxTemp);
#endif
        const enum AWCCPowerState_t powerState = Helper::PowerState();
        if (powerState != m_Internal.PowerState) {
            m_Internal.Config = m_Internal.ConfigsForPowerModes[powerState];
#if DEBUG
            LOG_F(INFO, "Power state changed from %d to %d",
                  m_Internal.PowerState, powerState);
#endif

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
#if DEBUG
        LOG_F(INFO, "Mode after ManageMode: %d, Phase: %d",
              m_Internal.ModeInfo.Mode, m_Internal.ModeInfo.ModePhase);
#endif

        if (AlienFan_SDK::ALIENFAN_PROFILE::PERFORMANCE !=
            m_Internal.ModeInfo.Mode) {
            m_Internal.ManageSuperBoost();
            for (int i = 0; i < m_Internal.BoostInfos.size(); i++) {
                m_Internal.ManageFanBoost(&m_alienfan.fans[i]);
#if DEBUG
                LOG_F(INFO,
                      "Fan %d - Current interval: %d, Target interval: %d, "
                      "Boost: %d",
                      i, m_Internal.BoostInfos[i]->BoostIntervalCurrent,
                      m_Internal.BoostInfos[i]->BoostIntervalToSet,
                      m_Internal.BoostInfos[i]->Boost);
            }
#endif
        }
    }

    LOG_F(INFO, "Sleeping for %d seconds",
          m_Internal.Config->TemperatureCheckInterval);
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
        LOG_F(ERROR, "Found SuperBoostConfig for fan");
        return &(*it);
    }

    LOG_F(ERROR, "No SuperBoostConfig found for fan");
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
enum { AWCCFanBoostMin = 0, AWCCFanBoostMax = 100 };

void Internal::SetFanBoost(const AlienFan_SDK::ALIENFAN_FAN* fan,
                           int boostInterval,
                           enum AWCCBoostPhase_t boostPhase) {
    const auto* currFanConfig = m_GetFanConfig(Config, fan);
    auto* currBoostInfo = m_GetBoostInfo(fan);
    const auto* currSuperBoostConfig = m_GetSuperBoostConfig(Config, fan);
    AWCCBoost_t boost = currFanConfig->BoostIntervals[boostInterval].Boost;
    if (AWCCFanBoostMin >= boost) {
        boost = AWCCFanBoostMin;
    } else if (AWCCFanBoostMax <= boost) {
        boost = AWCCFanBoostMax;
    } else {
        if (AWCCBoostPhase_t::AWCCBoostPhaseUpShift == boostPhase) {
            boost += currFanConfig->UpBoostShift;
        } else if (AWCCBoostPhase_t::AWCCBoostPhaseShiftToLower == boostPhase) {
            boost =
                currFanConfig
                    ->BoostIntervals[std::min(
                        currBoostInfo->MaxBoost,
                        currBoostInfo->BoostIntervalCurrent +
                            currSuperBoostConfig->ShiftToLower.IntervalOffset)]
                    .Boost;
            currBoostInfo->ShiftToLowerTime = CurrentTime;
        } else if (AWCCBoostPhase_t::AWCCBoostPhaseUpShift ==
                   currBoostInfo->BoostPhase) {
            currBoostInfo->UpShiftDownTime = CurrentTime;
        }
    }

    currBoostInfo->BoostPhase = boostPhase;

    Control->SetFanBoost(*fan, boost);
    currBoostInfo->Boost = boost;

    if (currBoostInfo->BoostIntervalCurrent != boostInterval) {
        currBoostInfo->BoostIntervalCurrent = boostInterval;
        currBoostInfo->BoostSetTime = CurrentTime;
    }
}
void Internal::SetMode(int modeInterval) {
    auto mode = Config->ModeIntervals[modeInterval].Profile;

    Control->SetPowerProfile(mode);

    if (mode != AlienFan_SDK::ALIENFAN_PROFILE::PERFORMANCE) {
        for (int i = 0; i < BoostInfos.size(); i++) {
            Control->SetFanBoost(Config->FanConfigs[i].Fan,
                                 BoostInfos[i]->Boost);
        }
    }

    if (ModeInfo.ModeInterval != modeInterval) {
        ModeInfo.Mode = mode;
        ModeInfo.ModeInterval = modeInterval;
        ModeInfo.ModeSetTime = CurrentTime;
    }
}

// TODO:
void Internal::ResetBoostInfo(const AlienFan_SDK::ALIENFAN_FAN* fan) {
    auto* currBoostInfo = m_GetBoostInfo(fan);
    const auto* currFanConfig = m_GetFanConfig(Config, fan);

    currBoostInfo->BoostPhase = AWCCBoostPhase_t::AWCCBoostPhaseInitial;
    currBoostInfo->BoostPendingState = BoostInfo::PendingState::None;
    currBoostInfo->BoostIntervalCurrent = -1;
    currBoostInfo->BoostIntervalToSet = -1;
    currBoostInfo->Boost = 0;
    currBoostInfo->MaxBoost = currFanConfig->_BoostIntervalCount - 1;
}
void Internal::HandleControl() {
    // TODO: What this does?
}
void Internal::ManageMode() {
    if (ModeInfo.Auto) {
        return;
    }

    int modeIntervalOfTemperature = 0;
    for (int i = 0; i < Config->_ModeIntervalCount; i++) {
        if (Config->ModeIntervals[i].TemperatureRange.Min <= ModeInfo.MaxTemp &&
            Config->ModeIntervals[i].TemperatureRange.Max >= ModeInfo.MaxTemp) {
            modeIntervalOfTemperature = i;
            break;
        }
    }
    bool pending = false;

    if (ModeInfo::AWCCModePhaseInitial == ModeInfo.ModePhase) {
        SetMode(modeIntervalOfTemperature);
        ModeInfo.ModePhase = ModeInfo::AWCCModePhaseNormal;
    } else if (modeIntervalOfTemperature > ModeInfo.ModeInterval) {
        pending = true;

        if (ModeInfo::AWCCModePendingUp == ModeInfo.ModePendingState) {
            if (difftime(CurrentTime, ModeInfo.ModePendingTime) >=
                Config->ModePendingTime) {
                SetMode(modeIntervalOfTemperature);
            }
        } else {
            ModeInfo.ModePendingState = ModeInfo::AWCCModePendingUp;
            ModeInfo.ModePendingTime = CurrentTime;
        }
    } else if (modeIntervalOfTemperature < ModeInfo.ModeInterval &&
               ModeInfo::AWCCModePhaseNormal == ModeInfo.ModePhase) {
        pending = true;

        if (ModeInfo::AWCCModePendingDown == ModeInfo.ModePendingState) {
            if (difftime(CurrentTime, ModeInfo.ModePendingTime) >=
                Config->ModePendingTime) {
                if (ModeInfo.MaxTemp <=
                    Config->ModeIntervals[ModeInfo.ModeInterval]
                            .TemperatureRange.Min -
                        Config->ModeDownHysteresis) {
                    if (difftime(CurrentTime, ModeInfo.ModeSetTime) >=
                        Config->MinTimeBeforeModeDown) {
                        SetMode(ModeInfo.ModeInterval - 1);
                    }
                }
            }
        } else {
            ModeInfo.ModePendingState = ModeInfo::AWCCModePendingDown;
            ModeInfo.ModePendingTime = CurrentTime;
        }
    }

    if (!pending) {
        ModeInfo.ModePendingState = ModeInfo::AWCCModePendingNone;
    }
}
void Internal::ManageSuperBoost() {
    struct FanInfo {
        int boostIntervalByTemperature{0};
    };
    std::vector<FanInfo> fanInfos(Config->FanConfigs.size());

    auto containsTemp = [](const auto& range, int temp) {
        return temp >= range.Min && temp <= range.Max;
    };
    int maxBoostInterval = 0;
    for (size_t i = 0; i < Config->FanConfigs.size(); ++i) {
        const auto& fanConfig = Config->FanConfigs[i];

        if (i >= BoostInfos.size() || !BoostInfos[i]) {
            continue;
        }

        const int currentTemp = BoostInfos[i]->Temperature;
        const auto intervals = std::views::counted(
            fanConfig.BoostIntervals, fanConfig._BoostIntervalCount);

        auto it = std::ranges::find_if(intervals, [&](const auto& interval) {
            return containsTemp(interval.TemperatureRange, currentTemp);
        });

        if (it != intervals.end()) {
            fanInfos[i].boostIntervalByTemperature =
                static_cast<int>(std::distance(intervals.begin(), it));
        }

        maxBoostInterval =
            std::max(maxBoostInterval, fanInfos[i].boostIntervalByTemperature);
    }

    int equalizationZoneMax = std::numeric_limits<int>::max();
    for (const auto& superBoost : Config->SuperBoostConfig) {
        equalizationZoneMax =
            std::min(equalizationZoneMax, superBoost.BoostEqualizationZoneMax);
    }
    const int minBoostIntervalToSet =
        std::min(equalizationZoneMax, maxBoostInterval);

    for (size_t i = 0; i < Config->FanConfigs.size(); ++i) {
        if (i >= BoostInfos.size() || !BoostInfos[i]) {
            continue;
        }

        auto& boostInfo = *BoostInfos[i];
        const int intervalByTemp = fanInfos[i].boostIntervalByTemperature;

        if (intervalByTemp < minBoostIntervalToSet) {
            boostInfo.BoostPhase = AWCCBoostPhase_t::AWCCBoostPhaseHelping;
            boostInfo.BoostIntervalToSet = minBoostIntervalToSet;
        } else {
            if (boostInfo.BoostPhase ==
                AWCCBoostPhase_t::AWCCBoostPhaseHelping) {
                boostInfo.BoostPhase = AWCCBoostPhase_t::AWCCBoostPhaseNormal;
                boostInfo.BoostSetTime = CurrentTime;
            }
            boostInfo.BoostIntervalToSet = intervalByTemp;
        }
    }
}
// TODO: Implementation
void Internal::ResetModeInfo() {}

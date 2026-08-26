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
                m_Internal.ResetBoostInfo(m_alienfan.fans[i]);
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
                m_Internal.ManageFanBoost(m_alienfan.fans[i]);
            }
        }

        std::this_thread::sleep_for(
            std::chrono::seconds(m_Internal.Config->TemperatureCheckInterval));
    }
}

# include "AWCCAutoBoost.h"

# include <threads.h>
# include <time.h>

# include "AWCC.h"
# include "AWCCConfig.h"

# error unfinished
# error untested

// WARN:
// 1) be careful handling UpBoostShiftTime and MinTimeBeforeBoostDown
// 2) be careful handling jumps over multiple boost intervals, go one by one
//    respecting MinTimeBeforeBoostDown

static void Start (struct AWCCConfig_t *);

struct AWCCBoost_t AWCCBoost = {
	.Start = & Start,
};

enum AWCCBoostPhase_t {
	AWCCBoostPhaseInitial,
	AWCCBoostPhaseUpShift,
	AWCCBoostPhaseNormal,
};

struct AWCCBoostInfo_t {
	AWCCTemperature_t Temperature;
	int BoostInterval;
	enum AWCCBoostPhase_t BoostPhase;
	time_t BoostSetTime;
};

static void ManageFanBoost (enum AWCCFan_t);
static void ManageMode (void);
static void SetFanBoost (enum AWCCFan_t, int, _Bool);

struct {
	void (* ManageFanBoost) (enum AWCCFan_t fan);
	void (* ManageMode) (void);
	void (* SetFanBoost) (enum AWCCFan_t, int, _Bool);
	const struct AWCCConfig_t * Config;
	struct AWCCBoostInfo_t BoostInfos [2];
} Internal = {
	.ManageFanBoost = & ManageFanBoost,
	.ManageMode = & ManageMode,
	.SetFanBoost = & SetFanBoost,
	.Config = NULL,
	.BoostInfos = {
		[AWCCFanCPU] = {
			.BoostPhase = AWCCBoostPhaseInitial,
			.BoostInterval = -1,
		},
		[AWCCFanGPU] = {
			.BoostPhase = AWCCBoostPhaseInitial,
			.BoostInterval = -1,
		},
	},
};

void Start (struct AWCCConfig_t * config)
{
	Internal.Config = config;

	while (1) {
		Internal.BoostInfos [AWCCFanCPU].Temperature  = AWCC.GetFanTemperature (AWCCFanCPU);
		Internal.BoostInfos [AWCCFanGPU].Temperature  = AWCC.GetFanTemperature (AWCCFanGPU);

		// if not G-Mode :
		Internal.ManageFanBoost (AWCCFanCPU);
		Internal.ManageFanBoost (AWCCFanGPU);
		Internal.ManageMode ();

		thrd_sleep (& (struct timespec) { .tv_sec = Internal.Config->TemperatureCheckInterval }, NULL);
	}
}

void ManageFanBoost (enum AWCCFan_t fan)
{
	int boostIntervalOfTemperature;

	for (int i = 0; i < Internal.Config->FanConfigs [fan]._BoostIntervalCount; i++) {
		if (
			Internal.Config->FanConfigs [fan].BoostIntervals [i].TemperatureRange.Min <= Internal.BoostInfos [fan].Temperature  &&
			Internal.Config->FanConfigs [fan].BoostIntervals [i].TemperatureRange.Max >= Internal.BoostInfos [fan].Temperature
		) {
			boostIntervalOfTemperature = i;
			break;
		}
	}

	if (AWCCBoostPhaseInitial == Internal.BoostInfos [fan].BoostPhase) {
		Internal.SetFanBoost (fan, boostIntervalOfTemperature, 1);
	}
	else if (boostIntervalOfTemperature > Internal.BoostInfos [fan].BoostInterval) {
		Internal.SetFanBoost (fan, boostIntervalOfTemperature, 1);
	}
	else if (
		   boostIntervalOfTemperature == Internal.BoostInfos [fan].BoostInterval
		&& AWCCBoostPhaseUpShift == Internal.BoostInfos [fan].BoostPhase
	) {
		if (difftime (time (NULL), Internal.BoostInfos [fan].BoostSetTime) > Internal.Config->FanConfigs [fan].UpBoostShiftTime) {
			Internal.SetFanBoost (fan, boostIntervalOfTemperature, 0);
		}
	}
	else if (
		   boostIntervalOfTemperature < Internal.BoostInfos [fan].BoostInterval
		&& AWCCBoostPhaseNormal == Internal.BoostInfos [fan].BoostPhase
	) {
		if (
			  Internal.BoostInfos [fan].Temperature
			< Internal.Config->FanConfigs [fan].BoostIntervals [Internal.BoostInfos [fan].BoostInterval].TemperatureRange.Min - Internal.Config->FanConfigs [fan].BoostDownHysteresis
		) {
			if (difftime (time (NULL), Internal.BoostInfos [fan].BoostSetTime) > Internal.Config [fan].FanConfigs [fan].MinTimeBeforeBoostDown) {
				Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostInterval - 1, 0);
			}
		}
	}
}

void ManageMode (void)
{
	AWCCTemperature_t tempMax; {
		tempMax = Internal.BoostInfos [AWCCFanCPU].Temperature;
		if (Internal.BoostInfos [AWCCFanGPU].Temperature > tempMax) {
			tempMax = Internal.BoostInfos [AWCCFanGPU].Temperature;
		}
	}

	int modeIntervalOfTemperature;

	for (int i = 0; i < Internal.Config->_ModeIntervalCount; i++) {
		if (
			Internal.Config->ModeIntervals [i].TemperatureRange.Min <= tempMax  &&
			Internal.Config->ModeIntervals [i].TemperatureRange.Max >= tempMax
		) {
			modeIntervalOfTemperature = i;
			break;
		}
	}
}

void SetFanBoost (enum AWCCFan_t fan, int boostInterval, _Bool upShift)
{
	AWCCBoost_t boost = Internal.Config->FanConfigs [fan].BoostIntervals [boostInterval].Boost;

	if (AWCCFanBoostMin > boost) {
		boost = AWCCFanBoostMin;
		Internal.BoostInfos [fan].BoostPhase = AWCCBoostPhaseNormal;
	}
	else if (AWCCFanBoostMax < boost) {
		boost = AWCCFanBoostMax;
		Internal.BoostInfos [fan].BoostPhase = AWCCBoostPhaseNormal;
	}
	else {
		if (1 == upShift) {
			Internal.BoostInfos [fan].BoostPhase = AWCCBoostPhaseUpShift;
			boost += Internal.Config->FanConfigs [fan].UpBoostShift;
		}
		else {
			Internal.BoostInfos [fan].BoostPhase = AWCCBoostPhaseNormal;
		}
	}

	AWCC.SetFanBoost (fan, boost);

	if (Internal.BoostInfos [fan].BoostInterval != boostInterval) {
		Internal.BoostInfos [fan].BoostInterval = boostInterval;
		Internal.BoostInfos [fan].BoostSetTime = time (NULL);
	}
}

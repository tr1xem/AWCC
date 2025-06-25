# include "AWCCAutoBoost.h"

#include <stdio.h>
# include <threads.h>
# include <time.h>

# include "AWCC.h"
# include "AWCCConfig.h"

# ifdef __STDC_NO_THREADS__
# error this tool currently depends on threads.h
# endif // __STDC_NO_THREADS__

// # define DRY_RUN
# define ENABLE_LOGS

// WARN:
// 1) be careful handling UpBoostShiftTime and MinTimeBeforeBoostDown
// 2) be careful handling jumps over multiple boost intervals, go one by one
//    respecting MinTimeBeforeBoostDown

static void Start (const struct AWCCConfig_t *);

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

enum AWCCModePhase_t {
	AWCCModePhaseInitial,
	AWCCModePhaseNormal,
};

struct AWCCModeInfo_t {
	AWCCTemperature_t MaxTemp;
	enum AWCCMode_t Mode;
	int ModeInterval;
	enum AWCCModePhase_t ModePhase;
	time_t ModeSetTime;
};

static void ManageFanBoost (enum AWCCFan_t);
static void ManageMode (void);
static void SetFanBoost (enum AWCCFan_t, int, _Bool);
static void SetMode (int);

struct {
	void (* ManageFanBoost) (enum AWCCFan_t fan);
	void (* ManageMode) (void);
	void (* SetFanBoost) (enum AWCCFan_t, int, _Bool);
	void (* SetMode) (int);
	const struct AWCCConfig_t * Config;
	struct AWCCBoostInfo_t BoostInfos [2];
	struct AWCCModeInfo_t ModeInfo;
	const char * FanNames [2];
} Internal = {
	.ManageFanBoost = & ManageFanBoost,
	.ManageMode = & ManageMode,
	.SetFanBoost = & SetFanBoost,
	.SetMode = & SetMode,
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
	.ModeInfo = {
		.Mode = AWCCModeQuiet,
		.ModeInterval = -1,
		.ModePhase = AWCCModePhaseInitial,
	},
	.FanNames = {
		[AWCCFanCPU] = "CPU",
		[AWCCFanGPU] = "GPU",
	},
};

void Start (const struct AWCCConfig_t * config)
{
	Internal.Config = config;

	while (1) {
		Internal.BoostInfos [AWCCFanCPU].Temperature  = AWCC.GetFanTemperature (AWCCFanCPU);
		Internal.BoostInfos [AWCCFanGPU].Temperature  = AWCC.GetFanTemperature (AWCCFanGPU);

		Internal.ModeInfo.MaxTemp = Internal.BoostInfos [AWCCFanCPU].Temperature;
		if (Internal.BoostInfos [AWCCFanGPU].Temperature > Internal.ModeInfo.MaxTemp) {
			Internal.ModeInfo.MaxTemp = Internal.BoostInfos [AWCCFanGPU].Temperature;
		}

# ifdef ENABLE_LOGS
		printf (
			"\tCPU %d[%d] GPU %d[%d]\n",
			Internal.BoostInfos [AWCCFanCPU].Temperature,
			AWCC.GetCpuBoost (),
			Internal.BoostInfos [AWCCFanGPU].Temperature,
			AWCC.GetGpuBoost ()
		);
# endif // ENABLE_LOGS

		// FIXME: changing mode seems to reset boosts, at least when changing
		// from performance to balanced
		Internal.ManageMode ();

		if (AWCCModeG != Internal.ModeInfo.Mode) {
			Internal.ManageFanBoost (AWCCFanCPU);
			Internal.ManageFanBoost (AWCCFanGPU);
		}

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
	int modeIntervalOfTemperature;

	for (int i = 0; i < Internal.Config->_ModeIntervalCount; i++) {
		if (
			Internal.Config->ModeIntervals [i].TemperatureRange.Min <= Internal.ModeInfo.MaxTemp  &&
			Internal.Config->ModeIntervals [i].TemperatureRange.Max >= Internal.ModeInfo.MaxTemp
		) {
			modeIntervalOfTemperature = i;
			break;
		}
	}

	if (AWCCModePhaseInitial == Internal.ModeInfo.ModePhase) {
		Internal.SetMode (modeIntervalOfTemperature);
		Internal.ModeInfo.ModePhase = AWCCModePhaseNormal;
	}
	else if (modeIntervalOfTemperature > Internal.ModeInfo.ModeInterval) {
		Internal.SetMode (modeIntervalOfTemperature);
	}
	else if (
		   modeIntervalOfTemperature < Internal.ModeInfo.ModeInterval
		&& AWCCModePhaseNormal == Internal.ModeInfo.ModePhase
	) {
		if (
			  Internal.ModeInfo.MaxTemp
			< Internal.Config->ModeIntervals [Internal.ModeInfo.ModeInterval].TemperatureRange.Min - Internal.Config->ModeDownHysteresis
		) {
			if (difftime (time (NULL), Internal.ModeInfo.ModeSetTime) > Internal.Config->MinTimeBeforeModeDown) {
				Internal.SetMode (Internal.ModeInfo.ModeInterval - 1);
			}
		}
	}
}

void SetFanBoost (enum AWCCFan_t fan, int boostInterval, _Bool upShift)
{
	AWCCBoost_t boost = Internal.Config->FanConfigs [fan].BoostIntervals [boostInterval].Boost;

	if (AWCCFanBoostMin >= boost) {
		boost = AWCCFanBoostMin;
		Internal.BoostInfos [fan].BoostPhase = AWCCBoostPhaseNormal;
	}
	else if (AWCCFanBoostMax <= boost) {
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

# ifdef ENABLE_LOGS
	printf ("%s BOOST %3d INTERVAL %d UPSHIFT %d\n", Internal.FanNames [fan], boost, boostInterval, upShift);
# endif // ENABLE_LOGS

# ifndef DRY_RUN
	AWCC.SetFanBoost (fan, boost);
# endif // DRY_RUN

	if (Internal.BoostInfos [fan].BoostInterval != boostInterval) {
		Internal.BoostInfos [fan].BoostInterval = boostInterval;
		Internal.BoostInfos [fan].BoostSetTime = time (NULL);
	}
}

void SetMode (int modeInterval)
{
	enum AWCCMode_t mode = Internal.Config->ModeIntervals [modeInterval].Mode;

# ifdef ENABLE_LOGS
	printf ("Mode %s\n", AWCC.GetModeName (mode));
# endif // ENABLE_LOGS

# ifndef DRY_RUN
	AWCC.SetMode (mode);
# endif // DRY_RUN

	if (Internal.ModeInfo.ModeInterval != modeInterval) {
		Internal.ModeInfo.Mode = mode;
		Internal.ModeInfo.ModeInterval = modeInterval;
		Internal.ModeInfo.ModeSetTime = time (NULL);
	}
}

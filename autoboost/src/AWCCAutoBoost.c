# include "AWCCAutoBoost.h"

# include <stdio.h>
# include <sys/stat.h>
# include <threads.h>
# include <time.h>

# include "AWCC.h"
# include "AWCCConfig.h"

# ifdef __STDC_NO_THREADS__
# error this tool currently depends on threads.h
# endif // __STDC_NO_THREADS__

// # define DRY_RUN

static void Start (const struct AWCCConfig_t *, const struct AWCCSystemLogger_t *);

struct AWCCBoost_t AWCCBoost = {
	.Start = & Start,
};

static void ManageFanBoost (enum AWCCFan_t);
static void ManageMode (void);
static void SetFanBoost (enum AWCCFan_t, int, _Bool);
static void SetMode (int);

struct {
	const struct AWCCConfig_t * Config;
	const struct AWCCSystemLogger_t * SystemLogger;
	struct {
		AWCCTemperature_t Temperature;
		int BoostInterval;
		enum {
			AWCCBoostPhaseInitial,
			AWCCBoostPhaseUpShift,
			AWCCBoostPhaseNormal,
		} BoostPhase;
		time_t BoostSetTime;
		AWCCBoost_t Boost;
	} BoostInfos [2];
	struct {
		AWCCTemperature_t MaxTemp;
		enum AWCCMode_t Mode;
		int ModeInterval;
		enum {
			AWCCModePhaseInitial,
			AWCCModePhaseNormal,
		} ModePhase;
		time_t ModeSetTime;
	} ModeInfo;
	const char * FanNames [2];
	void (* ManageFanBoost) (enum AWCCFan_t fan);
	void (* ManageMode) (void);
	void (* SetFanBoost) (enum AWCCFan_t, int, _Bool);
	void (* SetMode) (int);
} static Internal = {
	.Config = NULL,
	.SystemLogger = NULL,
	.BoostInfos = {
		[AWCCFanCPU] = {
			.BoostPhase = AWCCBoostPhaseInitial,
			.BoostInterval = -1,
			.Boost = 0,
		},
		[AWCCFanGPU] = {
			.BoostPhase = AWCCBoostPhaseInitial,
			.BoostInterval = -1,
			.Boost = 0,
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
	.ManageFanBoost = & ManageFanBoost,
	.ManageMode = & ManageMode,
	.SetFanBoost = & SetFanBoost,
	.SetMode = & SetMode,
};

void Start (const struct AWCCConfig_t * config, const struct AWCCSystemLogger_t * systemLogger)
{
	Internal.Config = config;
	Internal.SystemLogger = systemLogger;

	if (NULL != Internal.SystemLogger) {
		mkdir (Internal.SystemLogger->Dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}

	while (1) {
		Internal.BoostInfos [AWCCFanCPU].Temperature = AWCC.GetFanTemperature (AWCCFanCPU);
		Internal.BoostInfos [AWCCFanGPU].Temperature = AWCC.GetFanTemperature (AWCCFanGPU);

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

		Internal.ManageMode ();

		if (AWCCModeG != Internal.ModeInfo.Mode) {
			Internal.ManageFanBoost (AWCCFanCPU);
			Internal.ManageFanBoost (AWCCFanGPU);
		} // WARN: what happens with boosts after exiting G Mode?
		// NOTE: Seems to work fine.

		if (NULL != Internal.SystemLogger) {
			Internal.SystemLogger->LogCpuTemp (Internal.BoostInfos [AWCCFanCPU].Temperature);
			Internal.SystemLogger->LogGpuTemp (Internal.BoostInfos [AWCCFanGPU].Temperature);
			if (AWCCModeG != Internal.ModeInfo.Mode) {
				Internal.SystemLogger->LogCpuBoost (Internal.BoostInfos [AWCCFanCPU].Boost);
				Internal.SystemLogger->LogGpuBoost (Internal.BoostInfos [AWCCFanGPU].Boost);
			}
			else {
				Internal.SystemLogger->LogCpuBoost (AWCC.GetCpuBoost ());
				Internal.SystemLogger->LogGpuBoost (AWCC.GetGpuBoost ());
			}
			Internal.SystemLogger->LogMode (Internal.ModeInfo.Mode);
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
		   boostIntervalOfTemperature <= Internal.BoostInfos [fan].BoostInterval
		&& AWCCBoostPhaseUpShift == Internal.BoostInfos [fan].BoostPhase
	) {
		if (difftime (time (NULL), Internal.BoostInfos [fan].BoostSetTime) > Internal.Config->FanConfigs [fan].UpBoostShiftTime) {
			Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostInterval, 0);
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
			if (
				  difftime (time (NULL), Internal.BoostInfos [fan].BoostSetTime)
				> Internal.Config->FanConfigs [fan].MinTimeBeforeBoostDown / (float) (Internal.BoostInfos [fan].BoostInterval - boostIntervalOfTemperature)
			) {
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
	Internal.BoostInfos [fan].Boost = boost;
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

	if (AWCCModeG != mode) {
		AWCC.SetCpuBoost (Internal.BoostInfos [AWCCFanCPU].Boost);
		AWCC.SetGpuBoost (Internal.BoostInfos [AWCCFanGPU].Boost);
	}
# endif // DRY_RUN

	if (Internal.ModeInfo.ModeInterval != modeInterval) {
		Internal.ModeInfo.Mode = mode;
		Internal.ModeInfo.ModeInterval = modeInterval;
		Internal.ModeInfo.ModeSetTime = time (NULL);
	}
}

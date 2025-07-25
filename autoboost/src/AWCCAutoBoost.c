# include "AWCCAutoBoost.h"

# include <stdio.h>
# include <sys/stat.h>
# include <threads.h>
# include <time.h>

# include "AWCC.h"
# include "AWCCControl.h"

# ifdef __STDC_NO_THREADS__
# error this tool currently depends on threads.h
# endif // __STDC_NO_THREADS__

// # define DRY_RUN

static void Start (const struct AWCCConfig_t *, const struct AWCCConfig_t *, const struct AWCCSystemLogger_t *, const struct AWCCControl_t *);

const struct AWCCAutoBoost_t AWCCAutoBoost = {
	.Start = & Start,
};

static void ManageFanBoost (enum AWCCFan_t);
static void ManageMode (void);
static void SetFanBoost (enum AWCCFan_t, int, _Bool);
static void SetMode (int);
static void HandleControl (void);

struct {
	const struct AWCCConfig_t * Config;
	const struct AWCCConfig_t * Configs [2];
	const struct AWCCSystemLogger_t * SystemLogger;
	const struct AWCCControl_t * Control;

	time_t CurrentTime;
	struct {
		_Bool Auto;
		AWCCTemperature_t Temperature;
		int BoostInterval;
		enum {
			AWCCBoostPhaseInitial,
			AWCCBoostPhaseUpShift,
			AWCCBoostPhaseNormal,
		} BoostPhase;
		time_t BoostSetTime;
		// time_t LastTimeInCurrentTemperatureInterval;
		time_t UpShiftDownTime;
		enum {
			AWCCBoostPendingNone,
			AWCCBoostPendingUp,
			AWCCBoostPendingDown,
		} BoostPendingState;
		time_t BoostPendingTime;
		AWCCBoost_t Boost;
	} BoostInfos [2];
	struct {
		_Bool Auto;
		AWCCTemperature_t MaxTemp;
		enum AWCCMode_t Mode;
		int ModeInterval;
		enum {
			AWCCModePhaseInitial,
			AWCCModePhaseNormal,
		} ModePhase;
		time_t ModeSetTime;
		enum {
			AWCCModePendingNone,
			AWCCModePendingUp,
			AWCCModePendingDown,
		} ModePendingState;
		time_t ModePendingTime;
	} ModeInfo;
	enum AWCCPowerState_t PowerState;
	const char * FanNames [2];
	void (* ManageFanBoost) (enum AWCCFan_t fan);
	void (* ManageMode) (void);
	void (* SetFanBoost) (enum AWCCFan_t, int, _Bool);
	void (* SetMode) (int);
	void (* HandleControl) (void);
} static Internal = {
	.Config = NULL,
	.Configs = { NULL },
	.SystemLogger = NULL,
	.Control = NULL,

	.BoostInfos = {
		[AWCCFanCPU] = {
			.Auto = 1,
			.BoostPhase = AWCCBoostPhaseInitial,
			.BoostInterval = -1,
			.Boost = 0,
		},
		[AWCCFanGPU] = {
			.Auto = 1,
			.BoostPhase = AWCCBoostPhaseInitial,
			.BoostInterval = -1,
			.Boost = 0,
		},
	},
	.ModeInfo = {
		.Auto = 1,
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
	.HandleControl = & HandleControl,
};

void Start (const struct AWCCConfig_t * config_ac, const struct AWCCConfig_t * config_bat, const struct AWCCSystemLogger_t * systemLogger, const struct AWCCControl_t * control)
{
	Internal.SystemLogger = systemLogger;
	Internal.Control = control;

	Internal.Configs [AWCCPowerStateAC] = config_ac;
	Internal.Configs [AWCCPowerStateBAT] = config_bat;
	Internal.PowerState = AWCC.PowerState ();

	Internal.Config = Internal.Configs [Internal.PowerState];

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

		const enum AWCCPowerState_t powerState = AWCC.PowerState ();

		if (powerState != Internal.PowerState) {
			Internal.Config = Internal.Configs [powerState];
			Internal.BoostInfos [AWCCFanCPU].BoostPhase = AWCCBoostPhaseInitial;
			Internal.BoostInfos [AWCCFanCPU].BoostInterval = -1;
			Internal.BoostInfos [AWCCFanGPU].BoostPhase = AWCCBoostPhaseInitial;
			Internal.BoostInfos [AWCCFanGPU].BoostInterval = -1;
			Internal.ModeInfo.ModePhase = AWCCModePhaseInitial;
			Internal.ModeInfo.ModeInterval = -1;
			Internal.PowerState = powerState;
		}

		Internal.CurrentTime = time (NULL);

# ifdef ENABLE_LOGS
		printf (
			"\tCPU %d[%d] GPU %d[%d]\n",
			Internal.BoostInfos [AWCCFanCPU].Temperature,
			AWCC.GetCpuBoost (),
			Internal.BoostInfos [AWCCFanGPU].Temperature,
			AWCC.GetGpuBoost ()
		);
# endif // ENABLE_LOGS

		Internal.HandleControl ();

		Internal.ManageMode ();

		if (AWCCModeG != Internal.ModeInfo.Mode) {
			Internal.ManageFanBoost (AWCCFanCPU);
			Internal.ManageFanBoost (AWCCFanGPU);
		}

		if (NULL != Internal.SystemLogger) { // TODO: Separate system logger data in Internal
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
			Internal.SystemLogger->LogCpuRpm (AWCC.GetCpuRpm ());
			Internal.SystemLogger->LogGpuRpm (AWCC.GetGpuRpm ());
		}

		thrd_sleep (& (struct timespec) { .tv_sec = Internal.Config->TemperatureCheckInterval }, NULL);
	}
}

void ManageFanBoost (enum AWCCFan_t fan)
{
	if (0 == Internal.BoostInfos [fan].Auto) {
		return;
	}

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

	// if (boostIntervalOfTemperature == Internal.BoostInfos [fan].BoostInterval) {
	// 	Internal.BoostInfos [fan].LastTimeInCurrentTemperatureInterval = currentTime;
	// }

	_Bool pending = 0;

	if (AWCCBoostPhaseInitial == Internal.BoostInfos [fan].BoostPhase) {
		Internal.SetFanBoost (fan, boostIntervalOfTemperature, 1);
	}
	else if (boostIntervalOfTemperature > Internal.BoostInfos [fan].BoostInterval) {
		pending = 1;

		if (AWCCBoostPendingUp == Internal.BoostInfos [fan].BoostPendingState) {
			if (difftime (Internal.CurrentTime, Internal.BoostInfos [fan].BoostPendingTime) >= Internal.Config->FanConfigs [fan].PendingTime) {
				Internal.SetFanBoost (fan, boostIntervalOfTemperature, 1);
			}
		}
		else {
			Internal.BoostInfos [fan].BoostPendingState = AWCCBoostPendingUp;
			Internal.BoostInfos [fan].BoostPendingTime = Internal.CurrentTime;
		}
	}
	else if (
		   boostIntervalOfTemperature <= Internal.BoostInfos [fan].BoostInterval
		&& AWCCBoostPhaseUpShift == Internal.BoostInfos [fan].BoostPhase
	) {
		if (AWCCBoostPendingNone == Internal.BoostInfos [fan].BoostPendingState) {
			if (difftime (Internal.CurrentTime, Internal.BoostInfos [fan].BoostSetTime) >= Internal.Config->FanConfigs [fan].UpBoostShiftTime) {
				Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostInterval, 0);
			}
		}
	}
	else if (
		   boostIntervalOfTemperature < Internal.BoostInfos [fan].BoostInterval
		&& AWCCBoostPhaseNormal == Internal.BoostInfos [fan].BoostPhase
	) {
		pending = 1;

		if (AWCCBoostPendingDown == Internal.BoostInfos [fan].BoostPendingState) {
			if (difftime (Internal.CurrentTime, Internal.BoostInfos [fan].BoostPendingTime) >= Internal.Config->FanConfigs [fan].PendingTime) {
				if (
					   Internal.BoostInfos [fan].Temperature
					<= Internal.Config->FanConfigs [fan].BoostIntervals [Internal.BoostInfos [fan].BoostInterval].TemperatureRange.Min - Internal.Config->FanConfigs [fan].BoostDownHysteresis
				) {
					if (
						  // difftime (currentTime, Internal.BoostInfos [fan].LastTimeInCurrentTemperatureInterval)
						   difftime (Internal.CurrentTime, Internal.BoostInfos [fan].BoostSetTime)
						>= Internal.Config->FanConfigs [fan].MinTimeBeforeBoostDown / (float) (Internal.BoostInfos [fan].BoostInterval - boostIntervalOfTemperature)
					) {
						if (difftime (Internal.CurrentTime, Internal.BoostInfos [fan].UpShiftDownTime) >= Internal.Config->FanConfigs [fan].MinTimeAfterShiftDown) {
							Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostInterval - 1, 0);
						}
					}
				}
			}
		}
		else {
			Internal.BoostInfos [fan].BoostPendingState = AWCCBoostPendingDown;
			Internal.BoostInfos [fan].BoostPendingTime = Internal.CurrentTime;
		}
	}

	if (0 == pending) {
		Internal.BoostInfos [fan].BoostPendingState = AWCCBoostPendingNone;
	}
}

void ManageMode (void)
{
	if (0 == Internal.ModeInfo.Auto) {
		return;
	}

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

	_Bool pending = 0;

	if (AWCCModePhaseInitial == Internal.ModeInfo.ModePhase) {
		Internal.SetMode (modeIntervalOfTemperature);
		Internal.ModeInfo.ModePhase = AWCCModePhaseNormal;
	}
	else if (modeIntervalOfTemperature > Internal.ModeInfo.ModeInterval) {
		pending = 1;

		if (AWCCModePendingUp == Internal.ModeInfo.ModePendingState) {
			if (difftime (Internal.CurrentTime, Internal.ModeInfo.ModePendingTime) >= Internal.Config->ModePendingTime) {
				Internal.SetMode (modeIntervalOfTemperature);
			}
		}
		else {
			Internal.ModeInfo.ModePendingState = AWCCModePendingUp;
			Internal.ModeInfo.ModePendingTime = Internal.CurrentTime;
		}
	}
	else if (
		   modeIntervalOfTemperature < Internal.ModeInfo.ModeInterval
		&& AWCCModePhaseNormal == Internal.ModeInfo.ModePhase
	) {
		pending = 1;

		if (AWCCModePendingDown == Internal.ModeInfo.ModePendingState) {
			if (difftime (Internal.CurrentTime, Internal.ModeInfo.ModePendingTime) >= Internal.Config->ModePendingTime) {
				if (
					   Internal.ModeInfo.MaxTemp
					<= Internal.Config->ModeIntervals [Internal.ModeInfo.ModeInterval].TemperatureRange.Min - Internal.Config->ModeDownHysteresis
				) {
					if (difftime (Internal.CurrentTime, Internal.ModeInfo.ModeSetTime) >= Internal.Config->MinTimeBeforeModeDown) {
						Internal.SetMode (Internal.ModeInfo.ModeInterval - 1);
					}
				}
			}
		}
		else {
			Internal.ModeInfo.ModePendingState = AWCCModePendingDown;
			Internal.ModeInfo.ModePendingTime = Internal.CurrentTime;
		}
	}

	if (0 == pending) {
		Internal.ModeInfo.ModePendingState = AWCCModePendingNone;
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
			Internal.BoostInfos [fan].UpShiftDownTime = Internal.CurrentTime;
		}
	}

# ifdef ENABLE_LOGS
	printf ("%s BOOST %3d INTERVAL %d UPSHIFT %d\n", Internal.FanNames [fan], boost, boostInterval, upShift);
# endif // ENABLE_LOGS

# ifndef DRY_RUN
	AWCC.SetFanBoost (fan, boost);
	Internal.BoostInfos [fan].Boost = boost;
# endif // DRY_RUN

	// Internal.BoostInfos [fan].LastTimeInCurrentTemperatureInterval = Internal.CurrentTime;

	if (Internal.BoostInfos [fan].BoostInterval != boostInterval) {
		Internal.BoostInfos [fan].BoostInterval = boostInterval;
		Internal.BoostInfos [fan].BoostSetTime = Internal.CurrentTime;
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
		Internal.ModeInfo.ModeSetTime = Internal.CurrentTime;
	}
}

void HandleControl (void)
{
	struct {
		enum AWCCModeControl_t modeControlState;
		enum AWCCFanControl_t cpuControlState;
		enum AWCCFanControl_t gpuControlState;
		enum AWCCMode_t mode;
		AWCCBoost_t cpuBoost;
		AWCCBoost_t gpuBoost;
	} static _Thread_local autoControl;

	if (NULL != Internal.Control) {
		autoControl.modeControlState = Internal.Control->GetModeControlState (& autoControl.mode);
		autoControl.cpuControlState = Internal.Control->GetCpuControlState (& autoControl.cpuBoost);
		autoControl.gpuControlState = Internal.Control->GetGpuControlState (& autoControl.gpuBoost);

		if (AWCCModeControlSetAuto == autoControl.modeControlState) {
			if (0 == Internal.ModeInfo.Auto) {
				Internal.ModeInfo.Auto = 1;
				Internal.ModeInfo.ModePhase = AWCCModePhaseInitial;
			}
		}
		else if (AWCCModeControlSetManual == autoControl.modeControlState) {
			Internal.ModeInfo.Auto = 0;
			AWCC.SetMode (autoControl.mode);
			Internal.Control->ApproveModeControlState (); // TODO: Don't forget about system logger
		}

		if (AWCCFanControlSetAuto == autoControl.cpuControlState) {
			if (0 == Internal.BoostInfos [AWCCFanCPU].Auto) {
				Internal.BoostInfos [AWCCFanCPU].Auto = 1;
				Internal.BoostInfos [AWCCFanCPU].BoostPhase = AWCCBoostPhaseInitial;
			}
		}
		else if (AWCCFanControlSetManual == autoControl.cpuControlState) {
			Internal.BoostInfos [AWCCFanCPU].Auto = 0;
			AWCC.SetCpuBoost (autoControl.cpuBoost);
			Internal.Control->ApproveCpuControlState (); // TODO: Don't forget about system logger
		}

		if (AWCCFanControlSetAuto == autoControl.gpuControlState) {
			if (0 == Internal.BoostInfos [AWCCFanGPU].Auto) {
				Internal.BoostInfos [AWCCFanGPU].Auto = 1;
				Internal.BoostInfos [AWCCFanGPU].BoostPhase = AWCCBoostPhaseInitial;
			}
		}
		else if (AWCCFanControlSetManual == autoControl.gpuControlState) {
			Internal.BoostInfos [AWCCFanGPU].Auto = 0;
			AWCC.SetGpuBoost (autoControl.gpuBoost);
			Internal.Control->ApproveGpuControlState (); // TODO: Don't forget about system logger
		}
	}
}

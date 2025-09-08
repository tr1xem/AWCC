# include "AWCCAutoBoost.h"

# include <string.h>
# include <sys/stat.h>
# include <threads.h>
# include <time.h>

# include "AWCC.h"
# include "AWCCControl.h"
# include "AWCCUtils.h"

# ifdef __STDC_NO_THREADS__
# error this tool currently depends on threads.h
# endif // __STDC_NO_THREADS__

static void Start (const struct AWCCConfig_t *, const struct AWCCConfig_t *, const struct AWCCSystemLogger_t *, const struct AWCCControl_t *);

const struct AWCCAutoBoost_t AWCCAutoBoost = {
	.Start = & Start,
};

enum AWCCBoostPhase_t {
	AWCCBoostPhaseInitial,
	AWCCBoostPhaseUpShift,
	AWCCBoostPhaseNormal,
	AWCCBoostPhaseShiftToLower,
	AWCCBoostPhaseHelping,
};

static void ManageSuperBoost (void);
static void ManageFanBoost (enum AWCCFan_t);
static void ManageMode (void);
static void SetFanBoost (enum AWCCFan_t, int, enum AWCCBoostPhase_t);
static void SetMode (int);
static void HandleControl (void);
static void ResetBoostInfo (enum AWCCFan_t);
static void ResetModeInfo (void);

struct {
	const struct AWCCConfig_t * Config;
	const struct AWCCConfig_t * Configs [2];
	const struct AWCCSystemLogger_t * SystemLogger;
	const struct AWCCControl_t * Control;
	const char * const * BoostPhaseNames;

	time_t CurrentTime;
	struct {
		_Bool Auto;
		AWCCTemperature_t Temperature;
		int BoostIntervalCurrent;
		int BoostIntervalToSet;
		enum AWCCBoostPhase_t BoostPhase;
		time_t BoostSetTime;
		// time_t LastTimeInCurrentTemperatureInterval;
		time_t UpShiftDownTime;
		time_t ShiftToLowerTime;
		enum {
			AWCCBoostPendingNone,
			AWCCBoostPendingUp,
			AWCCBoostPendingDown,
		} BoostPendingState;
		time_t BoostPendingTime;
		AWCCBoost_t Boost;
		_Bool PendingHysteresis;
		time_t PendingHysteresisTime;
		int MaxBoost;
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
	void (* ManageSuperBoost) (void);
	void (* ManageFanBoost) (enum AWCCFan_t fan);
	void (* ManageMode) (void);
	void (* SetFanBoost) (enum AWCCFan_t, int, enum AWCCBoostPhase_t);
	void (* SetMode) (int);
	void (* HandleControl) (void);
	void (* ResetBoostInfo) (enum AWCCFan_t fan);
	void (* ResetModeInfo) (void);
} static Internal = {
	.Config = NULL,
	.Configs = { NULL },
	.SystemLogger = NULL,
	.Control = NULL,

	.BoostPhaseNames = (const char * []) {
		[AWCCBoostPhaseNormal] = "Normal",
		[AWCCBoostPhaseHelping] = "Helping",
		[AWCCBoostPhaseInitial] = "Initial",
		[AWCCBoostPhaseShiftToLower] = "ShiftToLower",
		[AWCCBoostPhaseUpShift] = "UpShift",
	},

	.BoostInfos = {
		[AWCCFanCPU] = {
			.Auto = 1,
			.BoostPhase = AWCCBoostPhaseInitial,
			.BoostIntervalCurrent = -1,
			.Boost = 0,
		},
		[AWCCFanGPU] = {
			.Auto = 1,
			.BoostPhase = AWCCBoostPhaseInitial,
			.BoostIntervalCurrent = -1,
			.Boost = 0,
		},
	},
	.ModeInfo = {
		.Auto = 1,
		.Mode = AWCCModeQuiet,
		.ModeInterval = -1,
		.ModePhase = AWCCModePhaseInitial,
	},
	.ManageSuperBoost = & ManageSuperBoost,
	.ManageFanBoost = & ManageFanBoost,
	.ManageMode = & ManageMode,
	.SetFanBoost = & SetFanBoost,
	.SetMode = & SetMode,
	.HandleControl = & HandleControl,
	.ResetBoostInfo = & ResetBoostInfo,
	.ResetModeInfo = & ResetModeInfo,
};

void Start (const struct AWCCConfig_t * config_ac, const struct AWCCConfig_t * config_bat, const struct AWCCSystemLogger_t * systemLogger, const struct AWCCControl_t * control)
{
	Internal.SystemLogger = systemLogger;
	Internal.Control = control;

	Internal.Configs [AWCCPowerStateAC] = config_ac;
	Internal.Configs [AWCCPowerStateBAT] = config_bat;
	Internal.PowerState = AWCC.PowerState ();

	Internal.Config = Internal.Configs [Internal.PowerState];
	Internal.BoostInfos [AWCCFanCPU].MaxBoost = Internal.Config->FanConfigs [AWCCFanCPU]._BoostIntervalCount - 1;
	Internal.BoostInfos [AWCCFanGPU].MaxBoost = Internal.Config->FanConfigs [AWCCFanGPU]._BoostIntervalCount - 1;

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
			Internal.ResetBoostInfo (AWCCFanCPU);
			Internal.ResetBoostInfo (AWCCFanGPU);
			Internal.ModeInfo.ModePhase = AWCCModePhaseInitial;
			Internal.ModeInfo.ModeInterval = -1;
			Internal.PowerState = powerState;
		}

		Internal.CurrentTime = time (NULL);

		Internal.HandleControl ();

		Internal.ManageMode ();

		enum AWCCFan_t fans [2] = {AWCCFanCPU, AWCCFanGPU};

# ifdef ENABLE_LOGS
		const char * time_str = ctime (& Internal.CurrentTime);

		printf (
			"[%.*s] %s: %d[%d] %s, %s: %d[%d] %s\n",
			(int) strlen (time_str) - 1,
			time_str,
			AWCC.GetFanName (AWCCFanCPU),
			AWCC.GetFanBoost (AWCCFanCPU),
			AWCC.GetFanTemperature (AWCCFanCPU),
			Internal.BoostPhaseNames [Internal.BoostInfos [AWCCFanCPU].BoostPhase],
			AWCC.GetFanName (AWCCFanGPU),
			AWCC.GetFanBoost (AWCCFanGPU),
			AWCC.GetFanTemperature (AWCCFanGPU),
			Internal.BoostPhaseNames [Internal.BoostInfos [AWCCFanGPU].BoostPhase]
		);
# endif // ENABLE_LOGS

		if (AWCCModeG != Internal.ModeInfo.Mode) {
			Internal.ManageSuperBoost ();
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

void ManageSuperBoost (void)
{
	enum AWCCFan_t fans [2] = {AWCCFanCPU, AWCCFanGPU};

	struct {
		int boostIntervalByTemperature;
	} fanInfos [2];


	for (int j = 0; j < 2; j++) {
		enum AWCCFan_t fan = fans [j];

		for (int i = 0; i < Internal.Config->FanConfigs [fan]._BoostIntervalCount; i++) {
			if (
				Internal.Config->FanConfigs [fan].BoostIntervals [i].TemperatureRange.Min <= Internal.BoostInfos [fan].Temperature  &&
				Internal.Config->FanConfigs [fan].BoostIntervals [i].TemperatureRange.Max >= Internal.BoostInfos [fan].Temperature
			) {
				fanInfos [fan].boostIntervalByTemperature = i;
				break;
			}
		}
	}

	int maxBoostInterval = AWCCUtils_MaxInt (
		  fanInfos [AWCCFanCPU].boostIntervalByTemperature
		, fanInfos [AWCCFanCPU].boostIntervalByTemperature
	);

	int minBoostIntervalToSet = AWCCUtils_MinInt (
		  Internal.Config->SuperBoostConfig.BoostEqualizationZoneMax
		, maxBoostInterval
	);

	for (int j = 0; j < 2; j++) {
		enum AWCCFan_t fan = fans [j];

		if (
			fanInfos [fan].boostIntervalByTemperature < minBoostIntervalToSet
		) {
			Internal.BoostInfos [fan].BoostPhase = AWCCBoostPhaseHelping;
			Internal.BoostInfos [fan].BoostIntervalToSet = minBoostIntervalToSet;
		}
		else {
			if (AWCCBoostPhaseHelping == Internal.BoostInfos [fan].BoostPhase) {
				Internal.BoostInfos [fan].BoostPhase = AWCCBoostPhaseNormal;
				Internal.BoostInfos [fan].BoostSetTime = Internal.CurrentTime;
			}

			Internal.BoostInfos [fan].BoostIntervalToSet = fanInfos [fan].boostIntervalByTemperature;
		}
	}
}

void ManageFanBoost (enum AWCCFan_t fan)
{
	if (0 == Internal.BoostInfos [fan].Auto) {
		return;
	}

	// if (Internal.BoostInfos [fan].BoostIntervalToSet == Internal.BoostInfos [fan].BoostInterval) {
	// 	Internal.BoostInfos [fan].LastTimeInCurrentTemperatureInterval = currentTime;
	// }

	_Bool pending = 0;
	_Bool pendingShiftToLower = 0;

	if (AWCCBoostPhaseInitial == Internal.BoostInfos [fan].BoostPhase) {
		Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostIntervalToSet, AWCCBoostPhaseUpShift);
	}
	else if (
		AWCCBoostPhaseHelping == Internal.BoostInfos [fan].BoostPhase
	) {
		if (Internal.BoostInfos [fan].BoostIntervalCurrent != Internal.BoostInfos [fan].BoostIntervalToSet) {
			Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostIntervalToSet, AWCCBoostPhaseHelping);
		}
	}
	else if (Internal.BoostInfos [fan].BoostIntervalToSet > Internal.BoostInfos [fan].BoostIntervalCurrent) {
		pending = 1;

		if (AWCCBoostPendingUp == Internal.BoostInfos [fan].BoostPendingState) {
			if (difftime (Internal.CurrentTime, Internal.BoostInfos [fan].BoostPendingTime) >= Internal.Config->FanConfigs [fan].PendingTime) {
				Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostIntervalToSet, AWCCBoostPhaseUpShift);
			}
		}
		else {
			Internal.BoostInfos [fan].BoostPendingState = AWCCBoostPendingUp;
			Internal.BoostInfos [fan].BoostPendingTime = Internal.CurrentTime;
		}
	}
	else if (
		   AWCCBoostPhaseShiftToLower == Internal.BoostInfos [fan].BoostPhase
		&& difftime (Internal.CurrentTime, Internal.BoostInfos [fan].ShiftToLowerTime) > Internal.Config->SuperBoostConfig.ShiftToLower [fan].Time
	) {
		// FIXME: avoid this big jump
		Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostIntervalToSet, AWCCBoostPhaseNormal);
	}
	else if (
		   AWCCBoostPhaseUpShift == Internal.BoostInfos [fan].BoostPhase
		&& Internal.BoostInfos [fan].BoostIntervalToSet <= Internal.BoostInfos [fan].BoostIntervalCurrent // alway true if reached this check
	) {
		if (AWCCBoostPendingNone == Internal.BoostInfos [fan].BoostPendingState) {
			if (difftime (Internal.CurrentTime, Internal.BoostInfos [fan].BoostSetTime) >= Internal.Config->FanConfigs [fan].UpBoostShiftTime) {
				Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostIntervalCurrent, AWCCBoostPhaseNormal);
			}
		}
	}
	else if (
		   AWCCBoostPhaseNormal == Internal.BoostInfos [fan].BoostPhase
		&& Internal.BoostInfos [fan].BoostIntervalToSet < Internal.BoostInfos [fan].BoostIntervalCurrent
	) {
		pending = 1;

		if (AWCCBoostPendingDown == Internal.BoostInfos [fan].BoostPendingState) {
			if (difftime (Internal.CurrentTime, Internal.BoostInfos [fan].BoostPendingTime) >= Internal.Config->FanConfigs [fan].PendingTime) {
				if (
					  // difftime (currentTime, Internal.BoostInfos [fan].LastTimeInCurrentTemperatureInterval)
					   difftime (Internal.CurrentTime, Internal.BoostInfos [fan].BoostSetTime)
					>= Internal.Config->FanConfigs [fan].MinTimeBeforeBoostDown / (float) (Internal.BoostInfos [fan].BoostIntervalCurrent - Internal.BoostInfos [fan].BoostIntervalToSet)
				) {
					if (difftime (Internal.CurrentTime, Internal.BoostInfos [fan].UpShiftDownTime) >= Internal.Config->FanConfigs [fan].MinTimeAfterShiftDown) {
						if (
							   Internal.BoostInfos [fan].Temperature
							<= Internal.Config->FanConfigs [fan].BoostIntervals [Internal.BoostInfos [fan].BoostIntervalCurrent].TemperatureRange.Min - Internal.Config->FanConfigs [fan].BoostDownHysteresis
						) {
							Internal.SetFanBoost (fan, Internal.BoostInfos [fan].BoostIntervalCurrent - 1, AWCCBoostPhaseNormal);
						}
						else {
							if (1 == Internal.BoostInfos [fan].PendingHysteresis) {
								if (
									   difftime (Internal.CurrentTime, Internal.BoostInfos [fan].PendingHysteresisTime)
									>= Internal.Config->SuperBoostConfig.ShiftToLower [fan].PendingTime
								) {
									Internal.SetFanBoost (
										  fan
										, Internal.BoostInfos [fan].BoostIntervalToSet
										, AWCCBoostPhaseShiftToLower
									);
								}
							}
							else {
								pendingShiftToLower = 1;
							}
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

	if (1 == pendingShiftToLower && 0 == Internal.BoostInfos [fan].PendingHysteresis) {
		Internal.BoostInfos [fan].PendingHysteresis = 1;
		Internal.BoostInfos [fan].PendingHysteresisTime = Internal.CurrentTime;
	}
	else if (0 == pendingShiftToLower && 1 == Internal.BoostInfos [fan].PendingHysteresis) {
		Internal.BoostInfos [fan].PendingHysteresis = 0;
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

void SetFanBoost (enum AWCCFan_t fan, int boostInterval, enum AWCCBoostPhase_t boostPhase)
{
	AWCCBoost_t boost = Internal.Config->FanConfigs [fan].BoostIntervals [boostInterval].Boost;

	if (AWCCFanBoostMin >= boost) {
		boost = AWCCFanBoostMin;
	}
	else if (AWCCFanBoostMax <= boost) {
		boost = AWCCFanBoostMax;
	}
	else {
		if (AWCCBoostPhaseUpShift == boostPhase) {
			boost += Internal.Config->FanConfigs [fan].UpBoostShift;
		}
		else if (AWCCBoostPhaseShiftToLower == boostPhase) {
			boost = Internal.Config->FanConfigs [fan].BoostIntervals [
				AWCCUtils_MinInt (Internal.BoostInfos [fan].MaxBoost, Internal.BoostInfos [fan].BoostIntervalCurrent + Internal.Config->SuperBoostConfig.ShiftToLower [fan].IntervalOffset)
			].Boost;
			Internal.BoostInfos [fan].ShiftToLowerTime = Internal.CurrentTime;
		}
		else if (AWCCBoostPhaseUpShift == Internal.BoostInfos [fan].BoostPhase) {
			Internal.BoostInfos [fan].UpShiftDownTime = Internal.CurrentTime;
		}
	}

	Internal.BoostInfos [fan].BoostPhase = boostPhase;

	AWCC.SetFanBoost (fan, boost);
	Internal.BoostInfos [fan].Boost = boost;

	// Internal.BoostInfos [fan].LastTimeInCurrentTemperatureInterval = Internal.CurrentTime;

	if (Internal.BoostInfos [fan].BoostIntervalCurrent != boostInterval) {
		Internal.BoostInfos [fan].BoostIntervalCurrent = boostInterval;
		Internal.BoostInfos [fan].BoostSetTime = Internal.CurrentTime;
	}
}

void SetMode (int modeInterval)
{
	enum AWCCMode_t mode = Internal.Config->ModeIntervals [modeInterval].Mode;

	AWCC.SetMode (mode);

	if (AWCCModeG != mode) {
		AWCC.SetCpuBoost (Internal.BoostInfos [AWCCFanCPU].Boost);
		AWCC.SetGpuBoost (Internal.BoostInfos [AWCCFanGPU].Boost);
	}

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

void ResetBoostInfo (enum AWCCFan_t fan)
{
	Internal.BoostInfos [fan].BoostPhase = AWCCBoostPhaseInitial;
	Internal.BoostInfos [fan].BoostPendingState = AWCCBoostPendingNone;
	Internal.BoostInfos [fan].BoostIntervalCurrent = -1;
	Internal.BoostInfos [fan].BoostIntervalToSet = -1;
	Internal.BoostInfos [fan].Boost = 0;
	Internal.BoostInfos [fan].MaxBoost = Internal.Config->FanConfigs [AWCCFanCPU]._BoostIntervalCount - 1;
}

void ResetModeInfo (void)
{

}

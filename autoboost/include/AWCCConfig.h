# ifndef __AWCCCONFIG_H
# define __AWCCCONFIG_H

# include "AWCC.h"

struct AWCCConfigRange_t {
	int Min;
	int Max;
};

struct AWCCBoostInterval_t {
	struct AWCCConfigRange_t TemperatureRange;
	AWCCBoost_t Boost;
};

struct AWCCModeInterval_t {
	struct AWCCConfigRange_t TemperatureRange;
	enum AWCCMode_t Mode;
};

struct AWCCFanConfig_t {
	int BoostDownHysteresis;
	int MinTimeBeforeBoostDown;
	struct AWCCBoostInterval_t * BoostIntervals;
	int _BoostIntervalCount;
};

struct AWCCConfig_t {
	int TemperatureCheckInterval;
	int MinTimeBeforeModeDown;
	struct AWCCFanConfig_t FanConfigs [2];
	struct AWCCModeInterval_t * ModeIntervals;
	int _ModeIntervalCount;
} extern const AWCCDefaultConfig;

# endif // __AWCCCONFIG_H

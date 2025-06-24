# ifndef __AWCCCONFIG_H
# define __AWCCCONFIG_H

# include "AWCC.h"

struct AWCCTemperatureRange_t {
	AWCCTemperature_t Min;
	AWCCTemperature_t Max;
};

struct AWCCBoostInterval_t { // A temperature range to the desired boost level
							 // mapping
	struct AWCCTemperatureRange_t TemperatureRange;
	AWCCBoost_t Boost;
};

struct AWCCModeInterval_t { // A temperature range to the desired mode mapping
	struct AWCCTemperatureRange_t TemperatureRange;
	enum AWCCMode_t Mode;
};

struct AWCCFanConfig_t { // The fan specific options
	int BoostDownHysteresis; // Hysteresis for down boosting
	int MinTimeBeforeBoostDown; // The minimum time of the current boost being
								// active before boost down
	struct AWCCBoostInterval_t * BoostIntervals;
	int _BoostIntervalCount; // Internal variable keeping mode intervals count
							 // to keep the implementation simple
};

struct AWCCConfig_t {
	int TemperatureCheckInterval; // The time interval in seconds between two
								  // consecutive temperature checks
	int MinTimeBeforeModeDown; // The minimum time before mode down happens
							   // after temperature drop to lower mode zone
	int UpBoostShift; // The amount of the additional boost after boost up
	int UpBoostShiftTime; // The time interval of being the additional boost
						  // active
	struct AWCCFanConfig_t FanConfigs [2];
	struct AWCCModeInterval_t * ModeIntervals;
	int _ModeIntervalCount; // Internal variable keeping mode intervals count
							// to keep the implementation simple
} extern const AWCCDefaultConfig;

# endif // __AWCCCONFIG_H

# ifndef __AWCC_CONFIG_H
# define __AWCC_CONFIG_H

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

struct AWCCSuperBoostShiftToLower_t {
	int PendingTime;
	int Time;
	int IntervalOffset;
};

// WARN: This makes a connection between the configured boost intervals of cpu
// and gpu
struct AWCCSuperBoostConfig_t {
	int BoostEqualizationZoneMax; // The maximum boost interval to be active to
								  // equalize the another boost interval if it's
								  // lower

	// TODO: Skeleton. When BoostDownHysteresis isn't satisfied for a certain
	// amount of time, and the only condition preventing boost down is it, shift
	// up the boost level by the offset for a certain amount of time to drop the
	// temperature and set lower boost level.
	struct AWCCSuperBoostShiftToLower_t ShiftToLower [2];
};

struct AWCCFanConfig_t { // The fan specific options
	int UpBoostShift; // The amount of the additional boost after boost up
	int UpBoostShiftTime; // The time interval of being the additional boost
						  // active
	int BoostDownHysteresis; // Hysteresis for down boosting
	int MinTimeBeforeBoostDown; // The minimum time of the current boost being
								// active before boost down
	int MinTimeAfterShiftDown; // The minimum time after shift down before boost
							   // down
	int PendingTime; // The minimum time of being in a new temperature range
					 // before switching to the corresponding boost level
	struct AWCCBoostInterval_t * BoostIntervals;
	int _BoostIntervalCount; // Internal variable keeping boost intervals count
							 // to keep the implementation simple
};

struct AWCCNoBoostConfig_t {
	enum AWCCMode_t Mode;
	AWCCBoost_t * FanBoosts;
};

struct AWCCConfig_t {
	int TemperatureCheckInterval; // The time interval in seconds between two
								  // consecutive temperature checks
	int MinTimeBeforeModeDown; // The minimum time before mode down happens
							   // after temperature drop to lower mode zone
	int ModeDownHysteresis; // Hysteresis for selecting a lower mode
	int ModePendingTime; // The minimum time of being in a new temperature range
					 // before switching to the corresponding mode
	struct AWCCSuperBoostConfig_t SuperBoostConfig;
	struct AWCCFanConfig_t FanConfigs [2];
	struct AWCCModeInterval_t * ModeIntervals;
	int _ModeIntervalCount; // Internal variable keeping mode intervals count
							// to keep the implementation simple
	struct AWCCNoBoostConfig_t NoBoostConf;
} extern const AWCCDefaultConfig;

extern const struct AWCCConfig_t AWCCDefaultConfigAC (void);
extern const struct AWCCConfig_t AWCCDefaultConfigBAT (void);

# endif // __AWCC_CONFIG_H

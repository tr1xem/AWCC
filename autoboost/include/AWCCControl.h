# ifndef __AWCC_SYSTEM_CONTROL_H
# define __AWCC_SYSTEM_CONTROL_H

# include "AWCC.h"

enum AWCCFanControl_t {
	AWCCFanControlAuto,
	AWCCFanControlManual,
};

enum AWCCModeControl_t {
	AWCCModeControlAuto,
	AWCCModeControlManual,
};

struct AWCCControl_t {
	const char * Dir;
	const char * CpuBoostFile;
	const char * GpuBoostFile;
	const char * ModeFile;

	const char * ControlAuto;

	enum AWCCFanControl_t (* GetCpuControlState) (AWCCBoost_t *);
	enum AWCCFanControl_t (* GetGpuControlState) (AWCCBoost_t *);
	enum AWCCModeControl_t (* GetModeControlState) (enum AWCCMode_t *);
} extern const AWCCControlDefault;

# endif // __AWCC_SYSTEM_CONTROL_H

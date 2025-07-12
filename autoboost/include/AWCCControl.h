# ifndef __AWCC_SYSTEM_CONTROL_H
# define __AWCC_SYSTEM_CONTROL_H

# include "AWCC.h"

enum AWCCFanControl_t {
	AWCCFanControlUnchanged,
	AWCCFanControlSetAuto,
	AWCCFanControlSetManual,
};

enum AWCCModeControl_t {
	AWCCModeControlUnchanged,
	AWCCModeControlSetAuto,
	AWCCModeControlSetManual,
};

struct AWCCControl_t {
	const char * Dir;
	const char * CpuBoostFile;
	const char * GpuBoostFile;
	const char * ModeFile;

	enum AWCCFanControl_t (* GetCpuControlState) (AWCCBoost_t *);
	enum AWCCFanControl_t (* GetGpuControlState) (AWCCBoost_t *);
	enum AWCCFanControl_t (* GetFanControlState) (enum AWCCFan_t, AWCCBoost_t *);
	enum AWCCModeControl_t (* GetModeControlState) (enum AWCCMode_t *);

	void (* ApproveCpuControlState) (void);
	void (* ApproveGpuControlState) (void);
	void (* ApproveFanControlState) (enum AWCCFan_t);
	void (* ApproveModeControlState) (void);

	void (* DiscardCpuControlState) (const char *);
	void (* DiscardGpuControlState) (const char *);
	void (* DiscardFanControlState) (enum AWCCFan_t, const char *);
	void (* DiscardModeControlState) (const char *);

	const char * (* GetFanControlName) (enum AWCCFanControl_t);
	const char * (* GetModeControlName) (enum AWCCModeControl_t);
} extern const AWCCControlDefault;

# endif // __AWCC_SYSTEM_CONTROL_H

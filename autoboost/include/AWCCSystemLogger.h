# ifndef __AWCC_SYSTEM_LOGGER_H
# define __AWCC_SYSTEM_LOGGER_H

# include "AWCC.h"
# include "AWCCControl.h"

struct AWCCSystemLogger_t {
	const char * Dir;
	const char * CpuTempFile;
	const char * GpuTempFile;
	const char * CpuBoostFile;
	const char * GpuBoostFile;
	const char * ModeFile;
	const char * CpuControlModeFile;
	const char * GpuControlModeFile;
	const char * ModeControlModeFile;

	const char * (* GetModeName) (enum AWCCMode_t);
	void (* LogCpuTemp) (AWCCTemperature_t);
	void (* LogGpuTemp) (AWCCTemperature_t);
	void (* LogCpuBoost) (AWCCBoost_t);
	void (* LogGpuBoost) (AWCCBoost_t);
	void (* LogMode) (enum AWCCMode_t);
	void (* LogCpuControlMode) (enum AWCCFanControl_t);
	void (* LogGpuControlMode) (enum AWCCFanControl_t);
	void (* LogModeControlMode) (enum AWCCFanControl_t);
} extern const AWCCSystemLoggerDefault;

# endif // __AWCC_SYSTEM_LOGGER_H

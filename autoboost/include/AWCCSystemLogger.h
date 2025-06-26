# ifndef __AWCCSYSTEM_LOGGER_H
# define __AWCCSYSTEM_LOGGER_H

# include "AWCC.h"

struct AWCCSystemLogger_t {
	const char * Dir;
	const char * CpuTempFile;
	const char * GpuTempFile;
	const char * CpuBoostFile;
	const char * GpuBoostFile;
	const char * ModeFile;

	const char * (* GetModeName) (enum AWCCMode_t);
	void (* LogCpuTemp) (AWCCTemperature_t);
	void (* LogGpuTemp) (AWCCTemperature_t);
	void (* LogCpuBoost) (AWCCBoost_t);
	void (* LogGpuBoost) (AWCCBoost_t);
	void (* LogMode) (enum AWCCMode_t);
} extern AWCCSystemLoggerDefault;

# endif // __AWCCSYSTEM_LOGGER_H

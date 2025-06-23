# ifndef __AWCC_H
# define __AWCC_H

enum AWCCMode_t {
	AWCCModeQuiet,
	AWCCModeBatterySaver,
	AWCCModeBalanced,
	AWCCModePerformance,
	AWCCModeG,
};

enum AWCCFan_t {
	AWCCFanCPU,
	AWCCFanGPU,
};

typedef int AWCCBoost_t;
typedef double AWCCTemperature_t;

struct AWCC_t {
	void (* Initialize) (void);

	AWCCBoost_t (* GetCpuBoost) (void);
	AWCCBoost_t (* GetGpuBoost) (void);

	void (* SetCpuBoost) (AWCCBoost_t);
	void (* SetGpuBoost) (AWCCBoost_t);

	AWCCTemperature_t (* GetCpuTemperature) (void);
	AWCCTemperature_t (* GetGpuTemperature) (void);

	AWCCBoost_t (* GetFanBoost) (enum AWCCFan_t);
	void (* SetFanBoost) (enum AWCCFan_t, AWCCBoost_t);
	AWCCTemperature_t (* GetFanTemperature) (enum AWCCFan_t);

	void (* SetMode) (enum AWCCMode_t);
	enum AWCCMode_t (* GetMode) (void);
	const char * (* GetModeName) (enum AWCCMode_t);

	void (* Deinitialize) (void);
} extern const AWCC;

# endif // __AWCC_H

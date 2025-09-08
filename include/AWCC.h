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
	AWCCFanCPU = 0, // NOTE: these values are important to keep as is
	AWCCFanGPU = 1,
};

enum {
	AWCCFanBoostMin = 0, AWCCFanBoostMax = 100
};

enum AWCCPowerState_t {
	AWCCPowerStateAC = 0, // NOTE: these values are important to keep as is
	AWCCPowerStateBAT = 1,
};

typedef int AWCCBoost_t;
typedef int AWCCTemperature_t;
typedef int AWCCFanRpm_t;

struct AWCC_t {
    void (*Initialize)(void);
    
    AWCCBoost_t (*GetCpuBoost)(void);
    AWCCBoost_t (*GetGpuBoost)(void);
    
    void (*SetCpuBoost)(AWCCBoost_t);
    void (*SetGpuBoost)(AWCCBoost_t);
    
    AWCCTemperature_t (*GetCpuTemperature)(void);
    AWCCTemperature_t (*GetGpuTemperature)(void);
    
    AWCCFanRpm_t (*GetCpuRpm)(void);
    AWCCFanRpm_t (*GetGpuRpm)(void);
    
    AWCCBoost_t (*GetFanBoost)(enum AWCCFan_t);
    void (*SetFanBoost)(enum AWCCFan_t, AWCCBoost_t);
    AWCCTemperature_t (*GetFanTemperature)(enum AWCCFan_t);
    AWCCFanRpm_t (*GetFanRpm)(enum AWCCFan_t);
    const char *(*GetFanName)(enum AWCCFan_t);
    
    void (*SetMode)(enum AWCCMode_t);
    enum AWCCMode_t (*GetMode)(void);
    const char *(*GetModeName)(enum AWCCMode_t);
    void (*ToggleGMode)(void);
    
    enum AWCCPowerState_t (*PowerState)(void);
    
    void (*Deinitialize)(void);
};

extern const struct AWCC_t AWCC;

# endif // __AWCC_H

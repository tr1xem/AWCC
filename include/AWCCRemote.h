#ifndef __AWCC_REMOTE_H
#define __AWCC_REMOTE_H

#include "AWCC.h"
#include "AWCCDaemon.h"

struct AWCCRemote_t {
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

extern const struct AWCCRemote_t AWCCRemote;

#endif // __AWCC_REMOTE_H
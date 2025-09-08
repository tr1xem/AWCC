#include "AWCCRemote.h"
#include "AWCCDaemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char response_buffer[AWCC_MAX_RESPONSE_DATA];

static void remote_initialize(void) {
    // No initialization needed for remote client
}

static AWCCBoost_t remote_get_cpu_boost(void) {
    struct AWCCCommand_t cmd = {AWCC_CMD_GET_FAN_BOOST, "cpu", 1};
    struct AWCCResponse_t response;
    
    if (awcc_daemon_send_command(&cmd, &response) == 0) {
        return atoi(response.data);
    }
    return 0;
}

static AWCCBoost_t remote_get_gpu_boost(void) {
    struct AWCCCommand_t cmd = {AWCC_CMD_GET_FAN_BOOST, "gpu", 1};
    struct AWCCResponse_t response;
    
    if (awcc_daemon_send_command(&cmd, &response) == 0) {
        return atoi(response.data);
    }
    return 0;
}

static void remote_set_cpu_boost(AWCCBoost_t boost) {
    struct AWCCCommand_t cmd = {AWCC_CMD_SET_FAN_BOOST, "", 0};
    snprintf(cmd.args, sizeof(cmd.args), "cpu %d", boost);
    struct AWCCResponse_t response;
    awcc_daemon_send_command(&cmd, &response);
}

static void remote_set_gpu_boost(AWCCBoost_t boost) {
    struct AWCCCommand_t cmd = {AWCC_CMD_SET_FAN_BOOST, "", 0};
    snprintf(cmd.args, sizeof(cmd.args), "gpu %d", boost);
    struct AWCCResponse_t response;
    awcc_daemon_send_command(&cmd, &response);
}

static AWCCTemperature_t remote_get_cpu_temperature(void) {
    // Temperature reading through fan interface
    return 0; // Not implemented in original AWCC interface
}

static AWCCTemperature_t remote_get_gpu_temperature(void) {
    // Temperature reading through fan interface
    return 0; // Not implemented in original AWCC interface
}

static AWCCFanRpm_t remote_get_cpu_rpm(void) {
    struct AWCCCommand_t cmd = {AWCC_CMD_GET_FAN_RPM, "cpu", 1};
    struct AWCCResponse_t response;
    
    if (awcc_daemon_send_command(&cmd, &response) == 0) {
        return atoi(response.data);
    }
    return 0;
}

static AWCCFanRpm_t remote_get_gpu_rpm(void) {
    struct AWCCCommand_t cmd = {AWCC_CMD_GET_FAN_RPM, "gpu", 1};
    struct AWCCResponse_t response;
    
    if (awcc_daemon_send_command(&cmd, &response) == 0) {
        return atoi(response.data);
    }
    return 0;
}

static AWCCBoost_t remote_get_fan_boost(enum AWCCFan_t fan) {
    struct AWCCCommand_t cmd = {AWCC_CMD_GET_FAN_BOOST, "", 1};
    snprintf(cmd.args, sizeof(cmd.args), "%s", fan == AWCCFanCPU ? "cpu" : "gpu");
    struct AWCCResponse_t response;
    
    if (awcc_daemon_send_command(&cmd, &response) == 0) {
        return atoi(response.data);
    }
    return 0;
}

static void remote_set_fan_boost(enum AWCCFan_t fan, AWCCBoost_t boost) {
    struct AWCCCommand_t cmd = {AWCC_CMD_SET_FAN_BOOST, "", 0};
    snprintf(cmd.args, sizeof(cmd.args), "%s %d", 
             fan == AWCCFanCPU ? "cpu" : "gpu", boost);
    struct AWCCResponse_t response;
    awcc_daemon_send_command(&cmd, &response);
}

static AWCCTemperature_t remote_get_fan_temperature(enum AWCCFan_t fan) {
    // Not implemented in original AWCC interface
    return 0;
}

static AWCCFanRpm_t remote_get_fan_rpm(enum AWCCFan_t fan) {
    struct AWCCCommand_t cmd = {AWCC_CMD_GET_FAN_RPM, "", 1};
    snprintf(cmd.args, sizeof(cmd.args), "%s", fan == AWCCFanCPU ? "cpu" : "gpu");
    struct AWCCResponse_t response;
    
    if (awcc_daemon_send_command(&cmd, &response) == 0) {
        return atoi(response.data);
    }
    return 0;
}

static const char *remote_get_fan_name(enum AWCCFan_t fan) {
    struct AWCCCommand_t cmd = {AWCC_CMD_GET_FAN_NAME, "", 1};
    snprintf(cmd.args, sizeof(cmd.args), "%s", fan == AWCCFanCPU ? "cpu" : "gpu");
    struct AWCCResponse_t response;
    
    if (awcc_daemon_send_command(&cmd, &response) == 0) {
        strncpy(response_buffer, response.data, sizeof(response_buffer) - 1);
        response_buffer[sizeof(response_buffer) - 1] = '\0';
        return response_buffer;
    }
    return fan == AWCCFanCPU ? "CPU Fan" : "GPU Fan";
}

static void remote_set_mode(enum AWCCMode_t mode) {
    struct AWCCCommand_t cmd = {AWCC_CMD_SET_MODE, "", 0};
    snprintf(cmd.args, sizeof(cmd.args), "%d", mode);
    struct AWCCResponse_t response;
    awcc_daemon_send_command(&cmd, &response);
}

static enum AWCCMode_t remote_get_mode(void) {
    struct AWCCCommand_t cmd = {AWCC_CMD_GET_MODE, "", 1};
    struct AWCCResponse_t response;
    
    if (awcc_daemon_send_command(&cmd, &response) == 0) {
        return (enum AWCCMode_t)atoi(response.data);
    }
    return AWCCModeBalanced;
}

static const char *remote_get_mode_name(enum AWCCMode_t mode) {
    // Use local mode name mapping to avoid daemon call
    switch (mode) {
        case AWCCModeQuiet: return "Quiet";
        case AWCCModeBatterySaver: return "Battery Saver";
        case AWCCModeBalanced: return "Balanced";
        case AWCCModePerformance: return "Performance";
        case AWCCModeG: return "G-Mode";
        default: return "Unknown";
    }
}

static void remote_toggle_gmode(void) {
    struct AWCCCommand_t cmd = {AWCC_CMD_TOGGLE_GMODE, "", 0};
    struct AWCCResponse_t response;
    awcc_daemon_send_command(&cmd, &response);
}

static enum AWCCPowerState_t remote_power_state(void) {
    // For now, assume AC power - could add daemon command later
    return AWCCPowerStateAC;
}

static void remote_deinitialize(void) {
    // No cleanup needed for remote client
}

const struct AWCCRemote_t AWCCRemote = {
    .Initialize = remote_initialize,
    .GetCpuBoost = remote_get_cpu_boost,
    .GetGpuBoost = remote_get_gpu_boost,
    .SetCpuBoost = remote_set_cpu_boost,
    .SetGpuBoost = remote_set_gpu_boost,
    .GetCpuTemperature = remote_get_cpu_temperature,
    .GetGpuTemperature = remote_get_gpu_temperature,
    .GetCpuRpm = remote_get_cpu_rpm,
    .GetGpuRpm = remote_get_gpu_rpm,
    .GetFanBoost = remote_get_fan_boost,
    .SetFanBoost = remote_set_fan_boost,
    .GetFanTemperature = remote_get_fan_temperature,
    .GetFanRpm = remote_get_fan_rpm,
    .GetFanName = remote_get_fan_name,
    .SetMode = remote_set_mode,
    .GetMode = remote_get_mode,
    .GetModeName = remote_get_mode_name,
    .ToggleGMode = remote_toggle_gmode,
    .PowerState = remote_power_state,
    .Deinitialize = remote_deinitialize
};
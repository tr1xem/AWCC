#ifndef __AWCC_DAEMON_H
#define __AWCC_DAEMON_H

#include "AWCC.h"
#include <sys/socket.h>
#include <sys/un.h>

#define AWCC_SOCKET_PATH "/var/run/awcc/awccd.sock"
#define AWCC_SOCKET_DIR "/var/run/awcc"
#define AWCC_MAX_COMMAND_ARGS 256
#define AWCC_MAX_RESPONSE_DATA 2048

enum AWCCCommandType_t {
    AWCC_CMD_GET_MODE = 1,
    AWCC_CMD_SET_MODE,
    AWCC_CMD_TOGGLE_GMODE,
    AWCC_CMD_GET_FAN_BOOST,
    AWCC_CMD_SET_FAN_BOOST,
    AWCC_CMD_GET_FAN_RPM,
    AWCC_CMD_GET_FAN_NAME,
    AWCC_CMD_AUTOBOOST,
    AWCC_CMD_LIGHTING,
    AWCC_CMD_DEVICE_INFO,
    AWCC_CMD_SHUTDOWN,
    AWCC_CMD_LIST_MODES,
    AWCC_CMD_FAN_STATUS
};

struct AWCCCommand_t {
    enum AWCCCommandType_t command_type;
    char args[AWCC_MAX_COMMAND_ARGS];
    int response_needed;
};

struct AWCCResponse_t {
    int status;
    char data[AWCC_MAX_RESPONSE_DATA];
};

int awcc_daemon_start(void);
int awcc_daemon_is_running(void);
int awcc_daemon_send_command(const struct AWCCCommand_t *cmd, struct AWCCResponse_t *response);

#endif // __AWCC_DAEMON_H

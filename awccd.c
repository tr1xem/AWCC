#include "include/AWCC.h"
#include "include/AWCCDaemon.h"
#include "include/supported_devices.h"
#include "include/lights.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/select.h>

static int daemon_running = 1;
static int server_socket = -1;

void signal_handler(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        daemon_running = 0;
        if (server_socket != -1) {
            close(server_socket);
        }
        unlink(AWCC_SOCKET_PATH);
        exit(0);
    }
}

int create_socket_dir(void) {
    struct stat st = {0};
    
    if (stat(AWCC_SOCKET_DIR, &st) == -1) {
        if (mkdir(AWCC_SOCKET_DIR, 0755) == -1) {
            fprintf(stderr, "Error creating socket directory: %s\n", strerror(errno));
            return -1;
        }
    }
    return 0;
}

void handle_command(int client_fd, const struct AWCCCommand_t *cmd) {
    struct AWCCResponse_t response = {0, ""};
    char temp_buffer[64];
    
    switch (cmd->command_type) {
        case AWCC_CMD_GET_MODE:
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "%d", AWCC.GetMode());
            break;
            
        case AWCC_CMD_SET_MODE: {
            int mode = atoi(cmd->args);
            if (mode >= AWCCModeQuiet && mode <= AWCCModeG) {
                AWCC.SetMode((enum AWCCMode_t)mode);
                response.status = 0;
                snprintf(response.data, sizeof(response.data), "Mode set to %s", 
                        AWCC.GetModeName((enum AWCCMode_t)mode));
            } else {
                response.status = -1;
                snprintf(response.data, sizeof(response.data), "Invalid mode: %d", mode);
            }
            break;
        }
        
        case AWCC_CMD_TOGGLE_GMODE:
            AWCC.ToggleGMode();
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "G-Mode toggled");
            break;
            
        case AWCC_CMD_GET_FAN_BOOST: {
            enum AWCCFan_t fan = (strcmp(cmd->args, "cpu") == 0) ? AWCCFanCPU : AWCCFanGPU;
            AWCCBoost_t boost = AWCC.GetFanBoost(fan);
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "%d", boost);
            break;
        }
        
        case AWCC_CMD_SET_FAN_BOOST: {
            char fan_str[16];
            int boost_value;
            if (sscanf(cmd->args, "%15s %d", fan_str, &boost_value) == 2) {
                enum AWCCFan_t fan = (strcmp(fan_str, "cpu") == 0) ? AWCCFanCPU : AWCCFanGPU;
                if (boost_value >= AWCCFanBoostMin && boost_value <= AWCCFanBoostMax) {
                    AWCC.SetFanBoost(fan, boost_value);
                    response.status = 0;
                    snprintf(response.data, sizeof(response.data), "%s fan boost set to %d%%", 
                            fan_str, boost_value);
                } else {
                    response.status = -1;
                    snprintf(response.data, sizeof(response.data), "Invalid boost value: %d", boost_value);
                }
            } else {
                response.status = -1;
                snprintf(response.data, sizeof(response.data), "Invalid arguments");
            }
            break;
        }
        
        case AWCC_CMD_GET_FAN_RPM: {
            enum AWCCFan_t fan = (strcmp(cmd->args, "cpu") == 0) ? AWCCFanCPU : AWCCFanGPU;
            AWCCFanRpm_t rpm = AWCC.GetFanRpm(fan);
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "%d", rpm);
            break;
        }
        
        case AWCC_CMD_GET_FAN_NAME: {
            enum AWCCFan_t fan = (strcmp(cmd->args, "cpu") == 0) ? AWCCFanCPU : AWCCFanGPU;
            const char *name = AWCC.GetFanName(fan);
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "%s", name);
            break;
        }
        
        case AWCC_CMD_FAN_STATUS:
            response.status = 0;
            snprintf(response.data, sizeof(response.data), 
                    "%s: %d RPM, %d%% boost\n%s: %d RPM, %d%% boost",
                    AWCC.GetFanName(AWCCFanCPU), AWCC.GetFanRpm(AWCCFanCPU), AWCC.GetFanBoost(AWCCFanCPU),
                    AWCC.GetFanName(AWCCFanGPU), AWCC.GetFanRpm(AWCCFanGPU), AWCC.GetFanBoost(AWCCFanGPU));
            break;
            
        case AWCC_CMD_LIST_MODES:
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "Available modes: Quiet, Battery Saver, Balanced, Performance, G-Mode");
            break;
            
        case AWCC_CMD_DEVICE_INFO:
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "Device: %s", get_device_name());
            break;
            
        case AWCC_CMD_SHUTDOWN:
            daemon_running = 0;
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "Daemon shutting down");
            break;
            
        default:
            response.status = -1;
            snprintf(response.data, sizeof(response.data), "Unknown command: %d", cmd->command_type);
            break;
    }
    
    if (cmd->response_needed) {
        if (send(client_fd, &response, sizeof(response), 0) == -1) {
            fprintf(stderr, "Error sending response: %s\n", strerror(errno));
        }
    }
}

int main(int argc, char **argv) {
    struct sockaddr_un addr;
    int client_fd;
    fd_set read_fds;
    struct timeval timeout;
    
    // Check if already running
    if (awcc_daemon_is_running()) {
        fprintf(stderr, "AWCC daemon already running\n");
        return 1;
    }
    
    // Setup signal handlers
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGPIPE, SIG_IGN);
    
    // Detect device
    device_detection_result_t detection_result = detect_device_model();
    if (detection_result == DEVICE_DETECTION_UNSUPPORTED) {
        fprintf(stderr, "Error: Unsupported device detected\n");
        return 1;
    }
    
    if (detection_result == DEVICE_DETECTION_SUCCESS) {
        printf("Device detected: %s\n", get_device_name());
    } else {
        printf("Warning: Device detection failed, using basic functionality\n");
    }
    
    // Initialize device and AWCC
    device_open();
    AWCC.Initialize();
    
    // Create socket directory
    if (create_socket_dir() != 0) {
        AWCC.Deinitialize();
        device_close();
        return 1;
    }
    
    // Create server socket
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
        AWCC.Deinitialize();
        device_close();
        return 1;
    }
    
    // Remove existing socket file
    unlink(AWCC_SOCKET_PATH);
    
    // Setup address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, AWCC_SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // Bind socket
    if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        fprintf(stderr, "Error binding socket: %s\n", strerror(errno));
        close(server_socket);
        AWCC.Deinitialize();
        device_close();
        return 1;
    }
    
    // Set socket permissions
    chmod(AWCC_SOCKET_PATH, 0666);
    
    // Listen for connections
    if (listen(server_socket, 5) == -1) {
        fprintf(stderr, "Error listening on socket: %s\n", strerror(errno));
        close(server_socket);
        unlink(AWCC_SOCKET_PATH);
        AWCC.Deinitialize();
        device_close();
        return 1;
    }
    
    printf("AWCC daemon started, listening on %s\n", AWCC_SOCKET_PATH);
    
    // Main daemon loop
    while (daemon_running) {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        int activity = select(server_socket + 1, &read_fds, NULL, NULL, &timeout);
        
        if (activity < 0 && errno != EINTR) {
            fprintf(stderr, "Select error: %s\n", strerror(errno));
            break;
        }
        
        if (activity > 0 && FD_ISSET(server_socket, &read_fds)) {
            client_fd = accept(server_socket, NULL, NULL);
            if (client_fd == -1) {
                if (errno != EINTR) {
                    fprintf(stderr, "Accept error: %s\n", strerror(errno));
                }
                continue;
            }
            
            struct AWCCCommand_t cmd;
            ssize_t bytes_received = recv(client_fd, &cmd, sizeof(cmd), 0);
            
            if (bytes_received == sizeof(cmd)) {
                handle_command(client_fd, &cmd);
            } else if (bytes_received == -1) {
                fprintf(stderr, "Receive error: %s\n", strerror(errno));
            }
            
            close(client_fd);
        }
    }
    
    // Cleanup
    close(server_socket);
    unlink(AWCC_SOCKET_PATH);
    AWCC.Deinitialize();
    device_close();
    cleanup_device_detection();
    
    printf("AWCC daemon stopped\n");
    return 0;
}
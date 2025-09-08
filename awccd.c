#include "AWCC.h"
#include "AWCCDaemon.h"
#include "supported_devices.h"
#include "lights.h"
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
#include <stdarg.h>

static int daemon_running = 1;
static int server_socket = -1;
static int verbose_mode = 0;
static int debug_mode = 0;

void print_usage(void) {
    printf("AWCC Daemon - Alienware Command Center Daemon\n");
    printf("Usage: awccd [options]\n\n");
    printf("Options:\n");
    printf("  --verbose, -v    Enable verbose output\n");
    printf("  --debug, -d      Enable debug mode (includes verbose)\n");
    printf("  --help, -h       Show this help message\n\n");
}

void log_verbose(const char *format, ...) {
    if (!verbose_mode && !debug_mode) return;
    
    va_list args;
    va_start(args, format);
    printf("[VERBOSE] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

void log_debug(const char *format, ...) {
    if (!debug_mode) return;
    
    va_list args;
    va_start(args, format);
    printf("[DEBUG] ");
    vprintf(format, args);
    printf("\n");
    va_end(args);
}

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
    
    log_debug("Processing command type: %d", cmd->command_type);
    
    switch (cmd->command_type) {
        case AWCC_CMD_GET_MODE:
            log_verbose("Getting current thermal mode");
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "%d", AWCC.GetMode());
            log_debug("Current mode: %s", response.data);
            break;
            
        case AWCC_CMD_SET_MODE: {
            int mode = atoi(cmd->args);
            log_verbose("Setting thermal mode to: %d", mode);
            if (mode >= AWCCModeQuiet && mode <= AWCCModeG) {
                AWCC.SetMode((enum AWCCMode_t)mode);
                response.status = 0;
                snprintf(response.data, sizeof(response.data), "Mode set to %s", 
                        AWCC.GetModeName((enum AWCCMode_t)mode));
                log_debug("Mode set successfully: %s", response.data);
            } else {
                response.status = -1;
                snprintf(response.data, sizeof(response.data), "Invalid mode: %d", mode);
                log_debug("Invalid mode requested: %d", mode);
            }
            break;
        }
        
        case AWCC_CMD_TOGGLE_GMODE:
            log_verbose("Toggling G-Mode");
            AWCC.ToggleGMode();
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "G-Mode toggled");
            log_debug("G-Mode toggled successfully");
            break;
            
        case AWCC_CMD_GET_FAN_BOOST: {
            enum AWCCFan_t fan = (strcmp(cmd->args, "cpu") == 0) ? AWCCFanCPU : AWCCFanGPU;
            log_verbose("Getting %s fan boost", cmd->args);
            AWCCBoost_t boost = AWCC.GetFanBoost(fan);
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "%d", boost);
            log_debug("%s fan boost: %d%%", cmd->args, boost);
            break;
        }
        
        case AWCC_CMD_SET_FAN_BOOST: {
            char fan_str[16];
            int boost_value;
            if (sscanf(cmd->args, "%15s %d", fan_str, &boost_value) == 2) {
                log_verbose("Setting %s fan boost to %d%%", fan_str, boost_value);
                enum AWCCFan_t fan = (strcmp(fan_str, "cpu") == 0) ? AWCCFanCPU : AWCCFanGPU;
                if (boost_value >= AWCCFanBoostMin && boost_value <= AWCCFanBoostMax) {
                    AWCC.SetFanBoost(fan, boost_value);
                    response.status = 0;
                    snprintf(response.data, sizeof(response.data), "%s fan boost set to %d%%", 
                            fan_str, boost_value);
                    log_debug("Fan boost set successfully: %s", response.data);
                } else {
                    response.status = -1;
                    snprintf(response.data, sizeof(response.data), "Invalid boost value: %d", boost_value);
                    log_debug("Invalid boost value: %d (valid range: %d-%d)", boost_value, AWCCFanBoostMin, AWCCFanBoostMax);
                }
            } else {
                response.status = -1;
                snprintf(response.data, sizeof(response.data), "Invalid arguments");
                log_debug("Invalid arguments for set fan boost: '%s'", cmd->args);
            }
            break;
        }
        
        case AWCC_CMD_GET_FAN_RPM: {
            enum AWCCFan_t fan = (strcmp(cmd->args, "cpu") == 0) ? AWCCFanCPU : AWCCFanGPU;
            log_verbose("Getting %s fan RPM", cmd->args);
            AWCCFanRpm_t rpm = AWCC.GetFanRpm(fan);
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "%d", rpm);
            log_debug("%s fan RPM: %d", cmd->args, rpm);
            break;
        }
        
        case AWCC_CMD_GET_FAN_NAME: {
            enum AWCCFan_t fan = (strcmp(cmd->args, "cpu") == 0) ? AWCCFanCPU : AWCCFanGPU;
            log_verbose("Getting %s fan name", cmd->args);
            const char *name = AWCC.GetFanName(fan);
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "%s", name);
            log_debug("%s fan name: %s", cmd->args, name);
            break;
        }
        
        case AWCC_CMD_FAN_STATUS:
            log_verbose("Getting fan status for all fans");
            response.status = 0;
            snprintf(response.data, sizeof(response.data), 
                    "%s: %d RPM, %d%% boost\n%s: %d RPM, %d%% boost",
                    AWCC.GetFanName(AWCCFanCPU), AWCC.GetFanRpm(AWCCFanCPU), AWCC.GetFanBoost(AWCCFanCPU),
                    AWCC.GetFanName(AWCCFanGPU), AWCC.GetFanRpm(AWCCFanGPU), AWCC.GetFanBoost(AWCCFanGPU));
            log_debug("Fan status retrieved successfully");
            break;
            
        case AWCC_CMD_LIST_MODES:
            log_verbose("Listing available thermal modes");
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "Available modes: Quiet, Battery Saver, Balanced, Performance, G-Mode");
            log_debug("Mode list provided");
            break;
            
        case AWCC_CMD_DEVICE_INFO:
            log_verbose("Getting device information");
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "Device: %s", get_device_name());
            log_debug("Device info: %s", response.data);
            break;
            
        case AWCC_CMD_SHUTDOWN:
            log_verbose("Shutdown command received");
            daemon_running = 0;
            response.status = 0;
            snprintf(response.data, sizeof(response.data), "Daemon shutting down");
            log_debug("Shutdown initiated");
            break;
            
        default:
            response.status = -1;
            snprintf(response.data, sizeof(response.data), "Unknown command: %d", cmd->command_type);
            log_debug("Unknown command received: %d", cmd->command_type);
            break;
    }
    
    if (cmd->response_needed) {
        log_debug("Sending response: status=%d, data='%s'", response.status, response.data);
        if (send(client_fd, &response, sizeof(response), 0) == -1) {
            fprintf(stderr, "Error sending response: %s\n", strerror(errno));
            log_debug("Failed to send response: %s", strerror(errno));
        } else {
            log_debug("Response sent successfully");
        }
    } else {
        log_debug("No response needed for this command");
    }
}

int main(int argc, char **argv) {
    struct sockaddr_un addr;
    int client_fd;
    fd_set read_fds;
    struct timeval timeout;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            verbose_mode = 1;
        } else if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-d") == 0) {
            debug_mode = 1;
            verbose_mode = 1; // debug includes verbose
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage();
            return 0;
        } else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            print_usage();
            return 1;
        }
    }
    
    log_debug("Debug mode enabled");
    log_verbose("Verbose mode enabled");
    
    // Check if already running
    log_debug("Checking if daemon is already running");
    if (awcc_daemon_is_running()) {
        fprintf(stderr, "AWCC daemon already running\n");
        return 1;
    }
    
    // Setup signal handlers
    log_debug("Setting up signal handlers");
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGPIPE, SIG_IGN);
    
    // Detect device
    log_verbose("Starting device detection");
    device_detection_result_t detection_result = detect_device_model();
    if (detection_result == DEVICE_DETECTION_UNSUPPORTED) {
        fprintf(stderr, "Error: Unsupported device detected\n");
        return 1;
    }
    
    if (detection_result == DEVICE_DETECTION_SUCCESS) {
        printf("Device detected: %s\n", get_device_name());
        log_debug("Device detection successful: %s", get_device_name());
    } else {
        printf("Warning: Device detection failed, using basic functionality\n");
        log_debug("Device detection failed, falling back to basic functionality");
    }
    
    // Initialize device and AWCC
    log_verbose("Initializing device and AWCC");
    device_open();
    AWCC.Initialize();
    
    // Create socket directory
    log_verbose("Creating socket directory");
    if (create_socket_dir() != 0) {
        AWCC.Deinitialize();
        device_close();
        return 1;
    }
    
    // Create server socket
    log_debug("Creating server socket");
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket == -1) {
        fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
        AWCC.Deinitialize();
        device_close();
        return 1;
    }
    
    // Remove existing socket file
    log_debug("Removing existing socket file");
    unlink(AWCC_SOCKET_PATH);
    
    // Setup address
    log_debug("Setting up socket address: %s", AWCC_SOCKET_PATH);
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, AWCC_SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // Bind socket
    log_debug("Binding socket");
    if (bind(server_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        fprintf(stderr, "Error binding socket: %s\n", strerror(errno));
        close(server_socket);
        AWCC.Deinitialize();
        device_close();
        return 1;
    }
    
    // Set socket permissions
    log_debug("Setting socket permissions to 0666");
    chmod(AWCC_SOCKET_PATH, 0666);
    
    // Listen for connections
    log_debug("Starting to listen on socket (max 5 connections)");
    if (listen(server_socket, 5) == -1) {
        fprintf(stderr, "Error listening on socket: %s\n", strerror(errno));
        close(server_socket);
        unlink(AWCC_SOCKET_PATH);
        AWCC.Deinitialize();
        device_close();
        return 1;
    }
    
    printf("AWCC daemon started, listening on %s\n", AWCC_SOCKET_PATH);
    log_verbose("Daemon startup complete, entering main loop");
    
    // Main daemon loop
    while (daemon_running) {
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        
        log_debug("Waiting for socket activity (1s timeout)");
        int activity = select(server_socket + 1, &read_fds, NULL, NULL, &timeout);
        
        if (activity < 0 && errno != EINTR) {
            fprintf(stderr, "Select error: %s\n", strerror(errno));
            break;
        }
        
        if (activity > 0 && FD_ISSET(server_socket, &read_fds)) {
            log_debug("Incoming connection detected");
            client_fd = accept(server_socket, NULL, NULL);
            if (client_fd == -1) {
                if (errno != EINTR) {
                    fprintf(stderr, "Accept error: %s\n", strerror(errno));
                }
                continue;
            }
            
            log_verbose("Client connected (fd: %d)", client_fd);
            
            struct AWCCCommand_t cmd;
            ssize_t bytes_received = recv(client_fd, &cmd, sizeof(cmd), 0);
            
            if (bytes_received == sizeof(cmd)) {
                log_debug("Received command: type=%d, args='%s', response_needed=%d", 
                         cmd.command_type, cmd.args, cmd.response_needed);
                handle_command(client_fd, &cmd);
                log_debug("Command processed");
            } else if (bytes_received == -1) {
                fprintf(stderr, "Receive error: %s\n", strerror(errno));
            } else {
                log_debug("Received incomplete command: %zd bytes (expected %zu)", 
                         bytes_received, sizeof(cmd));
            }
            
            close(client_fd);
            log_verbose("Client disconnected");
        }
    }
    
    // Cleanup
    log_verbose("Starting daemon cleanup");
    close(server_socket);
    unlink(AWCC_SOCKET_PATH);
    log_debug("Socket closed and removed");
    
    AWCC.Deinitialize();
    device_close();
    cleanup_device_detection();
    log_debug("Device and AWCC deinitialized");
    
    printf("AWCC daemon stopped\n");
    return 0;
}

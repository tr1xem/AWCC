#include "AWCCDaemon.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

int awcc_daemon_is_running(void) {
    struct stat st;
    return (stat(AWCC_SOCKET_PATH, &st) == 0);
}

int awcc_daemon_send_command(const struct AWCCCommand_t *cmd, struct AWCCResponse_t *response) {
    int sockfd;
    struct sockaddr_un addr;
    
    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        fprintf(stderr, "Error creating socket: %s\n", strerror(errno));
        return -1;
    }
    
    // Setup address
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, AWCC_SOCKET_PATH, sizeof(addr.sun_path) - 1);
    
    // Connect to daemon
    if (connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        fprintf(stderr, "Error connecting to daemon: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    }
    
    // Send command
    if (send(sockfd, cmd, sizeof(*cmd), 0) == -1) {
        fprintf(stderr, "Error sending command: %s\n", strerror(errno));
        close(sockfd);
        return -1;
    }
    
    // Receive response if needed
    if (cmd->response_needed && response) {
        if (recv(sockfd, response, sizeof(*response), 0) == -1) {
            fprintf(stderr, "Error receiving response: %s\n", strerror(errno));
            close(sockfd);
            return -1;
        }
    }
    
    close(sockfd);
    return 0;
}
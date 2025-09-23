#pragma once
#include <string>

class Daemon {
  private:
    bool m_running;
    int m_server_fd;
    std::string m_socket_path;

  public:
    Daemon(std::string socket_path = "/tmp/cmd.sock");
    ~Daemon();
    auto isDaemonRunning() -> bool;
    void init();
    auto executeFromDaemon(const char *command) -> std::string;
    void stop();
    [[nodiscard]] auto isServerMode() const -> bool { return m_running; }
};

#pragma once
#include "KeyBinder.h"
#include <string>
#include <thread>

class Daemon {
  private:
    bool m_running;
    int m_server_fd;
    std::string m_socket_path;
    std::thread m_keybinderThread;
    void m_StopBinder();
    KeyBinder *m_binder = nullptr;

  public:
    Daemon(std::string socket_path = "/tmp/cmd.sock");
    auto isDaemonRunning() -> bool;
    void init();
    auto executeFromDaemon(const char *command) -> std::string;
    void stop();
    [[nodiscard]] auto isServerMode() const -> bool { return m_running; }
    // NOTE: This is not needed as handled manually
    // ~Daemon();
};

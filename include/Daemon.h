#pragma once
#include "EffectController.h"
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
    EffectController &m_effectsController;
    // Thermals &thermals;

    short m_brightness{};
    void m_onGmodeKey();
    void m_onLightKey();
    std::function<void()> m_onGmodeKeyCallback;
    static bool m_CommandAllowed(const std::string &cmd);

  public:
    Daemon(EffectController &effectsController,
           std::string socket_path = "/tmp/awcc.sock");
    ~Daemon();
    void setOnGmodeKeyCallback(std::function<void()> cb) {
        m_onGmodeKeyCallback = std::move(cb);
    }
    bool isDaemonRunning();
    void init();
    std::string executeFromDaemon(const char *command);
    void stop();
    [[nodiscard]] bool isServerMode() const { return m_running; }
    // NOTE: This is not needed as handled manually
    // ~Daemon();
};

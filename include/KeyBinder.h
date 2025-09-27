#pragma once
#include <chrono>
#include <functional>
#include <libevdev-1.0/libevdev/libevdev.h>
#include <string>
#include <unordered_map>

class KeyBinder {
  public:
    using Callback = std::function<void()>;

    KeyBinder(const std::string &target_device_name, double timeout_sec = 0.3);
    ~KeyBinder();

    void setOnGModeKey(std::function<void()> cb);
    void setOnLightKey(std::function<void()> cb);
    void run();
    void stop();

  private:
    int fd_;
    libevdev *dev_;
    bool m_isRunning;
    double m_timeoutSec;
    std::unordered_map<int, std::chrono::steady_clock::time_point>
        m_lastTriggered;
    Callback m_onGmodeKey;
    Callback m_onLightKey;
};

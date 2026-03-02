#include "KeyBinder.h"
#include <fcntl.h>
#include <filesystem>
#include <loguru.hpp>
#include <thread>
#include <unistd.h>

KeyBinder::KeyBinder(const std::string &target_device_name, double timeout_sec)
    : fd_(-1), dev_(nullptr), m_isRunning(false), m_timeoutSec(timeout_sec) {
    LOG_S(INFO) << "Initializing KeyBinder Module";
    for (const auto &entry :
         std::filesystem::directory_iterator("/dev/input")) {
        if (entry.path().string().find("event") != std::string::npos) {
            int fd = open(entry.path().c_str(), O_RDONLY | O_NONBLOCK);
            if (fd < 0) {
                LOG_S(WARNING) << "Failed to open " << entry.path();
                continue;
            }
            libevdev *dev = nullptr;
            if (libevdev_new_from_fd(fd, &dev) == 0) {
                if (target_device_name == libevdev_get_name(dev)) {
                    fd_ = fd;
                    dev_ = dev;
                    LOG_S(INFO) << "Found device: " << entry.path() << " ("
                                << target_device_name << ")";
                    break;
                }
                libevdev_free(dev);
            }
            close(fd);
        }
    }
    if (dev_ == nullptr) {
        LOG_S(ERROR) << "Device " << target_device_name << " not found!";
    } else {
        LOG_S(INFO) << "KeyBinder module initialized";
    }
}

KeyBinder::~KeyBinder() {
    if (dev_ != nullptr)
        libevdev_free(dev_);
    if (fd_ >= 0)
        close(fd_);
}

void KeyBinder::setOnGModeKey(Callback cb) { m_onGmodeKey = std::move(cb); }
void KeyBinder::setOnLightKey(Callback cb) { m_onLightKey = std::move(cb); }

void KeyBinder::run() {
    if (dev_ == nullptr)
        return;
    m_isRunning = true;
    struct input_event ev;
    while (m_isRunning) {
        int rc = libevdev_next_event(dev_, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0 && ev.type == EV_MSC) {
            auto now = std::chrono::steady_clock::now();
            // NOTE:For EV_MSC, ev.value is the key id
            // Also MSC key dont support active high and low
            int code = ev.value;

            auto it = m_lastTriggered.find(code);
            bool should_fire = false;
            if (it == m_lastTriggered.end() ||
                std::chrono::duration<double>(now - it->second).count() >=
                    m_timeoutSec) {
                should_fire = true;
                m_lastTriggered[code] = now;
            }

            if (should_fire) {
                // LOG_S(INFO) << "Received EV_MSC code: " << code;
                if (code == 104 && m_onGmodeKey) {
                    // LOG_S(INFO) << "Triggering G-Mode callback";
                    m_onGmodeKey();
                } else if (code == 105 && m_onLightKey) {
                    // LOG_S(INFO) << "Triggering Light key callback";
                    m_onLightKey();
                }
            }
        } else if (rc == -EAGAIN) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        } else if (rc < 0) {
            LOG_S(ERROR) << "Error or device removed (rc=" << rc << ")";
            break;
        }
    }
}

void KeyBinder::stop() { m_isRunning = false; }

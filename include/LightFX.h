#pragma once
#include <libusb.h>
#include <span>

constexpr std::array<uint8_t, 1> ZONE_LEFT = {0x00};
constexpr std::array<uint8_t, 1> ZONE_MIDDLE_LEFT = {0x01};
constexpr std::array<uint8_t, 1> ZONE_MIDDLE_RIGHT = {0x02};
constexpr std::array<uint8_t, 1> ZONE_RIGHT = {0x03};

constexpr std::array<uint8_t, 4> ZONE_ALL = {
    *ZONE_LEFT.data(), *ZONE_MIDDLE_LEFT.data(), *ZONE_MIDDLE_RIGHT.data(),
    *ZONE_RIGHT.data()};

// Interface
constexpr uint8_t PREAMBLE = 0x03;

constexpr uint8_t REQUEST = 0x20;
constexpr uint8_t REQUEST_FIRMWARE_VERSION = 0x00;
constexpr uint8_t REQUEST_STATUS = 0x01;
constexpr uint8_t REQUEST_ELC_CONFIG = 0x02;
constexpr uint8_t REQUEST_ANIMATION_COUNT = 0x03;

constexpr uint8_t ANIMATION = 0x21;
constexpr uint8_t ANIMATION_CONFIG_START = 0x0001;
constexpr uint8_t ANIMATION_CONFIG_SAVE = 0x0002;
constexpr uint8_t ANIMATION_CONFIG_PLAY = 0x0003;
constexpr uint8_t ANIMATION_REMOVE = 0x0004;
constexpr uint8_t ANIMATION_PLAY = 0x0005;
constexpr uint8_t ANIMATION_SET_DEFAULT = 0x0006;
constexpr uint8_t ANIMATION_SET_STARTUP = 0x0007;
constexpr uint8_t ZONE_SELECT = 0x23;

constexpr uint8_t ADD_ACTION = 0x24;
constexpr uint8_t ACTION_COLOR = 0x00;
constexpr uint8_t ACTION_PULSE = 0x01;
constexpr uint8_t ACTION_MORPH = 0x02;

constexpr uint8_t SET_DIM = 0x26;

class LightFX {
  private:
    libusb_context *m_libusbContext{nullptr};
    libusb_device_handle *m_deviceHandle{nullptr};
    bool m_deviceAcquired{false};

    void m_deviceSend(std::span<const uint8_t> data);
    void m_deviceReceive(std::span<uint8_t> out);
    void m_SendRequestFirmwareVersion();
    void m_SendRequestStatus();
    void m_SendRequestElcConfig();
    void m_SendRequestAnimationCount();

  public:
    LightFX();
    void deviceOpen();
    void deviceAcquire();
    void deviceClose();
    void deviceRelease();

    void SendAnimationConfigStart(uint16_t animationId);
    void SendAnimationConfigPlay(uint16_t animationId);
    void SendAnimationConfigSave(uint16_t animationId);
    void SendAnimationRemove(uint16_t animationId);
    void SendAnimationPlay(uint16_t animationId);
    void SendAnimationSetDefault(uint16_t animationId);
    void SendAnimationSetStartup(uint16_t animationId);

    void SendZoneSelect(uint8_t loop, std::span<const uint8_t> zones);
    void SendAddAction(uint16_t action, uint16_t duration, uint16_t tempo,
                       uint32_t color);
    void SendSetDim(uint8_t dim, std::span<const uint8_t> zones);
};

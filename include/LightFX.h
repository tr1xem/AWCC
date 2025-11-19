#pragma once
#include <libusb-1.0/libusb.h>
#include <span>

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

    static constexpr uint8_t m_preamble = 0x03;

    static constexpr uint8_t m_request = 0x20;
    static constexpr uint8_t m_requestFirmwareVersion = 0x00;
    static constexpr uint8_t m_requestStatus = 0x01;
    static constexpr uint8_t m_requestElcConfig = 0x02;
    static constexpr uint8_t m_requestAnimationCount = 0x03;

    static constexpr uint8_t m_animation = 0x21;
    static constexpr uint16_t m_animationConfigStart = 0x0001;
    static constexpr uint16_t m_animationConfigSave = 0x0002;
    static constexpr uint16_t m_animationConfigPlay = 0x0003;
    static constexpr uint16_t m_animationRemove = 0x0004;
    static constexpr uint16_t m_animationPlay = 0x0005;
    static constexpr uint16_t m_animationSetDefault = 0x0006;
    static constexpr uint16_t m_animationSetStartup = 0x0007;
    static constexpr uint8_t m_zoneSelect = 0x23;

    static constexpr uint8_t m_addAction = 0x24;
    static constexpr uint8_t m_actionColor = 0x00;
    static constexpr uint8_t m_actionPulse = 0x01;
    static constexpr uint8_t m_actionMorph = 0x02;

    static constexpr uint8_t m_setDim = 0x26;

  public:
    LightFX();
    ~LightFX();
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

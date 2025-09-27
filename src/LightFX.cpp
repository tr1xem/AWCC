#include "LightFX.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <loguru.hpp>
#include <vector>

LightFX::LightFX() { LOG_S(INFO) << "LightFX Module initialized"; }

void LightFX::deviceOpen() {
    if (m_libusbContext != nullptr) {
        LOG_S(ERROR) << "LibUsb already initialized";
        std::exit(1);
    }
    if (m_deviceHandle != nullptr) {
        LOG_S(ERROR) << "Device already opened";
        std::exit(1);
    }
    libusb_init(&m_libusbContext);
    libusb_set_option(m_libusbContext, LIBUSB_OPTION_LOG_LEVEL,
                      LIBUSB_LOG_LEVEL_WARNING);
    libusb_device **devices;
    libusb_device *device = nullptr;

    ssize_t count = libusb_get_device_list(m_libusbContext, &devices);
    if (count < 0) {
        LOG_S(ERROR) << "Failed To Get Device List";
        std::exit(1);
    }
    for (ssize_t i = 0; i < count; i++) {
        struct libusb_device_descriptor descriptor = {.bLength = 0};
        if (libusb_get_device_descriptor(devices[i], &descriptor) != 0) {
            LOG_S(ERROR) << ("Failed to get device descriptor");
            std::exit(1);
        }
        if (descriptor.idVendor == 0x187c && (descriptor.idProduct == 0x0551 ||
                                              descriptor.idProduct == 0x0550)) {
            device = devices[i];
            LOG_S(INFO) << "Device acquired successfully: " << std::hex
                        << descriptor.idVendor << std::dec << ":" << std::hex
                        << descriptor.idProduct << std::dec;
            break;
        }
    }
    libusb_free_device_list(devices, 1);
    if (device == nullptr) {
        LOG_S(ERROR) << "Failed to find device";
        std::exit(1);
    }

    if (libusb_open(device, &m_deviceHandle) != 0) {
        LOG_S(ERROR) << "Failed to open device";
        std::exit(1);
    }
};

void LightFX::deviceAcquire() {
    if (m_deviceHandle == nullptr) {
        LOG_S(ERROR) << ("device not opened");
        std::exit(1);
    }
    if (m_deviceAcquired)
        return;
    if (libusb_kernel_driver_active(m_deviceHandle, 0) != 0)
        if (libusb_detach_kernel_driver(m_deviceHandle, 0) != 0) {
            LOG_S(ERROR) << "Please detach kernel driver";
            std::exit(1);
        }
    if (libusb_claim_interface(m_deviceHandle, 0) != 0) {
        LOG_S(ERROR) << "Cannot claim interface";
        std::exit(1);
    }
    m_deviceAcquired = true;
}

void LightFX::deviceRelease() {
    if (m_deviceHandle == nullptr) {
        LOG_S(ERROR) << "Device not opened";
        std::exit(1);
    }
    if (!m_deviceAcquired)
        return;
    libusb_release_interface(m_deviceHandle, 0);
    libusb_attach_kernel_driver(m_deviceHandle, 0);
    m_deviceAcquired = false;
    LOG_S(INFO) << "Device released";
}

void LightFX::deviceClose() {
    if (m_deviceHandle == nullptr) {
        LOG_S(ERROR) << "Device not opened";
        std::exit(1);
    }
    if (m_deviceAcquired)
        deviceRelease();
    libusb_close(m_deviceHandle);
    m_deviceHandle = nullptr;
    libusb_exit(m_libusbContext);
    m_libusbContext = nullptr;
}

void LightFX::m_deviceSend(std::span<const uint8_t> data) {
    if (!m_deviceAcquired)
        LOG_S(ERROR) << "Device not Acquired";

    std::array<uint8_t, 33> buffer{};
    // Copy up to 33 bytes from data into buffer
    std::memcpy(buffer.data(), data.data(),
                std::min<size_t>(data.size(), buffer.size()));

    if (libusb_control_transfer(
            m_deviceHandle, 0x21, 9, 0x202, 0,
            reinterpret_cast<unsigned char *>(buffer.data()),
            static_cast<uint16_t>(buffer.size()), 0) != 33) {
        LOG_S(ERROR) << "Couldn't write full packet";
        std::exit(1);
    }
}

void LightFX::m_deviceReceive(std::span<uint8_t> out) {
    if (!m_deviceAcquired) {
        LOG_S(ERROR) << "Device not Acquired";
        std::exit(1);
    }

    std::array<uint8_t, 33> buffer{};
    if (libusb_control_transfer(
            m_deviceHandle, 0xA1, 1, 0x101, 0,
            reinterpret_cast<unsigned char *>(buffer.data()),
            static_cast<uint16_t>(buffer.size()), 0) != 33) {
        LOG_S(ERROR) << ("couldn't read full packet");
        std::exit(1);
    }

    std::memcpy(out.data(), buffer.data(),
                std::min<size_t>(out.size(), buffer.size()));
}

void LightFX::m_SendRequestFirmwareVersion() {
    constexpr std::array<uint8_t, 3> packet{m_preamble, m_request,
                                            m_requestFirmwareVersion};
    m_deviceSend(std::span(packet));
}

void LightFX::m_SendRequestStatus() {
    constexpr std::array<uint8_t, 3> packet{m_preamble, m_request,
                                            m_requestStatus};
    m_deviceSend(std::span(packet));
}

void LightFX::m_SendRequestElcConfig() {
    constexpr std::array<uint8_t, 3> packet{m_preamble, m_request,
                                            m_requestElcConfig};
    m_deviceSend(std::span(packet));
}

void LightFX::m_SendRequestAnimationCount() {
    constexpr std::array<uint8_t, 3> packet{m_preamble, m_request,
                                            m_requestAnimationCount};
    m_deviceSend(std::span(packet));
}

void LightFX::SendAnimationConfigStart(uint16_t animationId) {
    std::array<uint8_t, 6> packet{
        m_preamble,
        m_animation,
        static_cast<uint8_t>((m_animationConfigStart >> 8) & 0xFF),
        static_cast<uint8_t>(m_animationConfigStart & 0xFF),
        static_cast<uint8_t>((animationId >> 8) & 0xFF),
        static_cast<uint8_t>(animationId & 0xFF),
    };
    m_deviceSend(std::span(packet));
}

void LightFX::SendAnimationConfigPlay(uint16_t animationId) {
    std::array<uint8_t, 6> packet{
        m_preamble,
        m_animation,
        static_cast<uint8_t>((m_animationConfigPlay >> 8) & 0xFF),
        static_cast<uint8_t>(m_animationConfigPlay & 0xFF),
        static_cast<uint8_t>((animationId >> 8) & 0xFF),
        static_cast<uint8_t>(animationId & 0xFF),
    };
    m_deviceSend(std::span(packet));
}

void LightFX::SendAnimationConfigSave(uint16_t animationId) {
    std::array<uint8_t, 6> packet{
        m_preamble,
        m_animation,
        static_cast<uint8_t>((m_animationConfigSave >> 8) & 0xFF),
        static_cast<uint8_t>(m_animationConfigSave & 0xFF),
        static_cast<uint8_t>((animationId >> 8) & 0xFF),
        static_cast<uint8_t>(animationId & 0xFF),
    };
    m_deviceSend(std::span(packet));
}

void LightFX::SendAnimationRemove(uint16_t animationId) {
    std::array<uint8_t, 6> packet{
        m_preamble,
        m_animation,
        static_cast<uint8_t>((m_animationRemove >> 8) & 0xFF),
        static_cast<uint8_t>(m_animationRemove & 0xFF),
        static_cast<uint8_t>((animationId >> 8) & 0xFF),
        static_cast<uint8_t>(animationId & 0xFF),
    };
    m_deviceSend(std::span(packet));
}

void LightFX::SendAnimationPlay(uint16_t animationId) {
    std::array<uint8_t, 6> packet{
        m_preamble,
        m_animation,
        static_cast<uint8_t>((m_animationPlay >> 8) & 0xFF),
        static_cast<uint8_t>(m_animationPlay & 0xFF),
        static_cast<uint8_t>((animationId >> 8) & 0xFF),
        static_cast<uint8_t>(animationId & 0xFF),
    };
    m_deviceSend(std::span(packet));
}

void LightFX::SendAnimationSetDefault(uint16_t animationId) {
    std::array<uint8_t, 6> packet{
        m_preamble,
        m_animation,
        static_cast<uint8_t>((m_animationSetDefault >> 8) & 0xFF),
        static_cast<uint8_t>(m_animationSetDefault & 0xFF),
        static_cast<uint8_t>((animationId >> 8) & 0xFF),
        static_cast<uint8_t>(animationId & 0xFF),
    };
    m_deviceSend(std::span(packet));
}

void LightFX::SendAnimationSetStartup(uint16_t animationId) {
    std::array<uint8_t, 6> packet{
        m_preamble,
        m_animation,
        static_cast<uint8_t>((m_animationSetStartup >> 8) & 0xFF),
        static_cast<uint8_t>(m_animationSetStartup & 0xFF),
        static_cast<uint8_t>((animationId >> 8) & 0xFF),
        static_cast<uint8_t>(animationId & 0xFF),
    };
    m_deviceSend(std::span(packet));
}

// For zone lists, use std::vector since the size is dynamic
void LightFX::SendZoneSelect(uint8_t loop, std::span<const uint8_t> zones) {
    const size_t zoneCount = zones.size();
    std::vector<uint8_t> packet(5 + zoneCount);
    packet[0] = m_preamble;
    packet[1] = m_zoneSelect;
    packet[2] = loop;
    packet[3] = static_cast<uint8_t>((zoneCount >> 8) & 0xFF);
    packet[4] = static_cast<uint8_t>(zoneCount & 0xFF);
    std::ranges::copy(zones, packet.begin() + 5);
    m_deviceSend(std::span(packet));
}

void LightFX::SendAddAction(uint16_t action, uint16_t duration, uint16_t tempo,
                            uint32_t color) {
    std::array<uint8_t, 10> packet{
        m_preamble,
        m_addAction,
        static_cast<uint8_t>(action),
        static_cast<uint8_t>((duration >> 8) & 0xFF),
        static_cast<uint8_t>(duration & 0xFF),
        static_cast<uint8_t>((tempo >> 8) & 0xFF),
        static_cast<uint8_t>(tempo & 0xFF),
        static_cast<uint8_t>((color >> 16) & 0xFF),
        static_cast<uint8_t>((color >> 8) & 0xFF),
        static_cast<uint8_t>(color & 0xFF),
    };
    m_deviceSend(std::span(packet));
}

void LightFX::SendSetDim(uint8_t dim, std::span<const uint8_t> zones) {
    const size_t zoneCount = zones.size();
    std::vector<uint8_t> packet(5 + zoneCount);
    packet[0] = m_preamble;
    packet[1] = m_setDim;
    packet[2] = dim;
    packet[3] = static_cast<uint8_t>((zoneCount >> 8) & 0xFF);
    packet[4] = static_cast<uint8_t>(zoneCount & 0xFF);
    std::ranges::copy(zones, packet.begin() + 5);
    m_deviceSend(std::span(packet));
}

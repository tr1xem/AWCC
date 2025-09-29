#include "LightFX.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <loguru.hpp>
#include <vector>

LightFX::LightFX() { LOG_S(INFO) << "LightFX Module initialized"; }

void LightFX::deviceOpen() {
    if (m_libusbContext != nullptr) {
        LOG_S(ERROR) << "LibUsb already initialized";
        return;
    }
    if (m_deviceHandle != nullptr) {
        LOG_S(ERROR) << "Device already opened";
        return;
    }
    libusb_init(&m_libusbContext);
    libusb_set_option(m_libusbContext, LIBUSB_OPTION_LOG_LEVEL,
                      LIBUSB_LOG_LEVEL_WARNING);
    libusb_device **devices;
    libusb_device *device = nullptr;

    ssize_t count = libusb_get_device_list(m_libusbContext, &devices);
    if (count < 0) {
        LOG_S(ERROR) << "Failed To Get Device List";
        return;
    }
    for (ssize_t i = 0; i < count; i++) {
        struct libusb_device_descriptor descriptor = {.bLength = 0};
        if (libusb_get_device_descriptor(devices[i], &descriptor) != 0) {
            LOG_S(ERROR) << ("Failed to get device descriptor");
            return;
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
        return;
    }

    if (libusb_open(device, &m_deviceHandle) != 0) {
        LOG_S(ERROR) << "Failed to open device";
        return;
    }
};

void LightFX::deviceAcquire() {
    if (m_deviceHandle == nullptr) {
        LOG_S(ERROR) << ("device not opened");
        return;
    }
    if (m_deviceAcquired)
        return;
    if (libusb_kernel_driver_active(m_deviceHandle, 0) != 0)
        if (libusb_detach_kernel_driver(m_deviceHandle, 0) != 0) {
            LOG_S(ERROR) << "Please detach kernel driver";
            return;
        }
    if (libusb_claim_interface(m_deviceHandle, 0) != 0) {
        LOG_S(ERROR) << "Cannot claim interface";
        return;
    }
    m_deviceAcquired = true;
    // LOG_S(INFO) << "Device Acquired";
}

void LightFX::deviceRelease() {
    if (m_deviceHandle == nullptr) {
        LOG_S(ERROR) << "Device not opened";
        return;
    }
    if (!m_deviceAcquired)
        return;
    libusb_release_interface(m_deviceHandle, 0);
    libusb_attach_kernel_driver(m_deviceHandle, 0);
    m_deviceAcquired = false;
    // LOG_S(INFO) << "Device released";
}

void LightFX::deviceClose() {
    if (m_deviceHandle == nullptr) {
        LOG_S(ERROR) << "Device not opened";
        return;
    }
    if (m_deviceAcquired)
        deviceRelease();
    libusb_close(m_deviceHandle);
    m_deviceHandle = nullptr;
    libusb_exit(m_libusbContext);
    m_libusbContext = nullptr;
}

void LightFX::m_deviceSend(std::span<const uint8_t> data) {
    if (!m_deviceAcquired) {
        LOG_S(ERROR) << "Device not Acquired";
        return;
    }

    unsigned char buffer[33];
    std::memset(buffer, 0, sizeof(buffer));
    size_t len = std::min<size_t>(data.size(), 33);
    std::memcpy(buffer, data.data(), len);

    int ret = libusb_control_transfer(m_deviceHandle, 0x21, 9, 0x202, 0, buffer,
                                      33, 0);
    if (ret != 33) {
        if (ret < 0) {
            LOG_S(ERROR) << "Couldn't write full packet: "
                            "libusb_control_transfer returned "
                         << libusb_error_name(ret) << " (" << ret << ")";
        } else {
            LOG_S(ERROR) << "Couldn't write full packet, only wrote " << ret
                         << " bytes";
        }
    }
}

void LightFX::m_deviceReceive(std::span<uint8_t> out) {
    if (!m_deviceAcquired) {
        LOG_S(ERROR) << "Device not Acquired";
        return;
    }

    unsigned char buffer[33];
    int ret = libusb_control_transfer(m_deviceHandle, 0xA1, 1, 0x101, 0, buffer,
                                      33, 0);
    if (ret != 33) {
        if (ret < 0) {
            LOG_S(ERROR) << "Couldn't read full packet: "
                            "libusb_control_transfer returned "
                         << libusb_error_name(ret) << " (" << ret << ")";
        } else {
            LOG_S(ERROR) << "Couldn't read full packet, only read " << ret
                         << " bytes";
        }
        return;
    }
    size_t len = std::min<size_t>(out.size(), 33);
    std::memcpy(out.data(), buffer, len);
}

void LightFX::m_SendRequestFirmwareVersion() {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_request;
    packet[2] = m_requestFirmwareVersion;
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::m_SendRequestStatus() {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_request;
    packet[2] = m_requestStatus;
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::m_SendRequestElcConfig() {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_request;
    packet[2] = m_requestElcConfig;
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::m_SendRequestAnimationCount() {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_request;
    packet[2] = m_requestAnimationCount;
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendAnimationConfigStart(uint16_t animationId) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_animation;
    packet[2] = static_cast<uint8_t>((m_animationConfigStart >> 8) & 0xFF);
    packet[3] = static_cast<uint8_t>(m_animationConfigStart & 0xFF);
    packet[4] = static_cast<uint8_t>((animationId >> 8) & 0xFF);
    packet[5] = static_cast<uint8_t>(animationId & 0xFF);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendAnimationConfigPlay(uint16_t animationId) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_animation;
    packet[2] = static_cast<uint8_t>((m_animationConfigPlay >> 8) & 0xFF);
    packet[3] = static_cast<uint8_t>(m_animationConfigPlay & 0xFF);
    packet[4] = static_cast<uint8_t>((animationId >> 8) & 0xFF);
    packet[5] = static_cast<uint8_t>(animationId & 0xFF);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendAnimationConfigSave(uint16_t animationId) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_animation;
    packet[2] = static_cast<uint8_t>((m_animationConfigSave >> 8) & 0xFF);
    packet[3] = static_cast<uint8_t>(m_animationConfigSave & 0xFF);
    packet[4] = static_cast<uint8_t>((animationId >> 8) & 0xFF);
    packet[5] = static_cast<uint8_t>(animationId & 0xFF);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendAnimationRemove(uint16_t animationId) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_animation;
    packet[2] = static_cast<uint8_t>((m_animationRemove >> 8) & 0xFF);
    packet[3] = static_cast<uint8_t>(m_animationRemove & 0xFF);
    packet[4] = static_cast<uint8_t>((animationId >> 8) & 0xFF);
    packet[5] = static_cast<uint8_t>(animationId & 0xFF);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendAnimationPlay(uint16_t animationId) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_animation;
    packet[2] = static_cast<uint8_t>((m_animationPlay >> 8) & 0xFF);
    packet[3] = static_cast<uint8_t>(m_animationPlay & 0xFF);
    packet[4] = static_cast<uint8_t>((animationId >> 8) & 0xFF);
    packet[5] = static_cast<uint8_t>(animationId & 0xFF);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendAnimationSetDefault(uint16_t animationId) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_animation;
    packet[2] = static_cast<uint8_t>((m_animationSetDefault >> 8) & 0xFF);
    packet[3] = static_cast<uint8_t>(m_animationSetDefault & 0xFF);
    packet[4] = static_cast<uint8_t>((animationId >> 8) & 0xFF);
    packet[5] = static_cast<uint8_t>(animationId & 0xFF);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendAnimationSetStartup(uint16_t animationId) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_animation;
    packet[2] = static_cast<uint8_t>((m_animationSetStartup >> 8) & 0xFF);
    packet[3] = static_cast<uint8_t>(m_animationSetStartup & 0xFF);
    packet[4] = static_cast<uint8_t>((animationId >> 8) & 0xFF);
    packet[5] = static_cast<uint8_t>(animationId & 0xFF);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendZoneSelect(uint8_t loop, std::span<const uint8_t> zones) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_zoneSelect;
    packet[2] = loop;
    size_t zoneCount = std::min(zones.size(), size_t(28)); // 5+28=33
    packet[3] = static_cast<uint8_t>((zoneCount >> 8) & 0xFF);
    packet[4] = static_cast<uint8_t>(zoneCount & 0xFF);
    std::memcpy(packet + 5, zones.data(), zoneCount);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendAddAction(uint16_t action, uint16_t duration, uint16_t tempo,
                            uint32_t color) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_addAction;
    packet[2] = static_cast<uint8_t>(action);
    packet[3] = static_cast<uint8_t>((duration >> 8) & 0xFF);
    packet[4] = static_cast<uint8_t>(duration & 0xFF);
    packet[5] = static_cast<uint8_t>((tempo >> 8) & 0xFF);
    packet[6] = static_cast<uint8_t>(tempo & 0xFF);
    packet[7] = static_cast<uint8_t>((color >> 16) & 0xFF);
    packet[8] = static_cast<uint8_t>((color >> 8) & 0xFF);
    packet[9] = static_cast<uint8_t>(color & 0xFF);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

void LightFX::SendSetDim(uint8_t dim, std::span<const uint8_t> zones) {
    uint8_t packet[33] = {0};
    packet[0] = m_preamble;
    packet[1] = m_setDim;
    packet[2] = dim;
    size_t zoneCount = std::min(zones.size(), size_t(28)); // 5+28=33
    packet[3] = static_cast<uint8_t>((zoneCount >> 8) & 0xFF);
    packet[4] = static_cast<uint8_t>(zoneCount & 0xFF);
    std::memcpy(packet + 5, zones.data(), zoneCount);
    m_deviceSend(std::span<const uint8_t>(packet, 33));
}

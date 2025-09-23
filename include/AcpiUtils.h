#pragma once
#include "Daemon.h"
#include "database.h"
#include <bitset>

class AcpiUtils {
  private:
    int m_acpiModelId{};
    const char *m_deviceName{};
    const char *m_acpiPrefix{};
    bool m_deviceResolved{true};
    std::bitset<7> m_featureSetBits{"1111111"};
    std::bitset<8> m_thermalModeBits{"1111111"};
    std::bitset<6> m_lightingModesBits{"111111"};
    static auto getPrefix() -> const char *;
    static auto m_getDeviceName() -> const char *;
    auto m_getDeviceAcpiCode() -> int;
    auto m_resolveDevicefromDatabase() -> int;
    Daemon m_daemon;

  public:
    AcpiUtils(Daemon &daemon);
    auto executeAcpiCommand(int arg1, int arg2 = 0x00, int arg3 = 0x00,
                            int arg4 = 0x00) -> int;
    [[nodiscard]] auto hasFeature(FeatureSet f) const -> bool;

    [[nodiscard]] auto hasThermalMode(ThermalModeSet m) const -> bool;

    [[nodiscard]] auto hasLightingMode(LightingSet l) const -> bool;
    void deviceInfo(bool unknownDevice = false) const;
};

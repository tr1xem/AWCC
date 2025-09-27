#pragma once
#include "Daemon.h"
#include "database.h"
#include <bitset>

class AcpiUtils {
  private:
    const char *m_deviceName{};
    const char *m_acpiPrefix{};
    bool m_deviceResolved{true};
    std::bitset<7> m_featureSetBits{"1111111"};
    std::bitset<8> m_thermalModeBits{"1111111"};
    std::bitset<6> m_lightingModesBits{"111111"};
    static const char *getPrefix();
    static const char *m_getDeviceName();
    int m_resolveDevicefromDatabase();
    Daemon &m_daemon;

  public:
    AcpiUtils(Daemon &daemon);
    int executeAcpiCommand(int arg1, int arg2 = 0x00, int arg3 = 0x00,
                           int arg4 = 0x00);
    [[nodiscard]] bool hasFeature(FeatureSet f) const;

    [[nodiscard]] bool hasThermalMode(ThermalModeSet m) const;

    [[nodiscard]] bool hasLightingMode(LightingSet l) const;
    void deviceInfo(bool unknownDevice = false);
};

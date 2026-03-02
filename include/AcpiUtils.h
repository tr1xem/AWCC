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
    int m_resolveDevicefromDatabase();
    static bool isIntel();
    static bool isAMD();
    bool m_testMode{false};
    Daemon *m_daemon{nullptr};

  public:
    AcpiUtils(bool testMode = false);
    void setDaemon(Daemon &daemon) { m_daemon = &daemon; }
    void deviceInfo(bool unknownDevice = false);
    ~AcpiUtils();
    int executeAcpiCommand(int arg1, int arg2 = 0x00, int arg3 = 0x00,
                           int arg4 = 0x00);
    [[nodiscard]] bool hasFeature(FeatureSet f) const;

    [[nodiscard]] bool hasThermalMode(ThermalModeSet m) const;

    [[nodiscard]] bool hasLightingMode(LightingSet l) const;
    [[nodiscard]] bool getTurboBoost() const;
    bool setTurboBoost(bool enable);
    [[nodiscard]] const std::vector<uint8_t> &getKeyboardZones() const {
        return m_keyboardZones;
    }


  private:
    std::vector<uint8_t> m_keyboardZones{0x00, 0x01, 0x02, 0x03};

};

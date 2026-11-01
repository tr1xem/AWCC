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
    Daemon &m_daemon;

  public:
    AcpiUtils(Daemon &daemon, bool testMode = false);
    ~AcpiUtils();
    int executeAcpiCommand(int arg1, int arg2 = 0x00, int arg3 = 0x00,
                           int arg4 = 0x00);
    [[nodiscard]] bool hasFeature(FeatureSet f) const;

    [[nodiscard]] bool hasThermalMode(ThermalModeSet m) const;

    [[nodiscard]] bool hasLightingMode(LightingSet l) const;
    void deviceInfo(bool unknownDevice = false);
    [[nodiscard]] bool getTurboBoost() const;
    bool setTurboBoost(bool enable);
    [[nodiscard]] const std::vector<uint8_t> &getKeyboardZones() const {
        return m_keyboardZones;
    }

    // Kernel alienware-wmi driver (platform_profile + hwmon), used as thermal
    // backend when acpi_call is not available
    [[nodiscard]] bool hasKernelThermal() const;
    [[nodiscard]] bool hasPlatformProfile(const std::string &profile) const;
    [[nodiscard]] std::string getPlatformProfile() const;
    bool setPlatformProfile(const std::string &profile);
    // fan is matched against the hwmon fan label, e.g. "CPU" or "GPU"
    // values are the driver's raw 0-255 range, -1 on failure
    [[nodiscard]] int getFanBoost(const std::string &fan) const;
    bool setFanBoost(const std::string &fan, int value);

  private:
    std::vector<uint8_t> m_keyboardZones = {0x00, 0x01, 0x02, 0x03};

    std::string m_platformProfileDir;
    std::string m_hwmonDir;
    void m_discoverKernelThermal();
    [[nodiscard]] std::string m_fanBoostPath(const std::string &fan) const;
    bool m_writeSysfs(const std::string &path, const std::string &value);
};

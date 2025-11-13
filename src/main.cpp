#include "AcpiUtils.h"
#include "Daemon.h"
#include "EffectController.h"
#include "LightFX.h"
#include "Thermals.h"
#include <Renderui.h>
#include <algorithm>
#include <cstring>
#include <format>
#include <ios>
#include <iostream>
#include <loguru.hpp>
#include <string>
#include <unistd.h> // for geteuid()

// TODO: Added checks for various lightparts
namespace awcc {
static auto shouldRunDaemon(std::span<char *> args) -> bool {
    return std::ranges::any_of(args.subspan(1), // skip program name
                               [](char *arg) {
                                   std::string_view s{arg};
                                   return s == "-d" || s == "--daemon";
                               });
}

static auto shouldRunTestMode(std::span<char *> args) -> bool {
    return std::ranges::any_of(args.subspan(1), // skip program name
                               [](char *arg) {
                                   std::string_view s{arg};
                                   return s == "--test-mode";
                               });
}

static void runDaemonServer(Daemon &daemon) {
#ifdef NDEBUG
    if (geteuid() != 0) {
        LOG_S(ERROR) << "Daemon server can only be run with admin permissions "
                        "(root). Exiting.";
        exit(1);
    }
    LOG_S(INFO) << "Starting daemon as server";
    daemon.init();
#else
    LOG_S(ERROR) << "In Debug mode, daemon cannot be started as server";
#endif
}

static void printHelp() {
    std::cout << std::format(R"(Alienware Command Center v{}
Copyright (c) 2025 tr1x_em. All Rights Reserved.
==========================================

App Commands:
  --daemon     (-d)        Run app in Daemon Mode (run with sudo)
  --gui        (-g)        Starts Gui
  --test-mode              Run in test mode (bypasses device detection)
  -v                       Runs app in verbose mosde

Keyboard Lighting Controls:
  brightness <0-100>       Set keyboard brightness
  static     <color_hex>   Set static color
  spectrum                 Color spectrum effect
  breathe    <color_hex>   Breathing effect
  rainbow                  Rainbow wave effect
  wave       <color_hex>   Wave effect
  bkf        <color_hex>   Back and forth effect
  defaultblue              Default blue color

Light bar Lighting Controls:
  lbbrightness <0-100>     Set light bar brightness
  lbrainbow                Rainbow wave effect
  lbspectrum               Color spectrum effect
  lbbreathe    <color_hex> Breathing effect
  lbdefaultblue            Default blue color

Fan Controls (Run as root):
  qm                       Query current fan mode
  quiet        (q )        Quiet mode for minimal noise
  battery      (bs)        Battery saving mode
  balance      (b )        Balanced performance and power
  fullspeed    (fs)        Full Speed mode
  performance  (p )        High performance mode
  gmode        (g )        Gaming mode (G-Mode)
  cool         (c )        Cool Mode
  manual       (m )        Manual mode
  gt                       Toggle G-Mode (useful for keybinds)


Fan Boost Controls (Run as root):
  cb                       Get CPU fan boost
  scb <value>              Set CPU fan boost (1-100)
  gb                       Get GPU fan boost
  sgb <value>              Set GPU fan boost (1-100)

Extra:
  setturbo <0,1> (st)      Sets Cpu turbo boost mode
  getturbo                 Gets Cpu turbo boost mode

System Information:
  device-info              Show detected device model and supported features
  test-zones               Scans for zones
  test-modes               Test all thermal modes and show device mappings
)",
                             VERSION)
              << "\n";
}

static inline uint32_t parseHexColor(const std::string &hex) {
    uint32_t color = 0;
    std::stringstream ss;
    ss << std::hex << hex;
    ss >> color;
    return color;
}

static int handleCliCommands(std::span<char *> args, EffectController &effects,
                             Thermals &thermals, AcpiUtils &acpiUtils) {
    if (args.size() < 2) {
        printHelp();
        return 0;
    }
    std::string_view cmd = args[1];

    // Help
    if (cmd == "-h" || cmd == "--help" || cmd == "help") {
        printHelp();
        return 0;
    }

    // Lighting Controls
    if (cmd == "static" && args.size() > 2) {
        uint32_t color = parseHexColor(args[2]);
        effects.StaticColor(color);
        std::cout << "Set static color: " << args[2] << "\n";
        return 0;
    }
    if (cmd == "breathe" && args.size() > 2) {
        uint32_t color = parseHexColor(args[2]);
        effects.Breathe(color);
        std::cout << "Set breathe color: " << args[2] << "\n";
        return 0;
    }
    if (cmd == "wave" && args.size() > 2) {
        uint32_t color = parseHexColor(args[2]);
        effects.Wave(color);
        std::cout << "Set wave color: " << args[2] << "\n";
        return 0;
    }
    if (cmd == "bkf" && args.size() > 2) {
        uint32_t color = parseHexColor(args[2]);
        effects.BackAndForth(color);
        std::cout << "Set back and forth color: " << args[2] << "\n";
        return 0;
    }
    if (cmd == "brightness" && args.size() > 2) {
        effects.Brightness(std::stoi(args[2]));
        std::cout << "Set brightness to " << args[2] << "\n";
        return 0;
    }
    if (cmd == "spectrum") {
        effects.Spectrum(1000);
        std::cout << "Set spectrum Mode \n";
        return 0;
    }
    if (cmd == "rainbow") {
        effects.Rainbow(500);
        std::cout << "Set rainbow mode \n";
        return 0;
    }
    if (cmd == "defaultblue") {
        effects.DefaultBlue();
        std::cout << "Set default blue color." << "\n";
        return 0;
    }
    // NOTE: Lighbar commands
    if (cmd == "lbspectrum") {
        effects.LightBarSpectrum(1000);
        std::cout << "Set light bar spectrum Mode \n";
        return 0;
    }
    if (cmd == "lbrainbow") {
        effects.LightbarRainbow(500);
        std::cout << "Set light bar rainbow mode \n";
        return 0;
    }

    if (cmd == "lbbrightness" && args.size() > 2) {
        effects.LightBarBrightness(std::stoi(args[2]));
        std::cout << "Set light bar brightness to " << args[2] << "\n";
        return 0;
    }
    if (cmd == "lbdefaultblue") {
        effects.LightBarDefaultBlue();
        std::cout << "Set default blue color for lightbar." << "\n";
        return 0;
    }
    if (cmd == "lbbreathe" && args.size() > 2) {
        uint32_t color = parseHexColor(args[2]);
        effects.LightBarBreathe(color);
        std::cout << "Set light bar breathe color: " << args[2] << "\n";
        return 0;
    }
    // Fan Controls
    if (cmd == "qm") {
        std::cout << "Current Mode: " << thermals.getCurrentModeName() << "\n";
        return 0;
    }
    if (cmd == "quiet" || cmd == "q") {
        thermals.setThermalMode(ThermalModes::Quiet);
        std::cout << "Set fan to quiet mode." << "\n";
        return 0;
    }
    if (cmd == "fullspeed" || cmd == "fs") {
        thermals.setThermalMode(ThermalModes::FullSpeed);
        std::cout << "Set fan to full speed mode." << "\n";
        return 0;
    }
    if (cmd == "battery" || cmd == "bs") {
        thermals.setThermalMode(ThermalModes::BatterySaver);
        std::cout << "Set fan to battery saving mode." << "\n";
        return 0;
    }
    if (cmd == "balance" || cmd == "b") {
        thermals.setThermalMode(ThermalModes::Balanced);
        std::cout << "Set fan to balanced mode." << "\n";
        return 0;
    }
    if (cmd == "performance" || cmd == "p") {
        thermals.setThermalMode(ThermalModes::Performance);
        std::cout << "Set fan to performance mode." << "\n";
        return 0;
    }
    if (cmd == "gmode" || cmd == "g") {
        thermals.setThermalMode(ThermalModes::Gmode);
        std::cout << "Set fan to gaming mode (G-Mode)." << "\n";
        return 0;
    }
    if (cmd == "gt") {
        thermals.toggleGmode();
        std::cout << "Toggle G-Mode." << "\n";
        return 0;
    }
    if (cmd == "cool" || cmd == "c") {
        thermals.setThermalMode(ThermalModes::Cool);
        std::cout << "Set fan to Cool mode." << "\n";
        return 0;
    }
    if (cmd == "manual" || cmd == "m") {
        thermals.setThermalMode(ThermalModes::Manual);
        std::cout << "Set fan to manual mode." << "\n";
        return 0;
    }

    // Fan Boost Controls
    if (cmd == "cb") {
        std::cout << "Cpu Boost: " << thermals.getCpuBoost() << "\n";
        return 0;
    }
    if (cmd == "scb" && args.size() > 2) {
        thermals.setCpuBoost(std::stoi(args[2]));
        std::cout << "Set CPU fan boost: " << args[2] << "\n";
        return 0;
    }
    if (cmd == "gb") {
        std::cout << "Gpu Boost: " << thermals.getGpuBoost() << "\n";
        std::cout << "Get GPU fan boost." << "\n";
        return 0;
    }
    if (cmd == "sgb" && args.size() > 2) {
        thermals.setGpuBoost(std::stoi(args[2]));
        std::cout << "Set GPU fan boost: " << args[2] << "\n";
        return 0;
    }

    if (cmd == "setturbo" || cmd == "st" && args.size() > 2) {
        acpiUtils.setTurboBoost(std::stoi(args[2]) != 0);
        std::cout << "Set Turbo Boost to: " << std::boolalpha
                  << std::stoi(args[2]) << "\n";
        return 0;
    }

    if (cmd == "getturbo") {
        std::cout << "Turbo Boost for Cpu: " << std::boolalpha
                  << acpiUtils.getTurboBoost() << "\n";
        return 0;
    }
    if (cmd == "device-info") {
        acpiUtils.deviceInfo();
        return 0;
    }

    if (cmd == "device-info") {
        acpiUtils.deviceInfo();
        return 0;
    }
    if (cmd == "test-zones") {
        effects.ScanZones();
        return 0;
    }

    if (cmd == "test-modes") {
        thermals.testThermalModes();
        return 0;
    }

    LOG_S(ERROR) << "Unknown command: " << cmd << "\n";
    // printHelp();
    return 1;
}

} // namespace awcc

int main(int argc, char *argv[]) {
    std::span<char *> args(argv, argc);
    // loguru::g_stderr_verbosity = -1;
    // loguru::init(argc, argv);

    bool verbose_mode = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-v") == 0) {
            verbose_mode = true;
            break;
        }
    }

    if (verbose_mode) {
        loguru::g_stderr_verbosity = 0;
    } else {
        loguru::g_stderr_verbosity = -1;
    }

    bool start_gui = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--gui") == 0) {
            start_gui = true;
            break;
        }
    }

    if (start_gui) {
        LOG_S(INFO) << "Initializing LightFX Module";
        LightFX lightfx;

        LOG_S(INFO) << "Initializing EffectController Module";
        EffectController effects(lightfx);

        LOG_S(INFO) << "Initializing Daemon Module";
        Daemon daemon(effects);

        LOG_S(INFO) << "Initializing AcpiUtils Module";
        bool testMode = awcc::shouldRunTestMode(args);
        AcpiUtils acpiUtils(daemon, testMode);

        LOG_S(INFO) << "Initializing Thermals Module";
        Thermals awccthermals(acpiUtils);

        // TODO: Add check for light mode in gui
        if (daemon.isDaemonRunning()) {
            LOG_S(INFO) << "Rendering UI";
            RenderUi::Init(awccthermals, acpiUtils, effects);
            return 0;
        } else {
            LOG_S(ERROR) << "GUI Mode Requires daemon by default,run daemon by "
                            "sudo ./main -d";
            return 1;
        }
    }
    if (awcc::shouldRunDaemon(args)) {
        LOG_S(INFO) << "Initializing LightFX Module";
        LightFX lightfx;

        LOG_S(INFO) << "Initializing EffectController Module";
        EffectController effects(lightfx);

        LOG_S(INFO) << "Initializing Daemon Module";
        Daemon daemon(effects);

        LOG_S(INFO) << "Initializing AcpiUtils Module";
        bool testMode = awcc::shouldRunTestMode(args);
        AcpiUtils acpiUtils(daemon, true);

        LOG_S(INFO) << "Initializing Thermals Module";
        Thermals awccthermals(acpiUtils);

        daemon.setOnGmodeKeyCallback(
            [&awccthermals]() { awccthermals.toggleGmode(); });
        awcc::runDaemonServer(daemon);

        if (geteuid() != 0) {
            LOG_S(ERROR)
                << "Daemon server can only be run with admin permissions "
                   "(root). Exiting.";
            exit(1);
        }
        LOG_S(INFO) << "Starting daemon as server";
        daemon.init();
    } else {
        LOG_S(INFO) << "Initializing LightFX Module";
        LightFX lightfx;

        LOG_S(INFO) << "Initializing EffectController Module";
        EffectController effects(lightfx);

        LOG_S(INFO) << "Initializing Daemon Module";
        Daemon daemon(effects);

        // LOG_S(INFO) << "Initializing AcpiUtils Module";
        bool testMode = awcc::shouldRunTestMode(args);
        AcpiUtils acpiUtils(daemon, testMode);

        LOG_S(INFO) << "Initializing Thermals Module";
        Thermals awccthermals(acpiUtils);

        return awcc::handleCliCommands(args, effects, awccthermals, acpiUtils);
    }

    // NOTE:  Stop the daemon from client
    // if (daemon.isDaemonRunning()) {
    //     LOG_S(INFO) << daemon.executeFromDaemon("stop");
    // }
    return 0;
}

#include "AcpiUtils.h"
#include "Daemon.h"
#include "LightFX.h"
#include "Thermals.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <loguru.hpp>
#include <unistd.h> // for geteuid()

namespace awcc {

static auto parseVerbosity(std::span<char *> args)
    -> std::pair<int, std::vector<char *>> {
    bool verbose = std::ranges::any_of(
        args.subspan(1), [](char *arg) { return std::strcmp(arg, "-v") == 0; });

    // Build modified argv for loguru
    std::vector<char *> loguru_argv;
    loguru_argv.push_back(args[0]); // program name

    if (verbose) {
        loguru_argv.push_back((char *)"-v");
        loguru_argv.push_back((char *)"0"); // INFO
    } else {
        loguru_argv.push_back((char *)"-v");
        loguru_argv.push_back((char *)"-1"); // suppress logs
    }

    loguru_argv.push_back(nullptr);

    int loguru_argc = static_cast<int>(loguru_argv.size() - 1);
    return {loguru_argc, loguru_argv};
}

static auto shouldRunDaemon(std::span<char *> args) -> bool {
    return std::ranges::any_of(args.subspan(1), // skip program name
                               [](char *arg) {
                                   std::string_view s{arg};
                                   return s == "-d" || s == "--daemon";
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

static void runClientMode(Daemon &daemon) {
    if (daemon.isDaemonRunning()) {
        LOG_S(INFO) << "Daemon Client Mode Initialized";
    }
    // AcpiUtils acpiUtils(daemon);
    // Thermals awccthermals(acpiUtils);
    //

    // acpiUtils.deviceInfo();
    // LOG_S(INFO) << "Current Thermal Mode: "
    //             << awccthermals.getCurrentModeName();
    // awccthermals.setThermalMode(ThermalModes::Quiet);
    // LOG_S(INFO) << "Current Thermal Mode: "
    //             << awccthermals.getCurrentModeName();
    // awccthermals.setThermalMode(ThermalModes::Cool);
    // LOG_S(INFO) << "Current Thermal Mode: "
    //             << awccthermals.getCurrentModeName();
    // awccthermals.setThermalMode(ThermalModes::Gmode);
    // LOG_S(INFO) << "Current Thermal Mode: "
    //             << awccthermals.getCurrentModeName();
    // awccthermals.setThermalMode(ThermalModes::Balanced);

    // NOTE:  Stop the daemon from client
    // if (daemon.isDaemonRunning()) {
    //     LOG_S(INFO) << daemon.executeFromDaemon("stop");
    // }
}

} // namespace awcc

int main(int argc, char *argv[]) {
    std::span<char *> args(argv, argc);
    auto [loguru_argc, loguru_argv] = awcc::parseVerbosity(args);
    loguru::init(loguru_argc, loguru_argv.data());
    LOG_S(INFO) << "Initializing Daemon";
    Daemon daemon;

    if (awcc::shouldRunDaemon(args)) {
        awcc::runDaemonServer(daemon);
        return 0;
    }

    // awcc::runClientMode(daemon);
    LightFX lightfx;
    lightfx.deviceOpen();
    lightfx.deviceAcquire();
    // NOTE: STATIC EXAMPLE

    lightfx.SendAnimationRemove(1);
    lightfx.SendAnimationConfigStart(1);
    lightfx.SendZoneSelect(1,
                           ZONE_ALL); // ZONE_ALL_ARRAY is
    lightfx.SendAddAction(ACTION_COLOR, 1, 2, 0x00FFFF);
    lightfx.SendAnimationConfigSave(1);
    lightfx.SendAnimationSetDefault(1);
    lightfx.SendAnimationRemove(1);
    //
    // int duration = 500;
    //
    //
    // NOTE: Specturm Example

    // lightfx.SendAnimationRemove(1);
    // lightfx.SendAnimationConfigStart(1);
    //
    // lightfx.SendZoneSelect(1, ZONE_LEFT);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFF0000);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFFA500);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFFFF00);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x008000);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x00BFFF);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x0000FF);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x800080);
    //
    // lightfx.SendZoneSelect(1, ZONE_MIDDLE_LEFT);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x800080);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFF0000);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFFA500);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFFFF00);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x008000);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x00BFFF);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x0000FF);
    //
    // lightfx.SendZoneSelect(1, ZONE_MIDDLE_RIGHT);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x0000FF);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x800080);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFF0000);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFFA500);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFFFF00);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x008000);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x00BFFF);
    //
    // lightfx.SendZoneSelect(1, ZONE_RIGHT);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x00BFFF);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x0000FF);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x800080);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFF0000);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFFA500);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0xFFFF00);
    // lightfx.SendAddAction(ACTION_MORPH, duration, 64, 0x008000);
    //
    // lightfx.SendAnimationConfigSave(1);
    // lightfx.SendAnimationSetDefault(1);
    //
    lightfx.deviceRelease();
    lightfx.deviceClose();
    return 0;
}

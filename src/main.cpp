#include "AcpiUtils.h"
#include "Daemon.h"
#include "EffectController.h"
#include "LightFX.h"
#include "Thermals.h"
#include <Renderui.h>
#include <algorithm>
#include <cstring>
#include <loguru.hpp>
#include <unistd.h> // for geteuid()

namespace awcc {
static int originalVerbosity{};
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
        originalVerbosity = 0;
    } else {
        loguru_argv.push_back((char *)"-v");
        loguru_argv.push_back((char *)"-1"); // suppress logs
        originalVerbosity = -1;
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

    AcpiUtils acpiUtils(daemon);
    Thermals awccthermals(acpiUtils);

    acpiUtils.deviceInfo();
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

// TODO: Light Commands using daemon
int main(int argc, char *argv[]) {
    std::span<char *> args(argv, argc);
    auto [loguru_argc, loguru_argv] = awcc::parseVerbosity(args);
    loguru::init(loguru_argc, loguru_argv.data());

    bool start_gui = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--gui") == 0) {
            start_gui = true;
            break;
        }
    }

    if (start_gui) {
        LOG_S(INFO) << "Initializing LightFX";
        LightFX lightfx;

        LOG_S(INFO) << "Initializing EffectController";
        EffectController effects(lightfx);

        LOG_S(INFO) << "Initializing Daemon";

        Daemon daemon(effects);
        AcpiUtils acpiUtils(daemon);
        Thermals awccthermals(acpiUtils);
        RenderUi::Init(awccthermals, acpiUtils);
        return 0;
    }
    if (awcc::shouldRunDaemon(args)) {
        LOG_S(INFO) << "Initializing LightFX";
        LightFX lightfx;

        LOG_S(INFO) << "Initializing EffectController";
        EffectController effects(lightfx);

        LOG_S(INFO) << "Initializing Daemon";

        Daemon daemon(effects);
        loguru::g_stderr_verbosity = -3;
        AcpiUtils acpiUtils(daemon);
        Thermals awccthermals(acpiUtils);
        loguru::g_stderr_verbosity = awcc::originalVerbosity;
        daemon.setOnGmodeKeyCallback(
            [&awccthermals]() { awccthermals.toggleGmode(); });

        if (geteuid() != 0) {
            LOG_S(ERROR)
                << "Daemon server can only be run with admin permissions "
                   "(root). Exiting.";
            exit(1);
        }
        LOG_S(INFO) << "Starting daemon as server";
        daemon.init();
    } else {
        LOG_S(INFO) << "Initializing LightFX";
        LightFX lightfx;

        LOG_S(INFO) << "Initializing EffectController";
        EffectController effects(lightfx);

        LOG_S(INFO) << "Initializing Daemon";
        Daemon daemon(effects);
        AcpiUtils acpiUtils(daemon);
        Thermals awccthermals(acpiUtils);
        LOG_S(INFO) << awccthermals.getCurrentModeName();
        // LOG_S(INFO) << "THERMALS : " << std::dec <<
        // awccthermals.getCpuBoost(); awccthermals.setCpuBoost(50); LOG_S(INFO)
        // << "THERMALS : " << std::dec << awccthermals.getCpuBoost();
    }

    return 0;
}

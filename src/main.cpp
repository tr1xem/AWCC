#include "AcpiUtils.h"
#include "Daemon.h"
#include "Thermals.h"
#include <algorithm>
#include <cstring>
#include <loguru.hpp>
#include <unistd.h> // for geteuid()

namespace awcc {

static void parseVerbosity(std::span<char *> args) {
    loguru::g_stderr_verbosity = -1;
    for (auto *arg : args.subspan(1)) { // skip program name
        if (std::strcmp(arg, "-v") == 0) {
            loguru::g_stderr_verbosity = 0; // INFO level
        }
    }
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
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    awccthermals.setThermalMode(ThermalModes::Quiet);
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    awccthermals.setThermalMode(ThermalModes::Cool);
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    awccthermals.setThermalMode(ThermalModes::Gmode);
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    awccthermals.setThermalMode(ThermalModes::Balanced);

    // NOTE:  Stop the daemon from client
    // if (daemon.isDaemonRunning()) {
    //     LOG_S(INFO) << daemon.executeFromDaemon("stop");
    // }
}

} // namespace awcc

int main(int argc, char *argv[]) {
    std::span<char *> args(argv, argc);
    awcc::parseVerbosity(args);

    LOG_S(INFO) << "Initializing Daemon";
    Daemon daemon;

    if (awcc::shouldRunDaemon(args)) {
        awcc::runDaemonServer(daemon);
        return 0;
    }

    awcc::runClientMode(daemon);
    return 0;
}

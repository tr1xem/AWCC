
#include "AcpiUtils.h"
#include "Daemon.h"
#include "Thermals.h"
#include <cstring>
#include <loguru.hpp>
#include <string>

int main(int argc, char *argv[]) {
    // Initialize loguru -- always first
    loguru::g_stderr_verbosity = -1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            loguru::g_stderr_verbosity = 0; // INFO level
        }
    }

    bool run_daemon = false;
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-d" ||
            std::string(argv[i]) == "--daemon") {
            run_daemon = true;
            break;
        }
    }

    LOG_S(INFO) << "Initializing Daemon";
    Daemon daemon;

    if (run_daemon) {
#ifdef NDEBUG
        LOG_S(INFO) << "Starting daemon as server";
        daemon.init();
#else
        LOG_S(ERROR) << "In Debug mode, daemon cannot be started as server";
#endif
        return 0;
    }

    if (daemon.isDaemonRunning()) {
        LOG_S(INFO) << "Daemon Client Mode Initialized";
    }
    AcpiUtils acpiUtils(daemon);
    Thermals awccthermals(acpiUtils);

    // LOG_S(INFO) << "Current Thermal Mode: "
    //             << awccthermals.getCurrentModeName();
    // awccthermals.setThermalMode(ThermalModes::Cool);
    // acpiUtils.deviceInfo();
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    awccthermals.setThermalMode(ThermalModes::Quiet);
    LOG_S(INFO) << "Current Thermal Mode: "
                << awccthermals.getCurrentModeName();
    // awccthermals.setThermalMode(ThermalModes::Performance);
    // awccthermals.setThermalMode(ThermalModes::Gmode);

    // NOTE:  Stop the daemon from client
    // if (daemon.isDaemonRunning()) {
    //     LOG_S(INFO) << daemon.executeFromDaemon("stop");
    // }
    return 0;
}

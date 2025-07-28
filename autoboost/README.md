# awcc_autoboost

automaded fan boost control for awcc

works based on a config which maps temperature ranges to boost levels

~currently written in bash, must be rewritten in c and have a systemd unit~
written in c, WIP

## current features

- map temperature ranges to fan boost levels
- map temperature ranges to modes
- advanced hysteresis handling
- temperature/boost/rpm and mode output on /var/run/awcc/ for non-root read access
- power state based configs
- systemd unit (WIP)

## TODOS

- [x] more flexible hysterisis handling and flexible boost switching (for example
different conditions for upboosting and downboosting)
- [x] avoid the dependency **sensors** if possible
- [ ] config file parsing (currently hardcoded defaults are used)
- [x] write info to /var/log/awcc
- [ ] smart logging system
- [ ] error handling
- [ ] client-daemon connection via unix sockets
- [ ] super boost: fans help each other
- [ ] take care about the other components too: ram, nvme, etc (use lm_sensors' sensors.h to read temps and critical temps)

## deprecated

There's an old prototype awcc_autoboost.sh script for automated boost management
which supports config file parsing from /etc/awcc/autoboost.conf. This script is
deprecated now. The development continues on the C version.

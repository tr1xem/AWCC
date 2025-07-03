# awcc_autoboost

automaded fan boost control for awcc

works based on a config which maps temperature ranges to boost levels

~currently written in bash, must be rewritten in c and have a systemd unit~
written in c, WIP

## current features

- map temperature ranges to fan boost levels
- map temperature ranges to modes
- hysteresis handling
- temperature/boost and mode output on /var/run/awcc/ for non-root read access
- systemd unit (WIP)

## TODOS (in C version)

- [x] more flexible hysterisis handling and flexible boost switching (for example
different conditions for upboosting and downboosting)
- [x] avoid the dependency **sensors** if possible
- [ ] config file parsing (currently hardcoded defaults are used)
- [x] write info to /var/log/awcc
- [ ] smart logging system
- [ ] error handling

## deprecated

There's an old prototype awcc_autoboost.sh script for automated boost management
which supports config file parsing from /etc/awcc/autoboost.conf. This script is
deprecated now. The development continues on the C version.

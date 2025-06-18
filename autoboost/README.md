# awcc_autoboost

automaded fan boost control for awcc

works based on a config which maps temperature ranges to boost levels

currently written in bash, must be rewritten in c and have a systemd unit

## current features

- map temperature ranges to fan boost levels
- hysteresis handling
- temperature/boost output on /var/run/awcc/ for non-root read access

## TODOS (in C version)

- systemd unit
- more flexible hysterisis handling and flexible boost switching (for example
different conditions for upboosting and downboosting)
- avoid the dependency **sensors** if possible

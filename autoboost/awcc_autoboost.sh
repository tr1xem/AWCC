#!/usr/bin/env bash

CONFIG="/etc/awcc/autoboost.conf"
CHECK_INTERVAL=2

declare -a CPU_RULES GPU_RULES
LAST_CPU_BOOST=""
LAST_GPU_BOOST=""

HYSTERESIS=3

LAST_CPU_TEMP=""
LAST_GPU_TEMP=""

COOLDOWN=10
LAST_CPU_BOOST_TIME=0
LAST_GPU_BOOST_TIME=0

mkdir -p /var/run/awcc

PIDFILE="/var/run/awcc/autoboost.pid"

# Prevent multiple instances
if [[ -f "$PIDFILE" ]] && kill -0 "$(cat "$PIDFILE")" 2>/dev/null; then
	echo "Already running with PID $(cat "$PIDFILE")"
	exit 1
fi

echo $$ > "$PIDFILE"

# Load config
load_config() {
	local section=""
	while IFS= read -r line; do
		line=$(echo "$line" | sed 's/[[:space:]]//g')
		[[ -z "$line" || "$line" =~ ^# ]] && continue

		if [[ "$line" =~ ^\[(.*)\]$ ]]; then
			section="${BASH_REMATCH[1]}"
			continue
		fi

		if [[ "$line" =~ ^([0-9]+)-([0-9]+)=([0-9]+)$ ]]; then
			rule="${BASH_REMATCH[1]}-${BASH_REMATCH[2]}=${BASH_REMATCH[3]}"
			if [[ "$section" == "CPU" ]]; then
				CPU_RULES+=("$rule")
			elif [[ "$section" == "GPU" ]]; then
				GPU_RULES+=("$rule")
			fi
		fi
	done < "$CONFIG"
}

# Match temperature to a rule
match_boost() {
	local TEMP=$1
	local -n RULES=$2

	for rule in "${RULES[@]}"; do
		IFS='-=' read -r MIN MAX BOOST <<< "$rule"
		if (( TEMP >= MIN && TEMP <= MAX )); then
			echo "$BOOST"
			return
		fi
	done
	echo ""
}

check_g_mode() {
	awcc qm 2>/dev/null | grep -iq 'G-Mode'
}

get_temp_cpu() {
	sensors dell_ddv-virtual-0 | grep 'CPU:' | sed 's/\s\+/ /g' | cut -f 2 -d ' ' | tr -d '+' | sed 's/\..*//'
}

get_temp_gpu() {
	sensors dell_ddv-virtual-0 | grep 'Video:' | sed 's/\s\+/ /g' | cut -f 2 -d ' ' | tr -d '+' | sed 's/\..*//'
}

load_config

CLEANED=''

# Cleanup function to reset fan boost
cleanup() {
	[[ -z "$CLEANED" ]] || return
	CLEANED="YES"

	if ! check_g_mode; then
		echo "Exiting: resetting fan boosts to 0"
		awcc scb 0
		awcc sgb 0
		echo 0 > /var/run/awcc/cpu_boost
		echo 0 > /var/run/awcc/gpu_boost
	else
		echo "Exiting in G-Mode: leaving boosts unchanged"
	fi

	rm -f "$PIDFILE"

	exit 0
}

# Trap script termination signals
trap cleanup SIGINT SIGTERM SIGHUP EXIT

g_mode=''

while true; do
	if check_g_mode; then
		if [[ -z "$g_mode" ]]
		then
			LAST_CPU_BOOST=""
			LAST_GPU_BOOST=""
			g_mode='on'
			echo 100 > /var/run/awcc/cpu_boost
			echo 100 > /var/run/awcc/gpu_boost
			touch /var/run/awcc/g_mode
			echo "G-Mode activated"
		fi
		sleep "$CHECK_INTERVAL"
		continue
	fi

	if ! [[ -z "$g_mode" ]]
	then
		rm /var/run/awcc/g_mode
		g_mode=''
		echo "G-Mode deactivated"
	fi

	CPU_TEMP=$(get_temp_cpu)
	GPU_TEMP=$(get_temp_gpu)
	echo "$CPU_TEMP" > /var/run/awcc/cpu_temp
	echo "$GPU_TEMP" > /var/run/awcc/gpu_temp

	[[ -z "$CPU_TEMP" || -z "$GPU_TEMP" ]] && sleep "$CHECK_INTERVAL" && continue

	CPU_BOOST=$(match_boost "$CPU_TEMP" CPU_RULES)
	GPU_BOOST=$(match_boost "$GPU_TEMP" GPU_RULES)

	now=$(date +%s)

	if [[ -n "$CPU_BOOST" && "$CPU_BOOST" != "$LAST_CPU_BOOST" ]]; then
		if [[ "$CPU_BOOST" -gt "$LAST_CPU_BOOST" ]]; then
			# Immediate upboost
			echo cpu "$CPU_TEMP $CPU_BOOST (upboost)"
			awcc scb "$CPU_BOOST"
			echo "$CPU_BOOST" > /var/run/awcc/cpu_boost
			LAST_CPU_BOOST="$CPU_BOOST"
			LAST_CPU_TEMP="$CPU_TEMP"
			LAST_CPU_BOOST_TIME=$now
		elif [[ "$CPU_TEMP" -le $((LAST_CPU_TEMP - HYSTERESIS)) && $((now - LAST_CPU_BOOST_TIME)) -ge $COOLDOWN ]]; then
			# Delayed downboost with hysteresis
			echo cpu "$CPU_TEMP $CPU_BOOST (downboost)"
			awcc scb "$CPU_BOOST"
			echo "$CPU_BOOST" > /var/run/awcc/cpu_boost
			LAST_CPU_BOOST="$CPU_BOOST"
			LAST_CPU_TEMP="$CPU_TEMP"
			LAST_CPU_BOOST_TIME=$now
		fi
	fi

	if [[ -n "$GPU_BOOST" && "$GPU_BOOST" != "$LAST_GPU_BOOST" ]]; then
		if [[ "$GPU_BOOST" -gt "$LAST_GPU_BOOST" ]]; then
			echo gpu "$GPU_TEMP $GPU_BOOST (upboost)"
			awcc sgb "$GPU_BOOST"
			echo "$GPU_BOOST" > /var/run/awcc/gpu_boost
			LAST_GPU_BOOST="$GPU_BOOST"
			LAST_GPU_TEMP="$GPU_TEMP"
			LAST_GPU_BOOST_TIME=$now
		elif [[ "$GPU_TEMP" -le $((LAST_GPU_TEMP - HYSTERESIS)) && $((now - LAST_GPU_BOOST_TIME)) -ge $COOLDOWN ]]; then
			echo gpu "$GPU_TEMP $GPU_BOOST (downboost)"
			awcc sgb "$GPU_BOOST"
			echo "$GPU_BOOST" > /var/run/awcc/gpu_boost
			LAST_GPU_BOOST="$GPU_BOOST"
			LAST_GPU_TEMP="$GPU_TEMP"
			LAST_GPU_BOOST_TIME=$now
		fi
	fi

	sleep "$CHECK_INTERVAL"
done

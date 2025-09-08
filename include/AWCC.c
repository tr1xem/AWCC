# include "AWCC.h"

# define _GNU_SOURCE
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# include "AWCCACPI.h"
# include "AWCCUtils.h"

static void Initialize (void);

static AWCCBoost_t GetCpuBoost (void);
static AWCCBoost_t GetGpuBoost (void);

static void SetCpuBoost (AWCCBoost_t);
static void SetGpuBoost (AWCCBoost_t);

static AWCCTemperature_t GetCpuTemperature (void);
static AWCCTemperature_t GetGpuTemperature (void);

static AWCCFanRpm_t GetCpuRpm (void);
static AWCCFanRpm_t GetGpuRpm (void);

static AWCCBoost_t GetFanBoost (enum AWCCFan_t);
static void SetFanBoost (enum AWCCFan_t, AWCCBoost_t);
static AWCCTemperature_t GetFanTemperature (enum AWCCFan_t);
static AWCCFanRpm_t GetFanRpm (enum AWCCFan_t);
static const char * GetFanName (enum AWCCFan_t);

static void SetMode (enum AWCCMode_t);
static enum AWCCMode_t GetMode (void);
static const char * GetModeName (enum AWCCMode_t);
static void ToggleGMode (void);

static enum AWCCPowerState_t PowerState (void);

static void Deinitialize (void);

const struct AWCC_t AWCC = {
	.Initialize = & Initialize,

	.GetCpuBoost = & GetCpuBoost,
	.GetGpuBoost = & GetGpuBoost,

	.SetCpuBoost = & SetCpuBoost,
	.SetGpuBoost = & SetGpuBoost,

	.GetCpuTemperature = & GetCpuTemperature,
	.GetGpuTemperature = & GetGpuTemperature,

	.GetCpuRpm = & GetCpuRpm,
	.GetGpuRpm = & GetGpuRpm,

	.GetFanBoost = & GetFanBoost,
	.SetFanBoost = & SetFanBoost,
	.GetFanTemperature = & GetFanTemperature,
	.GetFanRpm = & GetFanRpm,
	.GetFanName = & GetFanName,

	.SetMode = & SetMode,
	.GetMode = & GetMode,
	.GetModeName = & GetModeName,
	.ToggleGMode = & ToggleGMode,

	.PowerState = & PowerState,

	.Deinitialize = & Deinitialize,
};

struct {
	const char * const * FanNames;
	const char * const * ModeNames;
} static Internal = {
	.FanNames = (const char * []) {
		[AWCCFanCPU] = "CPU",
		[AWCCFanGPU] = "GPU",
	},
	.ModeNames = (const char * []) {
		[AWCCModeQuiet]          =   "Quiet",
		[AWCCModeBatterySaver]   =   "BatterySaver",
		[AWCCModeBalanced]       =   "Balanced",
		[AWCCModePerformance]    =   "Performance",
		[AWCCModeG]              =   "G"
	},
};

void Initialize (void)
{
	AWCCACPI.Initialize ();
}

void Deinitialize (void)
{
}

AWCCBoost_t GetCpuBoost (void)
{
	return GetFanBoost (AWCCFanCPU);
}

AWCCBoost_t GetGpuBoost (void)
{
	return GetFanBoost (AWCCFanGPU);
}

void SetCpuBoost (AWCCBoost_t boost)
{
	SetFanBoost (AWCCFanCPU, boost);
}

void SetGpuBoost (AWCCBoost_t boost)
{
	SetFanBoost (AWCCFanGPU, boost);
}

AWCCTemperature_t GetCpuTemperature (void)
{
	return GetFanTemperature (AWCCFanCPU);
}

AWCCTemperature_t GetGpuTemperature (void)
{
	return GetFanTemperature (AWCCFanGPU);
}

AWCCFanRpm_t GetCpuRpm (void)
{
	return GetFanRpm (AWCCFanCPU);
}

AWCCFanRpm_t GetGpuRpm (void)
{
	return GetFanRpm (AWCCFanGPU);
}

AWCCBoost_t GetFanBoost (enum AWCCFan_t fan)
{
	return AWCCACPI.GetFanBoost (fan);
}

void SetFanBoost (enum AWCCFan_t fan, AWCCBoost_t boost)
{
	AWCCACPI.SetFanBoost (fan, boost);
}

AWCCTemperature_t GetFanTemperature (enum AWCCFan_t fan)
{
	return AWCCACPI.GetFanTemperature (fan);
}

AWCCFanRpm_t GetFanRpm (enum AWCCFan_t fan)
{
	return AWCCACPI.GetFanRpm (fan);
}

const char * GetFanName (enum AWCCFan_t fan) {
	return Internal.FanNames [fan];
}

void SetMode (enum AWCCMode_t mode)
{
	AWCCACPI.SetMode (mode);
}

enum AWCCMode_t GetMode (void)
{
	return AWCCACPI.GetMode ();
}

const char * GetModeName (enum AWCCMode_t mode)
{
	return Internal.ModeNames [mode];
}

enum AWCCPowerState_t PowerState (void)
{
	static _Thread_local char buffer [128];

	FILE * fp = fopen ("/sys/class/power_supply/BAT0/status", "r");

	if (NULL == fp) {
		perror ("Unable to read /sys/class/power_supply/BAT0/status");
		exit (EXIT_FAILURE);
	}

	fgets (buffer, sizeof (buffer), fp);
	fclose (fp);

	_Bool discharging = (
		NULL != strstr (buffer, "Discharging")
	);

	if (1 == discharging) {
		return AWCCPowerStateBAT;
	}
	else {
		return AWCCPowerStateAC;
	}
}

void ToggleGMode (void)
{
	extern const char *acpi_prefix;
	char cmd[256];
	snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x25 {0x02, 0x00, 0x00, 0x00}",
			acpi_prefix);
	executeAcpiCall(cmd);

	usleep(100000);
	char *response = readAcpiResponse();

	if (strstr(response, "0x0")) {
		printf("G-Mode is currently OFF. Enabling Gaming Mode...\n");
		SetMode(AWCCModeG);
		snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x25 {0x01, 0x01, 0x00, 0x00}",
				acpi_prefix);
		executeAcpiCall(cmd);
	} else if (strstr(response, "0x1")) {
		printf("G-Mode is currently ON. Reverting to Default Mode...\n");
		SetMode(AWCCModePerformance);
		snprintf(cmd, sizeof(cmd), "\\_SB.%s.WMAX 0 0x25 {0x01, 0x00, 0x00, 0x00}",
				acpi_prefix);
		executeAcpiCall(cmd);
	} else {
		fprintf(stderr, "Unable to determine G-Mode status. Response: %s\n",
				response);
	}
}

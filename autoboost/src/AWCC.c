# include "AWCC.h"

# include <stdio.h>
# include <stdlib.h>

# include "AWCCACPI.h"

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

static void SetMode (enum AWCCMode_t);
static enum AWCCMode_t GetMode (void);
static const char * GetModeName (enum AWCCMode_t);

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

	.SetMode = & SetMode,
	.GetMode = & GetMode,
	.GetModeName = & GetModeName,

	.Deinitialize = & Deinitialize,
};

struct {
	const char * const * ModeNames;
} static Internal = {
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

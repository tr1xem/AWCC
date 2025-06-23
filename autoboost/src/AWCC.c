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

static AWCCBoost_t GetFanBoost (enum AWCCFan_t);
static void SetFanBoost (enum AWCCFan_t, AWCCBoost_t);
static AWCCTemperature_t GetFanTemperature (enum AWCCFan_t);

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

	.GetFanBoost = & GetFanBoost,
	.SetFanBoost = & SetFanBoost,
	.GetFanTemperature = & GetFanTemperature,

	.SetMode = & SetMode,
	.GetMode = & GetMode,
	.GetModeName = & GetModeName,

	.Deinitialize = & Deinitialize,
};

struct {
	const char * const * ModeNames;
	enum AWCCMode_t LastMode;
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
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

AWCCBoost_t GetGpuBoost (void)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

void SetCpuBoost (AWCCBoost_t boost)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

void SetGpuBoost (AWCCBoost_t boost)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

AWCCTemperature_t GetCpuTemperature (void)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

AWCCTemperature_t GetGpuTemperature (void)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

AWCCBoost_t GetFanBoost (enum AWCCFan_t fan)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

void SetFanBoost (enum AWCCFan_t fan, AWCCBoost_t boost)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

AWCCTemperature_t GetFanTemperature (enum AWCCFan_t fan)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

void SetMode (enum AWCCMode_t mode)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

enum AWCCMode_t GetMode (void)
{
	return AWCCACPI.GetMode ();
}

const char * GetModeName (enum AWCCMode_t mode)
{
	return Internal.ModeNames [mode];
}

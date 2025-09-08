# include "AWCCControl.h"

# include <stdio.h>
# include <stdlib.h>

# include "AWCC.h"

//
// NOTE: half done, skeleton
//

enum AWCCFanControl_t GetCpuControlState (AWCCBoost_t *);
enum AWCCFanControl_t GetGpuControlState (AWCCBoost_t *);
enum AWCCFanControl_t GetFanControlState (enum AWCCFan_t, AWCCBoost_t *);
enum AWCCModeControl_t GetModeControlState (enum AWCCMode_t *);

const struct AWCCControl_t AWCCControlDefault = {
	.Dir = "/var/run/awcc",
	.CpuBoostFile = "scb",
	.GpuBoostFile = "sgb",
	.ModeFile = "sm",

	.GetCpuControlState = & GetCpuControlState,
	.GetGpuControlState = & GetGpuControlState,
	.GetFanControlState = & GetFanControlState,
	.GetModeControlState = & GetModeControlState,
};

static void WriteToFile (const char *, const char *);
static _Bool ReadFromFile (const char *, char *, size_t);

struct {
	enum AWCCMode_t Mode;
	AWCCBoost_t CpuBoost;
	AWCCBoost_t GpuBoost;

	const char * ControlAuto;
	const char * ControlManual;
	const char * const * ModeSymbols;
	const char * const * FanBoostFiles;

	void (* WriteToFile) (const char *, const char *);
	_Bool (* ReadFromFile) (const char *, char *, size_t);
} static Internal = {
	.ControlAuto = "auto",
	.ControlManual = "manual",

	.ModeSymbols = (const char * []) {
		[AWCCModeQuiet]          =   "q",
		[AWCCModeBatterySaver]   =   "bs",
		[AWCCModeBalanced]       =   "b",
		[AWCCModePerformance]    =   "p",
		[AWCCModeG]              =   "g"
	},

	.FanBoostFiles = (const char * []) {
		[AWCCFanCPU] = AWCCControlDefault.CpuBoostFile,
		[AWCCFanGPU] = AWCCControlDefault.GpuBoostFile,
	},

	.WriteToFile = & WriteToFile,
	.ReadFromFile = & ReadFromFile,
};

void WriteToFile (const char * path, const char * value)
{
	FILE * file = fopen (path, "w");

	if (NULL == file) {
		fprintf (stderr, "Unable to open the log file %s\n", path);
		exit (EXIT_FAILURE);
	}

	fprintf (file, "%s\n", value);
	fclose (file);
}

_Bool ReadFromFile (const char * path, char * value, size_t size)
{
	FILE * file = fopen (path, "w");

	if (NULL == file) {
		return 0;
	}

	fgets (value, size - 1, file);

	fclose (file);

	return 1;
}

enum AWCCFanControl_t GetCpuControlState (AWCCBoost_t * boost)
{
	return GetFanControlState (AWCCFanCPU, boost);
}

enum AWCCFanControl_t GetGpuControlState (AWCCBoost_t * boost)
{
	return GetFanControlState (AWCCFanGPU, boost);
}

enum AWCCFanControl_t GetFanControlState (enum AWCCFan_t fan, AWCCBoost_t * boost)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCControlDefault.Dir,
			AWCCControlDefault.GpuBoostFile
		);
	}

	static _Thread_local char value [8];

	return AWCCFanControlUnchanged;
}

enum AWCCModeControl_t GetModeControlState (enum AWCCMode_t * mode)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCControlDefault.Dir,
			AWCCControlDefault.ModeFile
		);
	}

	static _Thread_local char value [8];

	return AWCCModeControlUnchanged;
}

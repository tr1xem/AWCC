# include "AWCCSystemLogger.h"

# include <stdio.h>
# include <stdlib.h>

static const char * GetModeName (enum AWCCMode_t);
static void LogCpuTemp (AWCCTemperature_t);
static void LogGpuTemp (AWCCTemperature_t);
static void LogCpuBoost (AWCCBoost_t);
static void LogGpuBoost (AWCCBoost_t);
static void LogCpuRpm (AWCCFanRpm_t);
static void LogGpuRpm (AWCCFanRpm_t);
static void LogMode (enum AWCCMode_t);

const struct AWCCSystemLogger_t AWCCSystemLoggerDefault = {
	.Dir = "/var/run/awcc",
	.CpuBoostFile = "cb",
	.GpuBoostFile = "gb",
	.CpuTempFile = "ct",
	.GpuTempFile = "gt",
	.CpuRpmFile = "cr",
	.GpuRpmFile = "gr",
	.ModeFile = "m",

	.GetModeName = & GetModeName,
	.LogCpuTemp = & LogCpuTemp,
	.LogGpuTemp = & LogGpuTemp,
	.LogCpuBoost = & LogCpuBoost,
	.LogGpuBoost = & LogGpuBoost,
	.LogCpuRpm = & LogCpuRpm,
	.LogGpuRpm = & LogGpuRpm,
	.LogMode = & LogMode,
};

static void WriteToFile (const char *, const char *);
// static void MakePath (char *, const char *, const char *);

struct {
	const char * const * ModeSymbols;
	void (* WriteToFile) (const char *, const char *);
	// void (* MakePath) (char *, const char *, const char *);
} static Internal = {
	.ModeSymbols = (const char * []) {
		[AWCCModeQuiet]          =   "q",
		[AWCCModeBatterySaver]   =   "bs",
		[AWCCModeBalanced]       =   "b",
		[AWCCModePerformance]    =   "p",
		[AWCCModeG]              =   "g"
	},
	.WriteToFile = & WriteToFile,
	// .MakePath = & MakePath,
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

// void MakePath (char * out, const char * dir, const char * file)
// {
// }

const char * GetModeName (enum AWCCMode_t mode)
{
	return Internal.ModeSymbols [mode];
}

void LogCpuTemp (AWCCTemperature_t temp)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCSystemLoggerDefault.Dir,
			AWCCSystemLoggerDefault.CpuTempFile
		);
	}

	static _Thread_local char value [8];
	snprintf (value, sizeof (value), "%d", (int) temp);
	Internal.WriteToFile (path, value);
}

void LogGpuTemp (AWCCTemperature_t temp)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCSystemLoggerDefault.Dir,
			AWCCSystemLoggerDefault.GpuTempFile
		);
	}

	static _Thread_local char value [8];
	snprintf (value, sizeof (value), "%d", (int) temp);
	Internal.WriteToFile (path, value);
}

void LogCpuBoost (AWCCBoost_t boost)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCSystemLoggerDefault.Dir,
			AWCCSystemLoggerDefault.CpuBoostFile
		);
	}

	static _Thread_local char value [8];
	snprintf (value, sizeof (value), "%d", (int) boost);
	Internal.WriteToFile (path, value);

}

void LogGpuBoost (AWCCBoost_t boost)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCSystemLoggerDefault.Dir,
			AWCCSystemLoggerDefault.GpuBoostFile
		);
	}

	static _Thread_local char value [8];
	snprintf (value, sizeof (value), "%d", (int) boost);
	Internal.WriteToFile (path, value);
}

void LogCpuRpm (AWCCFanRpm_t rpm)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCSystemLoggerDefault.Dir,
			AWCCSystemLoggerDefault.CpuRpmFile
		);
	}

	static _Thread_local char value [8];
	snprintf (value, sizeof (value), "%d", (int) rpm);
	Internal.WriteToFile (path, value);
}

void LogGpuRpm (AWCCFanRpm_t rpm)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCSystemLoggerDefault.Dir,
			AWCCSystemLoggerDefault.GpuRpmFile
		);
	}

	static _Thread_local char value [8];
	snprintf (value, sizeof (value), "%d", (int) rpm);
	Internal.WriteToFile (path, value);
}

void LogMode (enum AWCCMode_t mode)
{
	static _Thread_local char path [256] = {0};
	if (0 == path [0]) {
		snprintf (
			path,
			sizeof (path),
			"%s/%s",
			AWCCSystemLoggerDefault.Dir,
			AWCCSystemLoggerDefault.ModeFile
		);
	}

	Internal.WriteToFile (path, AWCCSystemLoggerDefault.GetModeName (mode));
}

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "AWCC.h"

static void Initialize (void);
static AWCCBoost_t GetCpuBoost (void);
static AWCCBoost_t GetGpuBoost (void);
static void SetCpuBoost (AWCCBoost_t);
static void SetGpuBoost (AWCCBoost_t);
static AWCCTemperature_t GetCpuTemperature (void);
static AWCCTemperature_t GetGpuTemperature (void);
static void SetMode (enum AWCCMode_t);
static enum AWCCMode_t GetMode (void);
const char * GetModeName (enum AWCCMode_t);
static void Deinitialize (void);

const struct AWCC_t AWCC = {
	.Initialize = & Initialize,
	.GetCpuBoost = & GetCpuBoost,
	.GetGpuBoost = & GetGpuBoost,
	.SetCpuBoost = & SetCpuBoost,
	.SetGpuBoost = & SetGpuBoost,
	.GetCpuTemperature = & GetCpuTemperature,
	.GetGpuTemperature = & GetGpuTemperature,
	.SetMode = & SetMode,
	.GetMode = & GetMode,
	.GetModeName = & GetModeName,
	.Deinitialize = & Deinitialize,
};

static void DetectCpuVendor (void);
static void DeviceOpen (void);
static void DeviceClose (void);

struct {
	const char * AcpiPrefix;
	const char * const * ModeNames;
	enum AWCCMode_t LastMode;
	void (* DetectCpuVendor) (void);
	void (* DeviceOpen) (void);
	void (* DeviceClose) (void);
} static InternalData = {
	.AcpiPrefix = "AMWW",
	.ModeNames = (const char * []) {
		[AWCCModeQuiet]          =   "Quiet",
		[AWCCModeBatterySaver]   =   "BatterySaver",
		[AWCCModeBalanced]       =   "Balanced",
		[AWCCModePerformance]    =   "Performance",
		[AWCCModeG]              =   "G"
	},
	.DetectCpuVendor = DetectCpuVendor,
	.DeviceOpen = DeviceOpen,
	.DeviceClose = DeviceClose,
};

void Initialize (void)
{
	InternalData.DetectCpuVendor ();
	InternalData.DeviceOpen ();
}

void Deinitialize (void)
{
	InternalData.DeviceClose ();
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

void SetMode (enum AWCCMode_t mode)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

enum AWCCMode_t GetMode (void)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

const char * GetModeName (enum AWCCMode_t mode) {
	return InternalData.ModeNames [mode];
}

void DetectCpuVendor (void)
{
	FILE *cpuinfo = fopen("/proc/cpuinfo", "r");
	if (!cpuinfo) {
		perror("Failed to open /proc/cpuinfo");
		return;
	}

	char line[256];
	while (fgets(line, sizeof(line), cpuinfo)) {
		if (strstr(line, "vendor_id")) {
			if (strstr(line, "AuthenticAMD")) {
				InternalData.AcpiPrefix = "AMW3"; // AMD detected
			}
			break;
		}
	}
	// printf("Detected CPU vendor: %s\n", acpi_prefix);
	fclose(cpuinfo);
}

void DeviceOpen (void)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

void DeviceClose (void)
{
	fputs ("Not Implemented Yet", stderr);
	exit (-1);
}

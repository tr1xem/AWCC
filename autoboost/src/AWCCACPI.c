# include "AWCCACPI.h"

# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <threads.h>
# include <time.h>

# include "AWCC.h"

static void Initialize (void);
static void Execute (const char *);
static const char * Read (void);
static enum AWCCMode_t GetMode (void);
static void SetMode (enum AWCCMode_t);
static AWCCBoost_t GetFanBoost (enum AWCCFan_t);
static void SetFanBoost (enum AWCCFan_t, AWCCBoost_t);
static unsigned int ParseHexValue (const char *);
static AWCCTemperature_t GetFanTemperature (enum AWCCFan_t);

const struct AWCCACPI_t AWCCACPI = {
	.Initialize = & Initialize,
	.GetMode = & GetMode,
	.SetMode = & SetMode,
	.GetFanBoost = & GetFanBoost,
	.SetFanBoost = & SetFanBoost,
	.GetFanTemperature = & GetFanTemperature,
};

struct {
	const char * Prefix;
	// const char * CmdGetCPUBoost;
	// const char * CmdGetGPUBoost;
	// const char * CmdSetCPUBoost;
	// const char * CmdSetGPUBoost;
	const char * CmdGetFanBoost;
	const char * CmdSetFanBoost;
	const char * CmdGetCurrentMode;
	const char * CmdSetCurrentMode;
	const char * CmdGetTemperature;

	unsigned * ModeToHexMap;
	enum AWCCMode_t * HexToModeMap;

	unsigned * FanToBoostHexMap;
	enum AWCCFan_t * BoostHexToFanMap;

	unsigned * FanToTempHexMap;
	// enum AWCCFan_t * TempHexToFanMap;

	void (* Execute) (const char *);
	const char * (* Read) (void);
	unsigned int (* ParseHexValue) (const char *);
} static Internal = {
	 .Prefix = "AMWW",

	// .CmdGetCPUBoost      =   "\\_SB.%s.WMAX 0 0x14 {0x0c, 0x32, 0x00, 0x00}"    ,
	// .CmdGetGPUBoost      =   "\\_SB.%s.WMAX 0 0x14 {0x0c, 0x33, 0x00, 0x00}"    ,
	// .CmdSetCPUBoost      =   "\\_SB.%s.WMAX 0 0x15 {0x02, 0x32, 0x%02x, 0x00}"  ,
	// .CmdSetGPUBoost      =   "\\_SB.%s.WMAX 0 0x15 {0x02, 0x33, 0x%02x, 0x00}"  ,
	.CmdGetFanBoost      =   "\\_SB.%s.WMAX 0 0x14 {0x0c, 0x%02x, 0x00, 0x00}"    ,
	.CmdSetFanBoost      =   "\\_SB.%s.WMAX 0 0x15 {0x02, 0x%02x, 0x%02x, 0x00}"  ,
	.CmdGetCurrentMode   =   "\\_SB.%s.WMAX 0 0x14 {0x0b, 0x00, 0x00, 0x00}"    ,
	.CmdSetCurrentMode   =   "\\_SB.%s.WMAX 0 0x15 {0x01, 0x%02x, 0x00, 0x00}"  ,
	.CmdGetTemperature   =   "\\_SB.%s.WMAX 0 0x14 {0x04, 0x%02x, 0x00, 0x00}"  ,


	.ModeToHexMap = (unsigned []) {
		[AWCCModeBalanced]       = 0xa0  ,
		[AWCCModePerformance]    = 0xa1  ,
		[AWCCModeQuiet]          = 0xa3  ,
		[AWCCModeBatterySaver]   = 0xa5  ,
		[AWCCModeG]              = 0xab  ,
		// [AWCCModeManual]         = 0x00  ,
	},

	.HexToModeMap = (enum AWCCMode_t []) {
		[0xa0]  =  AWCCModeBalanced      ,
		[0xa1]  =  AWCCModePerformance   ,
		[0xa3]  =  AWCCModeQuiet         ,
		[0xa5]  =  AWCCModeBatterySaver  ,
		[0xab]  =  AWCCModeG             ,
		// [0x00]  =  AWCCModeManual        ,
	},

	.FanToBoostHexMap = (unsigned []) {
		[AWCCFanCPU] = 0x32,
		[AWCCFanGPU] = 0x33,
	},

	.BoostHexToFanMap = (enum AWCCFan_t []) {
		[0x32] = AWCCFanCPU,
		[0x33] = AWCCFanGPU,
	},

	.FanToTempHexMap = (unsigned []) {
		[AWCCFanCPU] = 0x01,
		[AWCCFanGPU] = 0x06,
	},

	// .TempHexToFanMap = (enum AWCCFan_t []) {
	// 	[0x01] = AWCCFanCPU,
	// 	[0x06] = AWCCFanGPU,
	// },

	.Execute = & Execute,
	.Read = & Read,
	.ParseHexValue = & ParseHexValue,
};

void Initialize (void)
{
	FILE * cpuinfo = fopen ("/proc/cpuinfo", "r");
	if (NULL == cpuinfo) {
		perror ("Failed to open /proc/cpuinfo");
		return;
	}

	char line [256];

	while (NULL != fgets (line, sizeof(line), cpuinfo)) {
		if (NULL != strstr (line, "vendor_id")) {
			if (NULL != strstr (line, "AuthenticAMD")) {
				Internal.Prefix = "AMW3"; // AMD detected
			}
			break;
		}
	}

	fclose (cpuinfo);
}

void Execute (const char * command)
{
	FILE * acpi_file = fopen ("/proc/acpi/call", "w");

	if (NULL == acpi_file) {
		perror ("Unable to open /proc/acpi/call");
		exit (EXIT_FAILURE);
	}

	fprintf (acpi_file, "%s\n", command);
	fclose (acpi_file);
}

const char * Read (void)
{
	static _Thread_local char buffer [128];

	FILE * fp = fopen ("/proc/acpi/call", "r");

	if (NULL == fp) {
		perror ("Unable to read /proc/acpi/call");
		exit (EXIT_FAILURE);
	}

	fgets (buffer, sizeof (buffer), fp);
	fclose (fp);

	for (char * p = buffer; 0 != * p; p++) {
		* p = tolower (* p);
	}

	return buffer;
}

unsigned int ParseHexValue (const char * response)
{
	char * start = strstr (response, "0x");
	if (NULL == start) {
		fprintf (stderr, "No hex value found in response.\n");
		return -1;
	}
	unsigned int value;
	if (sscanf (start, "%x", & value) == 1)
		return value;
	else {
		fprintf (stderr, "Failed to parse hex value.\n");
		return -1;
	}
}

AWCCBoost_t GetFanBoost (enum AWCCFan_t fan)
{
	static _Thread_local char cmd [256];
	snprintf (
		cmd,
		sizeof (cmd),
		Internal.CmdGetFanBoost,
		Internal.Prefix,
		Internal.FanToBoostHexMap [fan]
	);

	Internal.Execute (cmd);

	thrd_sleep (& (struct timespec) {.tv_nsec = 1E8}, NULL);
	const char * response = Internal.Read ();

	unsigned int response_uint = Internal.ParseHexValue (response);


	if (-1 == response_uint) {
		fputs ("Failed to get the the current fan boost.\n", stderr);
		exit (-1);
	}

	return response_uint;
}

void SetFanBoost (enum AWCCFan_t fan, AWCCBoost_t boost)
{
	static _Thread_local char cmd [256];

	if (AWCCFanBoostMin > boost || AWCCFanBoostMax < boost) {
		fprintf (stderr, "Illegal fan boost value %d.\n", boost);
		exit (-1);
	}

	snprintf (
		cmd,
		sizeof (cmd),
		Internal.CmdSetFanBoost,
		Internal.Prefix,
		Internal.FanToBoostHexMap [fan],
		boost
	);
	Internal.Execute (cmd);
}

enum AWCCMode_t GetMode (void)
{
	static _Thread_local char cmd [256];
	snprintf (cmd, sizeof (cmd), Internal.CmdGetCurrentMode, Internal.Prefix);
	Internal.Execute (cmd);

	thrd_sleep (& (struct timespec) {.tv_nsec = 1E8}, NULL);
	const char * response = Internal.Read ();
	unsigned int response_uint;
	int status = sscanf (response, "0x%2x", & response_uint);

	if (EOF == status) {
		fputs ("Failed to get the current mode.\n", stderr);
		exit (-1);
	}

	return Internal.HexToModeMap [response_uint];
}

AWCCTemperature_t GetFanTemperature (enum AWCCFan_t fan)
{
	// FIXME: sometimes returns 160, although temperature is around 40-50
	// no matter cpu or gpu
	static _Thread_local char cmd [256];
	snprintf (
		cmd,
		sizeof (cmd),
		Internal.CmdGetTemperature,
		Internal.Prefix,
		Internal.FanToTempHexMap [fan]
	);
	Internal.Execute (cmd);

	thrd_sleep (& (struct timespec) {.tv_nsec = 1E8}, NULL);
	const char * response = Internal.Read ();

	unsigned int response_uint;
	int status = sscanf (response, "0x%2x", & response_uint);

	if (EOF == status) {
		fputs ("Failed to get the current mode.\n", stderr);
		exit (-1);
	}

	return response_uint;
}

void SetMode (enum AWCCMode_t mode)
{
	static _Thread_local char cmd [256];
	snprintf (
		cmd,
		sizeof (cmd),
		Internal.CmdSetCurrentMode,
		Internal.Prefix,
		Internal.ModeToHexMap [mode]
	);
	Internal.Execute (cmd);
}

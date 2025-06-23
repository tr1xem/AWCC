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

const struct AWCCACPI_t AWCCACPI = {
	.Initialize = & Initialize,
	.Execute = & Execute,
	.Read = & Read,
	.GetMode = & GetMode,
};

struct {
	const char * Prefix;
	const char * CmdGetCPUBoost;
	const char * CmdGetGPUBoost;
	const char * CmdSetCPUBoost;
	const char * CmdSetGPUBoost;
	const char * CmdGetCurrentMode;
	const char * CmdSetCurrentMode;
	unsigned * ModeToHexMap;
	enum AWCCMode_t * HexToModeMap;
} static Internal = {
	 .Prefix = "AMWW",

	.CmdGetCPUBoost      =   "\\_SB.%s.WMAX 0 0x14 {0x0c, 0x32, 0x00, 0x00}"    ,
	.CmdGetGPUBoost      =   "\\_SB.%s.WMAX 0 0x14 {0x0c, 0x33, 0x00, 0x00}"    ,
	.CmdSetCPUBoost      =   "\\_SB.%s.WMAX 0 0x15 {0x02, 0x32, 0x%02x, 0x00}"  ,
	.CmdSetGPUBoost      =   "\\_SB.%s.WMAX 0 0x15 {0x02, 0x33, 0x%02x, 0x00}"  ,
	.CmdGetCurrentMode   =   "\\_SB.%s.WMAX 0 0x14 {0x0b, 0x00, 0x00, 0x00}"    ,
	.CmdSetCurrentMode   =   "\\_SB.%s.WMAX 0 0x15 {0x01, 0x%02x, 0x00, 0x00}"  ,

	.ModeToHexMap = (unsigned []) {
		[AWCCModeBalanced]       = 0xa0  ,
		[AWCCModePerformance]    = 0xa1  ,
		[AWCCModeQuiet]          = 0xa3  ,
		[AWCCModeBatterySaver]   = 0xa5  ,
		[AWCCModeG]              = 0xab  ,
		// [AWCCModeManual]         = 0x00  ,
	},

	.HexToModeMap = (enum AWCCMode_t []) {
		[0xa0]       = AWCCModeBalanced  ,
		[0xa1]    = AWCCModePerformance  ,
		[0xa3]          = AWCCModeQuiet  ,
		[0xa5]   = AWCCModeBatterySaver  ,
		[0xab]              = AWCCModeG  ,
		// [0x00]         = AWCCModeManual  ,
	},
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

enum AWCCMode_t GetMode (void)
{
	static _Thread_local char cmd [256];
	snprintf (cmd, sizeof (cmd), Internal.CmdGetCurrentMode, Internal.Prefix);
	AWCCACPI.Execute (cmd);

	thrd_sleep (& (struct timespec) {.tv_nsec = 1E8}, NULL);
	const char * response = AWCCACPI.Read ();
	unsigned int response_ulong;
	int status = sscanf (response, "0x%2x", & response_ulong);

	if (EOF == status) {
		fputs ("Failed to get the current mode.\n", stderr);
		exit (-1);
	}

	return Internal.HexToModeMap [response_ulong];
}

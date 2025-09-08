# include "AWCCValueChecker.h"

# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>

static void AssertFan (enum AWCCFan_t fan);
static void AssertBoost (AWCCBoost_t boost);
static void AssertMode (enum AWCCMode_t mode);

const struct AWCCValueChecker_t AWCCValueChecker = {
	.AssertFan = & AssertFan,
	.AssertBoost = & AssertBoost,
	.AssertMode = & AssertMode,
};

void AssertFan (enum AWCCFan_t fan)
{
	if (AWCCFanCPU != fan && AWCCFanGPU != fan) {
		fprintf (stderr, "Illegal fan value %d.\n", fan);
		exit (EXIT_FAILURE);
	}
}

void AssertBoost (AWCCBoost_t boost)
{
	if (AWCCFanBoostMin > boost || AWCCFanBoostMax < boost) {
		fprintf (stderr, "Illegal fan boost value %d.\n", boost);
		exit (EXIT_FAILURE);
	}
}

void AssertMode (enum AWCCMode_t mode)
{
	if (
		AWCCModeQuiet != mode &&
		AWCCModeBatterySaver != mode &&
		AWCCModeBalanced != mode &&
		AWCCModePerformance != mode &&
		AWCCModeG != mode
	) {
		fprintf (stderr, "Illegal mode value %d.\n", mode);
		exit (EXIT_FAILURE);
	}
}

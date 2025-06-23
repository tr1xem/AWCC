# include <stdio.h>
# include "AWCC.h"

int main (void) {
	AWCC.Initialize ();
	printf ("cpu: %d, gpu: %d\n", AWCC.GetCpuBoost (), AWCC.GetGpuBoost ());
}

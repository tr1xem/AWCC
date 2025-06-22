# include <stdio.h>
# include "AWCC.h"

int main (void) {
	puts ("hello");
	printf ("cpu: %d, gpu: %d\n", AWCC.GetCpuBoost (), AWCC.GetGpuBoost ());
}

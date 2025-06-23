# include <stdio.h>
# include "AWCC.h"

int main (void)
{
	AWCC.Initialize ();

	printf ("Current Mode: %s\n", AWCC.GetModeName (AWCC.GetMode ()));
}

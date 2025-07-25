# include <stdio.h>
# include <stdlib.h>
# include <threads.h>
# include <stddef.h>
# include <assert.h>

# include "AWCC.h"
# include "AWCCAutoBoost.h"
# include "AWCCConfig.h"
# include "AWCCControl.h"

# ifdef __STDC_NO_THREADS__
# error this tool currently depends on threads.h
# endif // __STDC_NO_THREADS__

int main (void)
{
	AWCC.Initialize ();

	if (0) {
		while (1) {
			if (AWCCPowerStateBAT == AWCC.PowerState ()) {
				puts ("BAT");
			}
			else {
				puts ("AC");
			}
			thrd_sleep (& (struct timespec) {.tv_sec = 2}, NULL);
		}
	}

	if (0) {
		enum AWCCMode_t modes [] = {
			AWCCModeQuiet,
			AWCCModeBatterySaver,
			AWCCModeBalanced,
			AWCCModePerformance,
			AWCCModeG,
		};

		for (size_t i = 0; i < sizeof (modes) / sizeof (* modes); i++) {
			AWCC.SetMode (modes [i]);
			const enum AWCCMode_t mode = AWCC.GetMode ();
			assert (mode == modes [i]);
			printf ("::: Current Mode: %s\n", AWCC.GetModeName (mode));
			system ("awcc qm");
			thrd_sleep (& (struct timespec) {.tv_sec = 2}, NULL);
		}

		goto exit;
	}

	if (0) {
		for (int i = 0; i <= 87; i += 10) {
			AWCC.SetFanBoost (AWCCFanCPU, i);
			AWCC.SetFanBoost (AWCCFanGPU, i + 2);
			AWCCBoost_t boostCpu = AWCC.GetFanBoost (AWCCFanCPU);
			AWCCBoost_t boostGpu = AWCC.GetFanBoost (AWCCFanGPU);

			assert (i == boostCpu && i + 2 == boostGpu);

			printf ("%d = %d, %d = %d\n", i, boostCpu, i + 2, boostGpu);
			thrd_sleep (& (struct timespec) {.tv_sec = 3}, NULL);
		}

		goto exit;
	}

	if (0) {
		while (1) {
			printf ("cpu temp: %d\ngpu temp: %d\n\n", AWCC.GetCpuTemperature (), AWCC.GetGpuTemperature ());
			printf ("cpu rpm: %d\ngpu rpm: %d\n\n", AWCC.GetCpuRpm (), AWCC.GetGpuRpm ());
			thrd_sleep (& (struct timespec) {.tv_nsec = 1E9 * 0.25}, NULL);
		}

		goto exit;
	}

	if (0) {
		while (1) {
			AWCC.SetCpuBoost (10);
			AWCC.SetGpuBoost (10);
			thrd_sleep (& (struct timespec) {.tv_sec = 3}, NULL);

			AWCCBoost_t boostCpu = AWCC.GetFanBoost (AWCCFanCPU);
			AWCCBoost_t boostGpu = AWCC.GetFanBoost (AWCCFanGPU);

			if (boostCpu != 10 || boostGpu != 10) {
				printf ("cpu boost: %d\ngpu boost: %d\n\n", boostCpu, boostGpu);
				goto exit;
			}
			else {
				puts ("...........................");
			}
		}
	}

	if (1) {
		struct AWCCConfig_t conf_ac = AWCCDefaultConfigAC ();
		struct AWCCConfig_t conf_bat = AWCCDefaultConfigBAT ();
		AWCCAutoBoost.Start (& conf_ac, & conf_bat, & AWCCSystemLoggerDefault, & AWCCControlDefault);
		goto exit;
	}

exit:
	AWCC.Deinitialize ();
}

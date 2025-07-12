# include "AWCCConfig.h"

const struct AWCCConfig_t AWCCDefaultConfig = {
	.TemperatureCheckInterval = 1,

	.MinTimeBeforeModeDown = 20,
	.ModeDownHysteresis = 5,
	.ModePendingTime = 5,

	.ModeIntervals = (struct AWCCModeInterval_t [4]) {
		{ .TemperatureRange = { .Min =  0, .Max = 39  }, .Mode = AWCCModeQuiet },
		{ .TemperatureRange = { .Min = 40, .Max = 59  }, .Mode = AWCCModeBatterySaver },
		{ .TemperatureRange = { .Min = 60, .Max = 79  }, .Mode = AWCCModeBalanced },
		{ .TemperatureRange = { .Min = 80, .Max = 100  }, .Mode = AWCCModePerformance },
		// { .TemperatureRange = { .Min = 90, .Max = 100 }, .Mode = AWCCModeG },
	},
	._ModeIntervalCount = 4,

	.FanConfigs = {
		[AWCCFanCPU] = {
			.UpBoostShift = 5,
			.UpBoostShiftTime = 10,
			.BoostDownHysteresis = 3,
			.MinTimeBeforeBoostDown = 15,
			.MinTimeAfterShiftDown = 5,
			.PendingTime = 2,
			.BoostIntervals = (struct AWCCBoostInterval_t [9]) {
				{ .TemperatureRange = { .Min =  0, .Max = 44  }, .Boost = 0   },
				{ .TemperatureRange = { .Min = 45, .Max = 49  }, .Boost = 30  },
				{ .TemperatureRange = { .Min = 50, .Max = 54  }, .Boost = 40  },
				{ .TemperatureRange = { .Min = 55, .Max = 59  }, .Boost = 50  },
				{ .TemperatureRange = { .Min = 60, .Max = 64  }, .Boost = 60  },
				{ .TemperatureRange = { .Min = 65, .Max = 69  }, .Boost = 70  },
				{ .TemperatureRange = { .Min = 70, .Max = 74  }, .Boost = 80  },
				{ .TemperatureRange = { .Min = 75, .Max = 79  }, .Boost = 90  },
				{ .TemperatureRange = { .Min = 80, .Max = 100 }, .Boost = 100 },
			},
			._BoostIntervalCount = 9
		},

		[AWCCFanGPU] = {
			.UpBoostShift = 5,
			.UpBoostShiftTime = 10,
			.BoostDownHysteresis = 3,
			.MinTimeBeforeBoostDown = 15,
			.MinTimeAfterShiftDown = 5,
			.PendingTime = 2,
			.BoostIntervals = (struct AWCCBoostInterval_t [9]) {
				{ .TemperatureRange = { .Min =  0, .Max = 44  }, .Boost = 0   },
				{ .TemperatureRange = { .Min = 45, .Max = 49  }, .Boost = 30  },
				{ .TemperatureRange = { .Min = 50, .Max = 54  }, .Boost = 40  },
				{ .TemperatureRange = { .Min = 55, .Max = 59  }, .Boost = 50  },
				{ .TemperatureRange = { .Min = 60, .Max = 64  }, .Boost = 60  },
				{ .TemperatureRange = { .Min = 65, .Max = 69  }, .Boost = 70  },
				{ .TemperatureRange = { .Min = 70, .Max = 74  }, .Boost = 80  },
				{ .TemperatureRange = { .Min = 75, .Max = 79  }, .Boost = 90  },
				{ .TemperatureRange = { .Min = 80, .Max = 100 }, .Boost = 100 },
			},
			._BoostIntervalCount = 9
		},
	},
};

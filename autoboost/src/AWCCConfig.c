# include "AWCCConfig.h"
#include "AWCC.h"

const struct AWCCConfig_t AWCCDefaultConfig = {
	.TemperatureCheckInterval = 1,

	.MinTimeBeforeModeDown = 20,
	.ModeDownHysteresis = 5,
	.ModePendingTime = 5,

	.ModeIntervals = (struct AWCCModeInterval_t [2]) {
		{ .TemperatureRange = { .Min =  0, .Max = 79  }, .Mode = AWCCModeBalanced },
		{ .TemperatureRange = { .Min = 80, .Max = 100  }, .Mode = AWCCModePerformance },
		// { .TemperatureRange = { .Min = 90, .Max = 100 }, .Mode = AWCCModeG },
	},
	._ModeIntervalCount = 2,

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

	.NoBoostConf = {
		.Mode = AWCCModeBalanced,
		.FanBoosts = (AWCCBoost_t [2]) {
			[AWCCFanCPU] = 0,
			[AWCCFanGPU] = 0,
		}
	}
};

const struct AWCCConfig_t AWCCDefaultConfigAC (void)
{
	return AWCCDefaultConfig;
}

struct {
	struct AWCCModeInterval_t ModeIntervalsBAT [1];
	struct AWCCBoostInterval_t BoostIntervalsBAT [6];
	int _ModeIntervalsBATCount;
	int _BoostIntervalsBATCount;
} static Internal = {
	.ModeIntervalsBAT = {
		{ .TemperatureRange = { .Min =  0, .Max = 100 }, .Mode = AWCCModeBatterySaver },
	},
	._ModeIntervalsBATCount = 1,

	.BoostIntervalsBAT = {
		{ .TemperatureRange = { .Min =  0, .Max = 59, }, .Boost = 0   },
		{ .TemperatureRange = { .Min = 60, .Max = 64  }, .Boost = 20  },
		{ .TemperatureRange = { .Min = 65, .Max = 69  }, .Boost = 40  },
		{ .TemperatureRange = { .Min = 70, .Max = 74  }, .Boost = 60  },
		{ .TemperatureRange = { .Min = 75, .Max = 79  }, .Boost = 80  },
		{ .TemperatureRange = { .Min = 80, .Max = 100 }, .Boost = 100 },
	},
	._BoostIntervalsBATCount = 6,
};

const struct AWCCConfig_t AWCCDefaultConfigBAT (void)
{
	struct AWCCConfig_t conf = AWCCDefaultConfig;

	conf.ModeIntervals = Internal.ModeIntervalsBAT;
	conf._ModeIntervalCount = Internal._ModeIntervalsBATCount;


	conf.FanConfigs [AWCCFanCPU].BoostIntervals = Internal.BoostIntervalsBAT;
	conf.FanConfigs [AWCCFanCPU]._BoostIntervalCount = Internal._BoostIntervalsBATCount;
	conf.FanConfigs [AWCCFanGPU].BoostIntervals = Internal.BoostIntervalsBAT;
	conf.FanConfigs [AWCCFanGPU]._BoostIntervalCount = Internal._BoostIntervalsBATCount;

	return conf;
}

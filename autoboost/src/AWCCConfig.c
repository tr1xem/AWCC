# include "AWCCConfig.h"

const struct AWCCConfig_t AWCCDefaultConfig = {
	.MinTimeBeforeModeDown = 10,

	.FanConfigs = (struct AWCCFanConfig_t []) {
		[AWCCFanCPU] = {
			.TemperatureCheckInterval = 1,
			.BoostDownHysteresis = 3,
			.MinTimeBeforeBoostDown = 10,
			.BoostIntervals = (struct AWCCBoostInterval_t []) {
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min =  0, .Max = 44  }, .Boost = 0   },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 45, .Max = 49  }, .Boost = 30  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 50, .Max = 54  }, .Boost = 40  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 55, .Max = 59  }, .Boost = 50  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 60, .Max = 64  }, .Boost = 60  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 65, .Max = 69  }, .Boost = 70  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 70, .Max = 74  }, .Boost = 80  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 75, .Max = 79  }, .Boost = 90  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 80, .Max = 100 }, .Boost = 100 },
			},
			._BoostIntervalCount = 9
		},
		[AWCCFanGPU] = {
			.TemperatureCheckInterval = 1,
			.BoostDownHysteresis = 3,
			.MinTimeBeforeBoostDown = 10,
			.BoostIntervals = (struct AWCCBoostInterval_t []) {
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min =  0, .Max = 44  }, .Boost = 0   },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 45, .Max = 49  }, .Boost = 30  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 50, .Max = 54  }, .Boost = 40  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 55, .Max = 59  }, .Boost = 50  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 60, .Max = 64  }, .Boost = 60  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 65, .Max = 69  }, .Boost = 70  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 70, .Max = 74  }, .Boost = 80  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 75, .Max = 79  }, .Boost = 90  },
				(struct AWCCBoostInterval_t) { .TemperatureRange = { .Min = 80, .Max = 100 }, .Boost = 100 },
			},
			._BoostIntervalCount = 9
		},
	},

	.ModeIntervals = (struct AWCCModeInterval_t []) {
		(struct AWCCModeInterval_t) { .TemperatureRange = { .Min =  0, .Max = 29  }, .Mode = AWCCModeQuiet },
		(struct AWCCModeInterval_t) { .TemperatureRange = { .Min = 30, .Max = 44  }, .Mode = AWCCModeBatterySaver },
		(struct AWCCModeInterval_t) { .TemperatureRange = { .Min = 45, .Max = 54  }, .Mode = AWCCModeBalanced },
		(struct AWCCModeInterval_t) { .TemperatureRange = { .Min = 55, .Max = 89  }, .Mode = AWCCModePerformance },
		(struct AWCCModeInterval_t) { .TemperatureRange = { .Min = 90, .Max = 100 }, .Mode = AWCCModeG },
	},
	._ModeIntervalCount = 5
};

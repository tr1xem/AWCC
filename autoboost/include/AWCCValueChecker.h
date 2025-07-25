# pragma once

# include "AWCC.h"

struct AWCCValueChecker_t {
	void (* AssertFan) (enum AWCCFan_t fan);
	void (* AssertBoost) (AWCCBoost_t boost);
	void (* AssertMode) (enum AWCCMode_t mode);
} extern const AWCCValueChecker;

# ifndef __AWCC_VALUECHECKER_H
# define __AWCC_VALUECHECKER_H

# include "AWCC.h"

struct AWCCValueChecker_t {
	void (* AssertFan) (enum AWCCFan_t fan);
	void (* AssertBoost) (AWCCBoost_t boost);
	void (* AssertMode) (enum AWCCMode_t mode);
} extern const AWCCValueChecker;

# endif // __AWCC_VALUECHECKER_H

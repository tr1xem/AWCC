# ifndef __AWCC_AUTOBOOST_H
# define __AWCC_AUTOBOOST_H

# include "AWCC.h"
# include "AWCCConfig.h"
# include "AWCCSystemLogger.h"

struct AWCCAutoBoost_t {
	void (* Start) (const struct AWCCConfig_t *, const struct AWCCSystemLogger_t *);
} extern const AWCCAutoBoost;

# endif // __AWCC_AUTOBOOST_H

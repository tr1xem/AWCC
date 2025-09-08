# ifndef __AWCC_AUTOBOOST_H
# define __AWCC_AUTOBOOST_H

# include "AWCCConfig.h"
# include "AWCCControl.h"
# include "AWCCSystemLogger.h"

struct AWCCAutoBoost_t {
	void (* Start) (const struct AWCCConfig_t *, const struct AWCCConfig_t *, const struct AWCCSystemLogger_t *, const struct AWCCControl_t *);
} extern const AWCCAutoBoost;

# endif // __AWCC_AUTOBOOST_H

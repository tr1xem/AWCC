# ifndef __AWCCAUTOBOOST_H
# define __AWCCAUTOBOOST_H

# include "AWCC.h"
# include "AWCCConfig.h"
# include "AWCCSystemLogger.h"

struct AWCCBoost_t {
	void (* Start) (const struct AWCCConfig_t *, const struct AWCCSystemLogger_t *);
} extern AWCCBoost;

# endif // __AWCCAUTOBOOST_H

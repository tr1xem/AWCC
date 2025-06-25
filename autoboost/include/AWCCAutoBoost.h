# ifndef __AWCCAUTOBOOST_H
# define __AWCCAUTOBOOST_H

# include "AWCC.h"
# include "AWCCConfig.h"

struct AWCCBoost_t {
	void (* Start) (struct AWCCConfig_t *);
} extern AWCCBoost;

# endif // __AWCCAUTOBOOST_H

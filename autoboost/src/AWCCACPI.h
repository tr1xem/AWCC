# ifndef __AWCCACPI_H
# define __AWCCACPI_H

// https://docs.kernel.org/wmi/devices/alienware-wmi.html
// https://docs.kernel.org/admin-guide/laptops/alienware-wmi.html

# include "AWCC.h"

struct AWCCACPI_t {
	void (* Initialize) (void);
	enum AWCCMode_t (* GetMode) (void);
	void (* SetMode) (enum AWCCMode_t);

	AWCCBoost_t (* GetFanBoost) (enum AWCCFan_t);
	void (* SetFanBoost) (enum AWCCFan_t, AWCCBoost_t);
	AWCCTemperature_t (* GetFanTemperature) (enum AWCCFan_t);

} extern const AWCCACPI;

# endif // __AWCCACPI_H

# ifndef __AWCCACPI_H
# define __AWCCACPI_H

// https://docs.kernel.org/wmi/devices/alienware-wmi.html
// https://docs.kernel.org/admin-guide/laptops/alienware-wmi.html

struct AWCCACPI_t {
	void (* Initialize) (void);
	void (* Execute) (const char *);
	const char * (* Read) (void);
} extern const AWCCACPI;

# endif // __AWCCACPI_H

# ifndef __AWCCACPI_H
# define __AWCCACPI_H

struct AWCCACPI_t {
	void (* Initialize) (void);
	void (* Execute) (const char *);
	const char * (* Read) (void);
} extern const AWCCACPI;

# endif // __AWCCACPI_H

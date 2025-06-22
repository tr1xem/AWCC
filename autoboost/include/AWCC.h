# ifndef AWCC_H
# define AWCC_H

enum AWCCMode {
	AWCCModeQuiet,
	AWCCModeBatterySaver,
	AWCCModeBalanced,
	AWCCModePerformance,
	AWCCModeG
};

struct AWCC_t {
	void (* Initialize) (void);
	int (* GetCpuBoost) (void);
	int (* GetGpuBoost) (void);
	void (* SetCpuBoost) (int);
	void (* SetGpuBoost) (int);
	void (* SetMode) (enum AWCCMode);
	enum AWCCMode (* GetMode) (void);
	const char * (* GetModeName) (enum AWCCMode);
	void (* Deinitialize) (void);
} extern const AWCC;

# endif // AWCC_H

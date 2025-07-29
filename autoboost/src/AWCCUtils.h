# ifndef __AWCC_UTILS_H
# define __AWCC_UTILS_H

static inline int AWCCUtils_MinInt (int x, int y) { return x < y ? x : y; }
static inline int AWCCUtils_MaxInt (int x, int y) { return x > y ? x : y; }

// struct AWCCUtils_t {
// } extern const AWCCUtils;

# endif // __AWCC_UTILS_H

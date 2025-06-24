# include "AWCCAutoBoost.h"

// WARN:
// 1) be careful handling UpBoostShiftTime and MinTimeBeforeBoostDown
// 2) be careful handling jumps over multiple boost intervals, go one by one
//    respecting MinTimeBeforeBoostDown

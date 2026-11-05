/// origin binary
#include <stdlib.h>
#include "FirePlace/FireWorks/FMemHooks.h"

/// 0x08db3a94
void FireFreeAlignedNoTracking(void *pBlock) {
    free(pBlock);
}

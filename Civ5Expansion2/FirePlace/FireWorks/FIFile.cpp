/// origin binary
#include "FirePlace/FireWorks/FIFile.h"

/// 0x08da87ea
FIFileSystem& FIFileSystem::GetInstance() {
    return *ms_pInstance;
}

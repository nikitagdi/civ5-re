/// origin binary
#include "FirePlace/FireWorks/FILogFile.h"

/// 0x08d8940c
FILogFileMgr& FILogFileMgr::GetInstance() {
    return *ms_pInstance;
}

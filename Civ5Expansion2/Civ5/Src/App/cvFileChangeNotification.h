/// origin guess
#pragma once
#include "Civ5/Src/App/cvPath.h"
#include <Windows.h>

/// size 0x434
class cvFileChangeNotification {
public:
    /// 0x08767274
    cvFileChangeNotification();
    /// 0x087672be
    ~cvFileChangeNotification();
    /// 0x0876733c
    bool SetFileToMonitor(cvPath const& a1, unsigned long a2);
    /// 0x08767480
    UNK_RET FolderChangeNotify(void* a1, unsigned char a2);
    /// 0x087674f6
    bool HasChanged() const;
    /// 0x08767502
    UNK_RET ResetChange();

    int m_hNotification;       ///< +0x0
    int field_4;               ///< +0x4
    bool m_bChanged;           ///< +0x8
    char unk_9[0x3];           ///< +0x9
    cvPath m_Path;            ///< +0xc
    FILETIME m_ftLastWrite;    ///< +0x42c
};

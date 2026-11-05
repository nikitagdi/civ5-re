/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"

/// size ?
class PreGamePlayerInfo {
public:
    /// 0x08988206
    PreGamePlayerInfo();
    /// 0x08988350
    UNK_RET Init(PlayerTypes a1);
    /// 0x08988c06
    UNK_RET Commit() const;
};

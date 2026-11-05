/// origin guess
#pragma once
#include "Civ5/Src/App/UI/UIStringBase.h"

/// size ?
/// vtable 0x0a2f9a30
class UIString16384 : public UIStringBase {
public:
    /// 0x086d78ac
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x086d78b2
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x0a3d17e4
    static UNK_DATA s_Cache[0x1c];
};

extern "C" {
    /// 0x0a3d1800
    extern UNK_DATA g_pUIManager[0x4];
}

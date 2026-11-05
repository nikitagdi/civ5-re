/// origin guess
#pragma once
#include "FirePlace/FireWorks/FSoundBase.h"
#include "FirePlace/FireWorks/FString.h"

class F2DSoundScriptData;
class FSoundBase;
class FSoundInit;

/// size ?
/// vtable 0x0a303740
class FSound2D : public FSoundBase {
public:
    /// 0x08d315b0
    FSound2D();
    /// 0x08d319ae
    virtual UNK_RET ApplyScript();
    /// 0x08d3172e
    virtual UNK_RET Clear();
    /// 0x08d31caa
    virtual UNK_RET ComputePriorityScoreVersus(FSoundBase const* a1) const;
    /// 0x08d31826
    virtual UNK_RET Destroy();
    /// 0x08d317ea
    virtual UNK_RET Init(FSoundInit const& a1);
    /// 0x08d318dc
    virtual UNK_RET InitSoundType();
    /// 0x08d31764
    virtual UNK_RET InsertSoundIntoAudioManager();
    /// 0x08d3185c
    virtual UNK_RET Load(FStringA* a1);
    /// 0x08d31cbe
    virtual UNK_RET NotifyAudioManagerSoundUnloaded();
    /// 0x08d316fe
    virtual UNK_RET RemoveSoundWithLowestPriority();
    /// 0x08d31798
    virtual UNK_RET Update(float a1);
    /// 0x08d31c12
    virtual UNK_RET ApplyScriptUpdate();
    /// 0x08d31670
    /// deleting 0x08d31648
    virtual ~FSound2D();
    /// 0x08d317fe
    virtual UNK_RET Term();
    /// 0x08d318a8
    virtual UNK_RET DoLoad(FStringA* a1);
    /// 0x08d318f6
    UNK_RET SetPan(float a1);
    /// 0x08d31982
    UNK_RET GetPan(float& a1) const;
    /// 0x08d3199c
    UNK_RET SetScript(F2DSoundScriptData const* a1);
    /// 0x08d319e6
    UNK_RET ApplyScriptPan();
    /// 0x0ff85df8
    static UNK_DATA s_ObjPool[0x18];
};

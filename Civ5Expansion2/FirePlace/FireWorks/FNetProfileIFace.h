/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FNetProfileCallbacksIFace;
class ProfilePermissions;

/// size ?
/// vtable 0x0a305010
class FNetProfileIFace {
public:
    /// 0x08d6cea0
    FNetProfileIFace(int a1, FNetProfileCallbacksIFace* a2, int a3);
    /// 0x08d6cffa
    /// deleting 0x08d6cfd2
    virtual ~FNetProfileIFace() = 0;
    /// 0x08d6d140
    virtual UNK_RET Init();
    /// 0x08d6d1b8
    virtual UNK_RET Update();
    /// 0x08d6d1e6
    virtual UNK_RET Shutdown();
    /// 0x08d6d64e
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x08d6d652
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08d6d656
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08d6d658
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08d6d660
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x08d6d664
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x08d6d668
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x08d6d66e
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x08d6d674
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x08d6d678
    virtual UNK_RET vf_14(UNK_PARAMS);
    /// 0x08d6d67c
    virtual UNK_RET vf_15(UNK_PARAMS);
    /// 0x08d6d680
    virtual UNK_RET vf_16(UNK_PARAMS);
    /// 0x08d6d684
    virtual UNK_RET vf_17(UNK_PARAMS);
    /// 0x08d6d688
    virtual UNK_RET vf_18(UNK_PARAMS);
    /// 0x08d6d68c
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x08d6d690
    virtual UNK_RET vf_20(UNK_PARAMS);
    /// 0x08d6d694
    virtual UNK_RET vf_21(UNK_PARAMS);
    /// 0x08d6d698
    virtual UNK_RET IsConnected(bool a1);
    /// 0x08d6d69e
    virtual UNK_RET IsPermitted(ProfilePermissions a1);
    /// 0x08d6d6a4
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x08d6d258
    virtual UNK_RET ResetDetails();
    /// 0x08d6d36a
    virtual UNK_RET CreateStatManager();
    /// 0x08d6d4f2
    virtual UNK_RET CreateAchievementManager();
    /// 0x08d6d362
    UNK_RET GetInitializationState() const;
    /// 0x08d6d398
    UNK_RET StatRequestComplete(int a1, bool a2);
    /// 0x08d6d3de
    UNK_RET CacheProfileName(int a1, FStringA const& a2);
    /// 0x08d6d436
    UNK_RET GetCachedProfileName(int a1);
};

namespace FNetProfileFactory {
/// 0x08d6d514
UNK_RET CreateProfileMgr(int a1, FNetProfileCallbacksIFace* a2, int a3);
}

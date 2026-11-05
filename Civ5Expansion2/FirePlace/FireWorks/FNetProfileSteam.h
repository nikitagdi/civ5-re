/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetProfileIFace.h"

class FNetProfileCallbacksIFace;
class ProfilePermissions;

/// size ?
/// vtable 0x0a3053c0
class FNetProfileSteam : public FNetProfileIFace {
public:
    /// 0x08d78238
    FNetProfileSteam(int a1, FNetProfileCallbacksIFace* a2, int a3);
    /// 0x08d782a6
    /// deleting 0x08d7827e
    virtual ~FNetProfileSteam();
    /// 0x08d782ba
    virtual UNK_RET Init();
    /// 0x08d782c8
    virtual UNK_RET IsConnected(bool a1);
    /// 0x08d782fe
    virtual UNK_RET IsPermitted(ProfilePermissions a1);
    /// 0x08d782c6
    virtual UNK_RET Shutdown();
    /// 0x08d782c0
    virtual UNK_RET Update();
};

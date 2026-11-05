/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetProfileCallbacksIFace.h"

class NetProxy;

/// size ?
/// vtable 0x0a2fc1d0
class NetProxyProfileCallbacks : public FNetProfileCallbacksIFace {
public:
    /// 0x0873a4fc
    NetProxyProfileCallbacks(NetProxy* a1);
    /// 0x0873cba0
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x0873cba2
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x0873cbb6
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x0873cbb8
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x0873cbba
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x0873cbbc
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x0873cbbe
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x0873cbc0
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x0873cbc2
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x0873a520
    virtual UNK_RET UseComputerNameByDefault();
};

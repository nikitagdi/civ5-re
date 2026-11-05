/// origin binary
#pragma once

class FNetMessage;

/// size ?
/// vtable 0x0a303fa8
class FNetMessageFactory {
public:
    /// 0x08d4cbe0
    FNetMessageFactory();
    /// 0x08d4cd5c
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08d4cd8c
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08d4cc20
    UNK_RET AddCreator(FNetMessage* (*a1)(), int a2);
    /// 0x08d4cc78
    UNK_RET CreateMessage(int a1);
    /// 0x0ff87be8
    static UNK_DATA m_pInst[0x4];
};

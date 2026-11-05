/// origin guess
#pragma once

class FSessionSetup;

/// size ?
/// vtable 0x0a305500
class FSecureSession {
public:
    /// 0x08d7a048
    FSecureSession(int a1);
    /// 0x08d7a0ee
    /// deleting 0x08d7a0f0
    virtual ~FSecureSession();
    /// 0x08d7a104
    virtual UNK_RET Host(unsigned int a1, FSessionSetup const* a2);
    /// 0x08d7a10a
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08d7a110
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x08d7a116
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x08d7a11c
    virtual UNK_RET End();
    /// 0x08d7a122
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08d7a124
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08d7a12a
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x08d7a130
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x08d7a132
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x08d7a138
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x08d7a13e
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x08d7a142
    virtual UNK_RET vf_14(UNK_PARAMS);
    /// 0x08d7a146
    virtual UNK_RET vf_15(UNK_PARAMS);
    /// 0x08d7a14a
    virtual UNK_RET ValidateClient(unsigned int a1, unsigned char const* a2);
    /// 0x08d7a150
    virtual UNK_RET HandleValidationResponse(int a1, unsigned int a2, unsigned char const* a3);
    /// 0x08d7a156
    virtual UNK_RET vf_18(UNK_PARAMS);
    /// 0x08d7a15c
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x08d7a15e
    virtual UNK_RET vf_20(UNK_PARAMS);
    /// 0x08d7a1a8
    virtual UNK_RET vf_21(UNK_PARAMS);
    /// 0x08d7a1aa
    virtual UNK_RET vf_22(UNK_PARAMS);
    /// 0x08d7a1b2
    virtual UNK_RET vf_23(UNK_PARAMS);
    /// 0x08d7a1b8
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x08d7a1bc
    virtual UNK_RET vf_25(UNK_PARAMS);
    /// 0x08d7a1c0
    virtual UNK_RET vf_26(UNK_PARAMS);
    /// 0x08d7a1c4
    virtual UNK_RET GetValidationData() const;
    /// 0x08d7a1c8
    virtual UNK_RET GetValidationDataLen() const;
    /// 0x08d7a1cc
    virtual UNK_RET vf_29(UNK_PARAMS);
    /// 0x08d7a072
    UNK_RET GetSession() const;
};

namespace FSecureSessionFactory {
/// 0x08d7a0a2
UNK_RET CreateSecureSession(int a1, int a2);
}

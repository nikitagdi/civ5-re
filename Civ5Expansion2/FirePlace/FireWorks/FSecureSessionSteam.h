/// origin guess
#pragma once
#include "FirePlace/FireWorks/FSecureSession.h"

class FSessionSetup;

/// size ?
/// vtable 0x0a3055d0
class FSecureSessionSteam : public FSecureSession {
public:
    /// 0x08d7acd0
    FSecureSessionSteam(int a1);
    /// 0x08d7ae34
    /// deleting 0x08d7ae20
    virtual ~FSecureSessionSteam();
    /// 0x08d7aed0
    virtual UNK_RET End();
    /// 0x08d7af3e
    virtual UNK_RET GetValidationData() const;
    /// 0x08d7af90
    virtual UNK_RET GetValidationDataLen() const;
    /// 0x08d7b05c
    virtual UNK_RET HandleValidationResponse(int a1, unsigned int a2, unsigned char const* a3);
    /// 0x08d7ae36
    virtual UNK_RET Host(unsigned int a1, FSessionSetup const* a2);
    /// 0x08d7afe2
    virtual UNK_RET ValidateClient(unsigned int a1, unsigned char const* a2);
    /// 0x08d7b114
    virtual UNK_RET vf_15(UNK_PARAMS);
};

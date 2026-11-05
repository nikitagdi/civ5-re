/// origin guess
#pragma once
#include "FirePlace/FireWorks/FTimedTriggerEffect.h"

class FClassId;
class FContext;
class FXmlElement;

/// size ?
/// vtable 0x0a2fcea0
class cvParticleEffectTrigger : public FTimedTriggerEffect {
public:
    /// 0x087febcc
    cvParticleEffectTrigger();
    /// 0x087fed02
    /// deleting 0x087fec2a
    /// thunk 0x087fec52
    /// thunk 0x087fec7e
    /// thunk 0x087fecaa
    /// thunk 0x087fecd6
    /// thunk 0x087fed16
    /// thunk 0x087fed3a
    /// thunk 0x087fed5e
    /// thunk 0x087fed82
    virtual ~cvParticleEffectTrigger();
    /// 0x087fedee
    /// thunk 0x087fee78
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x087feda6
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x087feea4
    virtual UNK_RET fired(FContext& a1);
    /// 0x087ff364
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x087ff384
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x087fef34
    UNK_RET OnFired(unsigned int a1, FContext& a2);
    /// 0x0a7f4148
    static UNK_DATA ms_kInstructionCache[0xc4];
};

extern "C" {
    /// 0x0a7f46d0
    extern UNK_DATA g_cvUnitSimSystem[0x5b0];
}

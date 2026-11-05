/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIGrannyTimedTrigger.h"
#include "FirePlace/FireWorks/FISoundEventName.h"
#include "FirePlace/FireWorks/FTimedTrigger.h"

class FClassId;
class FContext;
class FXmlElement;

/// size ?
/// vtable 0x0a3021a0
class FTimedTriggerSound : public FTimedTrigger, public FIGrannyTimedTrigger, public FISoundEventName {
public:
    /// 0x08ce1214
    FTimedTriggerSound();
    /// 0x08ce13a2
    /// deleting 0x08ce129e
    /// thunk 0x08ce12c6
    /// thunk 0x08ce12f2
    /// thunk 0x08ce131e
    /// thunk 0x08ce134a
    /// thunk 0x08ce1376
    /// thunk 0x08ce13b6
    /// thunk 0x08ce13da
    /// thunk 0x08ce13fe
    /// thunk 0x08ce1422
    /// thunk 0x08ce1446
    virtual ~FTimedTriggerSound();
    /// 0x08ce1524
    /// thunk 0x08ce1610
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08ce163c
    /// thunk 0x08ce16fa
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08ce149a
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08ce188a
    virtual UNK_RET fired(FContext& a1);
    /// 0x08ce1a78
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce1a98
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08ce177a
    /// thunk 0x08ce1782
    virtual UNK_RET GetEventName() const;
    /// 0x08ce19ee
    /// thunk 0x08ce19fe
    virtual UNK_RET SetEventCode(int a1);
    /// 0x08ce1a0a
    /// thunk 0x08ce1a16
    virtual UNK_RET GetEventCode() const;
    /// 0x08ce1a1e
    /// thunk 0x08ce1a26
    virtual UNK_RET GetGrannyBoneName() const;
    /// 0x08a27080
    virtual UNK_RET vf_20(UNK_PARAMS);
    /// 0x08ce146a
    UNK_RET EnableTriggerSounds(bool a1);
    /// 0x08ce1484
    UNK_RET IsTriggerSoundsEnabled();
    /// 0x08ce1726
    UNK_RET SetEventName(char const* a1);
    /// 0x08ce178a
    UNK_RET RefreshSound();
    /// 0x08ce17a0
    UNK_RET BindSound();
    /// 0x08ce1a2e
    UNK_RET SetBoneName(char const* a1);
    /// 0x08ce1a70
    UNK_RET GetBoneName() const;
    /// 0x0a3129d4
    static UNK_DATA s_enabled[0x1];
};

namespace FireGrafix {
/// 0x0a3129d8
extern UNK_DATA g_DDFormats[0x3e8];
/// 0x0a312dc0
extern UNK_DATA g_VertexFormatList[0xe8];
}

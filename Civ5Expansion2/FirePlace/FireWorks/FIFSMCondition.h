/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIUniqueId.h"
#include "FirePlace/FireWorks/FIUnknown.h"
#include "FirePlace/FireWorks/FIXmlSerialize.h"

class FContext;
class FXmlElement;

/// size 0x18
/// vtable 0x0a305be0
class FIFSMCondition : public FIXmlSerialize, public FIUniqueId, public FIUnknown {
public:
    int m_iUniqueId;          ///< +0xc
    float m_fOrder;           ///< +0x10
    char* m_pDescription;     ///< +0x14
    /// 0x08dad064
    FIFSMCondition();
    /// 0x08dae7b2
    /// deleting 0x08dad0a2
    /// thunk 0x08dad0e8
    /// thunk 0x08dad10c
    /// thunk 0x08dad130
    /// thunk 0x08dad170
    virtual ~FIFSMCondition();
    /// 0x08dad1b0
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dad1ee
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08dae916
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dae936
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// pure
    virtual bool evaluate(FContext& a1) = 0;
    /// 0x08dad2b6
    virtual float get_order() const;
    /// 0x08dad2be
    virtual void set_order(float a1);
    /// 0x08dad230
    virtual UNK_RET set_description(char const* a1);
    /// 0x08dad286
    virtual char const* get_description() const;
    /// 0x08dad28e
    /// thunk 0x08dad296
    virtual UNK_RET GetUniqueId() const;
    /// 0x08dad29e
    /// thunk 0x08dad2aa
    virtual UNK_RET SetUniqueId(int a1);
};

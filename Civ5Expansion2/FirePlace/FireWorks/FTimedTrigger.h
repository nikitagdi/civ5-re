/// origin binary
#pragma once
#include "FirePlace/FireWorks/FITimedTrigger.h"
#include "FirePlace/FireWorks/FITimedTriggerEdit.h"
#include "FirePlace/FireWorks/FIXmlSerialize.h"

class FClassId;
class FContext;
class FXmlElement;

/// size ?
/// vtable 0x0a306860
class FTimedTrigger : public FITimedTrigger, public FITimedTriggerEdit, public FIXmlSerialize {
public:
    char unk_10[0x10];    ///< +0x10
    /// 0x08dca8e0
    FTimedTrigger();
    /// 0x08dca92c
    FTimedTrigger(float a1, int a2);
    /// 0x08dca9fe
    /// deleting 0x08dca97e
    /// thunk 0x08dca992
    /// thunk 0x08dca9b6
    /// thunk 0x08dca9da
    /// thunk 0x08dcaa00
    /// thunk 0x08dcaa02
    /// thunk 0x08dcaa04
    virtual ~FTimedTrigger();
    /// 0x08dcac24
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08dcae6e
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dcae8e
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dcaa06
    virtual UNK_RET get_time() const;
    /// 0x08dcaa0e
    virtual UNK_RET get_duration() const;
    /// 0x08dcaa16
    virtual UNK_RET get_repeat_count() const;
    /// 0x08dcaa1e
    virtual UNK_RET fired(FContext& a1);
    /// 0x08dcacbe
    /// thunk 0x08dcacc6
    virtual UNK_RET GetUniqueId() const;
    /// 0x08dcacce
    /// thunk 0x08dcacda
    virtual UNK_RET SetUniqueId(int a1);
    /// 0x08dcaa78
    /// thunk 0x08dcab1e
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dcab4a
    /// thunk 0x08dcabf8
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08dcaa20
    /// thunk 0x08dcaa30
    virtual UNK_RET set_time(float a1);
    /// 0x08dcaa40
    /// thunk 0x08dcaa50
    virtual UNK_RET set_duration(float a1);
    /// 0x08dcaa60
    /// thunk 0x08dcaa6c
    virtual UNK_RET set_repeat_count(int a1);
};

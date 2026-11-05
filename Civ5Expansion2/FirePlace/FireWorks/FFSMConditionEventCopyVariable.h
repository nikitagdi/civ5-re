/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionVariable.h"
#include "FirePlace/FireWorks/FIConditionEvent.h"
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FClassId;
class FContext;
class FXmlElement;

/// size 0x7c
/// vtable 0x0a306330
class FFSMConditionEventCopyVariable : public FIFSMCondition, public FIConditionEvent {
public:
    class EmptyCondition;
    /// size 0x30
    /// vtable 0x0a302d40
    class EmptyCondition : public FFSMConditionVariable {
    public:
        /// absent
        virtual ~EmptyCondition();
        /// 0x08cee42c
        virtual bool evaluate(FContext& a1);
    };
    EmptyCondition m_source;  ///< +0x1c
    EmptyCondition m_target;  ///< +0x4c
    /// 0x08db06a2
    /// deleting 0x08db0744
    virtual ~FFSMConditionEventCopyVariable();
    /// 0x08dad978
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dada4a
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08db07ee
    virtual bool evaluate(FContext& a1);
    /// 0x08db064c
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08db066c
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dad862
    /// thunk 0x08dad8a4
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08dad8e8
    /// thunk 0x08dad94c
    virtual UNK_RET fire_event(FContext& a1) const;
};

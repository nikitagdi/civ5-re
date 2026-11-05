/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIXmlSerialize.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FTimedTriggerSystem.h"
#include <functional>
#include <hash_map>
#include <unordered_map>
#include <utility>

class FClassId;
class FXmlElement;

/// size ?
/// vtable 0x0a302590
class FGrannyTimedTriggerSystem : public FTimedTriggerSystem, public FIXmlSerialize {
public:
    /// 0x08ce2d68
    FGrannyTimedTriggerSystem();
    /// 0x08ce2e46
    /// deleting 0x08ce2dd2
    /// thunk 0x08ce2e22
    /// thunk 0x08ce2e8e
    virtual ~FGrannyTimedTriggerSystem();
    /// 0x08ce3676
    virtual UNK_RET FinalizeTriggers();
    /// 0x08ce2ede
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08ce39c0
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce39e0
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08ce2f4a
    /// thunk 0x08ce33ac
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08ce33d8
    /// thunk 0x08ce3628
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08ce3654
    UNK_RET GetUniqueTrackId(std::unordered_map<int, FStringA, stdext::hash_compare<int, std::equal_to<int> >, stdext::hash_compare<int, std::equal_to<int> >, FSTL_Tagged_Allocator<std::pair<int const, FStringA>, 1u, 34u, 4u> > const& a1);
    /// 0x08ce379a
    UNK_RET GetTriggersForEventCode(int a1);
};

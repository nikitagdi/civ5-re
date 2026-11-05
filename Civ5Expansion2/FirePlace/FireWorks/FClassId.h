/// origin binary
#pragma once
#include "FirePlace/FireWorks/FUncopyable.h"
#include <memory>
#include <utility>
#include <vector>

class FClassId;

/// size ?
class FClassId {
public:
    class Meta;
    class Info;
    /// size ?
    /// vtable 0x0a306798
    class Meta : public FUncopyable {
    public:
        /// 0x08dc4608
        Meta();
        /// 0x08dc47ee
        virtual UNK_RET vf_00(UNK_PARAMS);
        /// 0x08dc480e
        virtual UNK_RET vf_01(UNK_PARAMS);
        /// 0x08dc4632
        UNK_RET key_value(char const* a1, char const* a2);
        /// 0x08dc4638
        UNK_RET friendly_name(char const* a1);
        /// 0x08dc463e
        UNK_RET description(char const* a1);
    };
    /// 0x08dc4644
    FClassId();
    /// 0x08dc4650
    FClassId(FClassId::Info* a1);
    /// 0x08dc465c
    FClassId(FClassId const& a1);
    /// 0x08dc466a
    UNK_RET IsValid() const;
    /// 0x08dc4678
    UNK_RET IsKindOf(FClassId const& a1) const;
    /// 0x08dc46c0
    UNK_RET IsClass(FClassId const& a1) const;
    /// 0x08dc46d4
    UNK_RET operator==(FClassId const& a1) const;
    /// 0x08dc46e8
    UNK_RET operator!=(FClassId const& a1) const;
    /// 0x08dc46fc
    UNK_RET GetName() const;
    /// 0x08dc471c
    UNK_RET operator=(FClassId const& a1);
    /// 0x08dc472a
    UNK_RET NullId();
    /// 0x08dc4774
    UNK_RET ClassInfo() const;
    /// 0x08dc477c
    UNK_RET MetaData() const;
    /// 0x08dc4786
    UNK_RET GetFriendlyName() const;
    /// 0x08dc47a6
    UNK_RET GetDescription() const;
    /// 0x08dc47ba
    UNK_RET GetValue(char const* a1) const;
    /// 0x08dc47ce
    UNK_RET GetKeyValues(std::vector<std::pair<char const*, char const*>, std::allocator<std::pair<char const*, char const*> > >& a1) const;
};

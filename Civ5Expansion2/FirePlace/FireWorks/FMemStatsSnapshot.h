/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FString.h"
#include <lua.h>
#include <vector>

class FAllocationLocationStatistics;
class FMemStatsSnapshot;
template <class T0> class FSTLAllocator;

/// size ?
class FMemStatsSnapshot {
public:
    /// 0x08dd6784
    FMemStatsSnapshot();
    /// 0x08dd67e6
    UNK_RET Read(FDataStream& a1);
    /// 0x08dd69e0
    UNK_RET Write(FDataStream& a1);
    /// 0x08dd6aae
    UNK_RET Take();
    /// 0x08dd6fb8
    UNK_RET Save(char const* a1);
    /// 0x08dd71c0
    UNK_RET Load(char const* a1);
    /// 0x08dd7394
    UNK_RET CopyFrom(FMemStatsSnapshot const& a1);
    /// 0x08dd74de
    UNK_RET operator-(FMemStatsSnapshot const& a1) const;
};

/// 0x08dd6b57
UNK_RET MakeSnapShotFileName(int a1);
/// 0x08dd6bb2
UNK_RET GetNextSnapShotFileName();
/// 0x08dd6d1d
UNK_RET GetLastTwoSnapShotFileNames(FStringA& a1, FStringA& a2);
/// 0x08dd73f6
UNK_RET Found(FAllocationLocationStatistics const& a1, std::vector<FAllocationLocationStatistics, FSTLAllocator<FAllocationLocationStatistics> > const& a2);
/// 0x08dd7bee
int DefaultPanicHandler(lua_State* a1);

namespace FAppHandle {
/// 0x08dd79cc
UNK_RET Create(char const* a1, char const* a2, char const* a3);
}

/// origin binary
#pragma once
#include "FirePlace/FireWorks/FCallStack.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FIMemTracker.h"
#include "FirePlace/FireWorks/Win32/FDebugHelper.h"
#include <list>
#include <vector>

class FAllocationFilter;
class FAllocationLocationStatistics;
class FAllocationStatistics;
class FFile;
class FMemStatsSnapshot;
template <class T0> class FSTLAllocator;

/// size ?
/// vtable 0x0a3065c0
class FMemTracker : public FIMemTracker {
public:
    /// 0x08db3b5c
    FMemTracker(bool a1, bool a2);
    /// 0x08db47d8
    /// deleting 0x08db47b0
    virtual ~FMemTracker();
    /// 0x08db4a88
    virtual UNK_RET TrackMalloc(void const* a1, unsigned int a2, int a3, char const* a4, int a5, int a6, int a7, int a8);
    /// 0x08db528a
    virtual UNK_RET TrackCalloc(void const* a1, unsigned int a2, unsigned int a3, int a4, char const* a5, int a6, int a7, int a8, int a9);
    /// 0x08db52fe
    virtual UNK_RET TrackRealloc(void const* a1, void const* a2, unsigned int a3, int a4, char const* a5, int a6, int a7, int a8, int a9);
    /// 0x08db5752
    virtual UNK_RET TrackAlloc(FIMemTracker::EMemType a1, void const* a2, unsigned int a3, int a4, char const* a5, int a6, int a7, int a8, int a9);
    /// 0x08db5946
    virtual UNK_RET AddUntrackedMalloc(void* a1, unsigned int a2);
    /// 0x08db5732
    virtual UNK_RET TrackFree(void const* a1);
    /// 0x08db57c6
    virtual UNK_RET TrackFree(FIMemTracker::EMemType a1, void const* a2);
    /// 0x08db5848
    virtual UNK_RET SetBlockTag(FIMemTracker::EMemType a1, void const* a2, char const* a3, bool a4);
    /// 0x08db8000
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x08db8046
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x08db43a2
    UNK_RET GetUsedMemory() const;
    /// 0x08db43a6
    UNK_RET RegisterPoolID(int a1, char const* a2);
    /// 0x08db44c8
    UNK_RET RegisterPoolTagID(int a1, int a2, char const* a3);
    /// 0x08db48a2
    UNK_RET Shutdown();
    /// 0x08db498a
    UNK_RET SetDebugHelper(FDebugHelper* a1);
    /// 0x08db499a
    UNK_RET ResetFrame();
    /// 0x08db4aa8
    UNK_RET Allocation(void const* a1, unsigned int a2, int a3, char const* a4, int a5, int a6, int a7, int a8);
    /// 0x08db53de
    UNK_RET Deallocation(void const* a1);
    /// 0x08db57fc
    UNK_RET GetBlockSize(void const* a1) const;
    /// 0x08db598c
    UNK_RET IsTrackingType(FIMemTracker::EMemType a1);
    /// 0x08db59a0
    UNK_RET SetTrackingType(FIMemTracker::EMemType a1, bool a2);
    /// 0x08db59de
    UNK_RET GetTotalMemory() const;
    /// 0x08db5a06
    UNK_RET GetFreeMemory() const;
    /// 0x08db5a2c
    UNK_RET GetInitialMemory() const;
    /// 0x08db5a34
    UNK_RET GetRequestedMemory() const;
    /// 0x08db5a40
    UNK_RET GetTrackedMemory() const;
    /// 0x08db5a4c
    UNK_RET GetTrackedMemory(int a1);
    /// 0x08db5ab4
    UNK_RET GetActiveAllocations(int a1);
    /// 0x08db5ba2
    UNK_RET CopyFrameStatsList(std::list<FAllocationStatistics, FSTLAllocator<FAllocationStatistics> >& a1);
    /// 0x08db5c40
    UNK_RET CopyLocationStats(std::vector<FAllocationLocationStatistics, FSTLAllocator<FAllocationLocationStatistics> >& a1);
    /// 0x08db5d08
    UNK_RET SetExtendedTypeString(char const* a1);
    /// 0x08db5d36
    UNK_RET GetExtendedTypeString() const;
    /// 0x08db5d5c
    UNK_RET GetTrackingText();
    /// 0x08db5dd6
    UNK_RET GetTrackingText(FMemStatsSnapshot& a1);
    /// 0x08db6560
    UNK_RET Write(FDataStream& a1);
    /// 0x08db6618
    UNK_RET Read(FDataStream& a1);
    /// 0x08db68f4
    UNK_RET DumpCallStack(FCallStack const& a1, FFile* a2);
    /// 0x08db6b48
    UNK_RET DumpAllocations(bool a1, char const* a2, int a3);
    /// 0x08db6b9e
    UNK_RET DumpAllocationsByFilter(FAllocationFilter const& a1, bool a2, char const* a3);
    /// 0x08db71fa
    UNK_RET IncrementMarker(char const* a1);
    /// 0x08db747c
    UNK_RET SetCurrentMarkerDescription(char const* a1);
    /// 0x08db74e6
    UNK_RET FindPoolID(int a1);
    /// 0x08db7516
    UNK_RET FindPoolID(int a1) const;
    /// 0x08db7546
    UNK_RET FindPoolID(char const* a1);
    /// 0x08db75a2
    UNK_RET FindPoolID(char const* a1) const;
    /// 0x08db75fe
    UNK_RET AddPoolTypeFilter(int a1);
    /// 0x08db766a
    UNK_RET RemovePoolTypeFilter(int a1);
    /// 0x08db76f0
    UNK_RET RemoveFilteredAllocations();
    /// 0x08db7880
    UNK_RET ClearPoolTypeFilters();
    /// 0x08db78be
    UNK_RET InvertPoolTypeFilters(bool a1);
    /// 0x08db78fc
    UNK_RET DisableTrackingOptions(unsigned int a1);
};

/// 0x08db59c1
UNK_RET ActualBytesUsedByAllocation(int a1, int a2);
/// 0x08db5a03
UNK_RET GetProcessMemoryUsage();
/// 0x08db5b26
UNK_RET SortByTotalCalls(FAllocationLocationStatistics const& a1, FAllocationLocationStatistics const& a2);
/// 0x08db5b39
UNK_RET SortByFrameCalls(FAllocationLocationStatistics const& a1, FAllocationLocationStatistics const& a2);
/// 0x08db5b4e
UNK_RET SortByTotalBytes(FAllocationLocationStatistics const& a1, FAllocationLocationStatistics const& a2);
/// 0x08db5b63
UNK_RET SortByFrameBytes(FAllocationLocationStatistics const& a1, FAllocationLocationStatistics const& a2);
/// 0x08db5b78
UNK_RET SortByActiveBytes(FAllocationLocationStatistics const& a1, FAllocationLocationStatistics const& a2);
/// 0x08db5b8d
UNK_RET SortByActiveAllocs(FAllocationLocationStatistics const& a1, FAllocationLocationStatistics const& a2);
/// 0x08db604f
UNK_RET GetMemTracker();

namespace FSourceLocation {
/// 0x08db58de
UNK_RET SetFileTag(char const* a1, bool a2);
/// 0x08db60fe
UNK_RET SetLineTag(unsigned int a1);
/// 0x08db610a
UNK_RET Read(FDataStream& a1);
/// 0x08db615c
UNK_RET Write(FDataStream& a1);
}

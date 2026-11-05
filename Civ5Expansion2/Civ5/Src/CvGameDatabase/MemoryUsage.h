/// origin guess
#pragma once

namespace Database {
/// size ?
class MemoryUsage {
public:
    /// 0x08b97e28
    MemoryUsage();
    /// 0x08b97e5e
    UNK_RET AdjustCurrentUserAllocated(long a1);
};
}

namespace Database {
/// 0x08b97e73
UNK_RET MSHeapMemMalloc(int a1);
/// 0x08b97ec2
UNK_RET MSHeapMemFree(void* a1);
/// 0x08b97f25
UNK_RET MSHeapMemRealloc(void* a1, int a2);
/// 0x08b97f9c
UNK_RET MSHeapMemSize(void* a1);
/// 0x08b97fd9
UNK_RET MSHeapMemRoundup(int a1);
/// 0x08b97fe4
UNK_RET MSHeapMemInit(void* a1);
/// 0x08b98050
UNK_RET MSHeapMemShutdown(void* a1);
/// 0x08b98075
UNK_RET MSHeapGetHandle();
/// 0x08b9808a
UNK_RET MSHeapCollectUsageStatistics();
/// 0x08b9817e
UNK_RET MSHeapGetMemoryUsage();
}

/// origin guess
#pragma once

/// size ?
class FAudioMemMgr {
public:
    /// 0x08d3dbf4
    FAudioMemMgr();
    /// 0x08d3de86
    ~FAudioMemMgr();
    /// 0x08d3de60
    UNK_RET Init();
    /// 0x08d3df0c
    UNK_RET Deinit();
    /// 0x08d3df0e
    UNK_RET Allocate(unsigned int a1);
    /// 0x08d3df14
    UNK_RET Deallocate(void* a1);
};

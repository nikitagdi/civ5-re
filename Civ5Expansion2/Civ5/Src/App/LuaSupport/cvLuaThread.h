/// origin guess
#pragma once

/// size ?
/// vtable 0x0a2fde10
class cvLuaThread {
public:
    class StdLibs;
    /// 0x08975f5c
    cvLuaThread();
    /// 0x08975fbc
    /// deleting 0x08975f94
    virtual ~cvLuaThread();
    /// 0x08976004
    UNK_RET RegisterEvents(unsigned int a1);
    /// 0x08976032
    UNK_RET SetName(char const* a1);
    /// 0x089760d4
    UNK_RET RegisterStdLibs(cvLuaThread::StdLibs a1);
    /// 0x08976102
    UNK_RET RegisterGameCore();
    /// 0x08976128
    UNK_RET Lock();
    /// 0x08976158
    UNK_RET Unlock();
};

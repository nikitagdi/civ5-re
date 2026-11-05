/// origin guess
#pragma once
#include "FirePlace/FireWorks/FThread.h"

/// size ?
/// vtable 0x0a2fc880
class GameCoreThread : public FThread {
public:
    /// 0x087948b4
    GameCoreThread();
    /// 0x087ab8e2
    /// deleting 0x087ab918
    virtual ~GameCoreThread();
    /// 0x08794928
    virtual UNK_RET Run();
};

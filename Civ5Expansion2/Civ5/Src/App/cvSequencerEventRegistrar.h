/// origin guess
#pragma once

class GameCoreEventEntryHeader;
namespace cvSerialEventLogging {
enum STATE : int;
}

/// size ?
class cvSequencerEventRegistrar {
public:
    /// 0x0881def0
    UNK_RET RegisterEvents();
};

/// 0x0881f5ff
UNK_RET EventSystem_SynchronizeGameCore();

namespace cvSerialEventLogging {
/// 0x0881eeee
UNK_RET LogEvent(cvSerialEventLogging::STATE a1, int a2, unsigned int a3, GameCoreEventEntryHeader const* a4);
/// 0x0881f416
UNK_RET LogMessage(char const* a1);
/// 0x0881f472
UNK_RET LogDispatchTimeStart(int a1, unsigned int a2);
/// 0x0881f4d0
UNK_RET LogDispatchTimeEnd();
/// 0x0881f5cc
UNK_RET EnableLogging(bool a1);
/// 0x0881f5e6
UNK_RET EnableProfiling(bool a1);
}

namespace cvSerialEventRegistrar {
/// 0x0881e4f0
UNK_RET RegisterEvents();
/// 0x0881ed8e
UNK_RET RegisterFriendlyName(int a1, char const* a2);
/// 0x0881ee9e
UNK_RET GetFriendlyName(int a1);
}

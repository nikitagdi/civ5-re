/// origin guess
#pragma once

/// size ?
class Civ5CrashInfo {
public:
    /// 0x0865b510
    Civ5CrashInfo();
    /// 0x0865b546
    UNK_RET AddCrashTimeInfo();
    /// 0x0865b93a
    UNK_RET AddInfo(char const* a1);
    /// 0x0865b99c
    UNK_RET OnSystemCallFailure(char const* a1);
};

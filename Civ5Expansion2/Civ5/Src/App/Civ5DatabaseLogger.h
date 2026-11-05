/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/IDatabaseLogger.h"

/// size ?
/// vtable 0x0a2fa1e0
class Civ5DatabaseLogger : public Database::IDatabaseLogger {
public:
    /// 0x086f49fc
    /// deleting 0x086f49d4
    virtual ~Civ5DatabaseLogger();
    /// 0x086f49fe
    virtual void Log(char const* a1, Database::IDatabaseLogger::LogTypes a2);
    /// 0x086f49e8
    void operator delete(void* a1);
    /// 0x086f4a5a
    void* operator new(unsigned int a1);
};

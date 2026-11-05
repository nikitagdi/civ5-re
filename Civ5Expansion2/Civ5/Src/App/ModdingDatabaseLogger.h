/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/IDatabaseLogger.h"

namespace Modding {
/// size ?
/// vtable 0x0a301240
class ModdingDatabaseLogger : public Database::IDatabaseLogger {
public:
    /// 0x08c79aa0
    /// deleting 0x08c79a78
    virtual ~ModdingDatabaseLogger();
    /// 0x08c79aa2
    virtual void Log(char const* a1, Database::IDatabaseLogger::LogTypes a2);
    /// 0x08c79a8c
    void operator delete(void* a1);
    /// 0x08c79b1c
    void* operator new(unsigned int a1);
};
}

namespace CvModdingFramework { namespace cvPath {
/// 0x08c79b30
UNK_RET GetDirectoryName(wchar_t const* a1, unsigned int a2, unsigned int& a3);
/// 0x08c79ba6
UNK_RET GetFileName(wchar_t const* a1, unsigned int a2, unsigned int& a3);
/// 0x08c79bf8
UNK_RET GetVolumeSeparatorChar();
/// 0x08c79bfe
UNK_RET GetExtension(wchar_t const* a1, unsigned int a2, unsigned int& a3);
/// 0x08c79c2c
UNK_RET GetExtensionSeparatorChar();
/// 0x08c79c32
UNK_RET GetDirectorySeparatorChar();
/// 0x08c79c38
UNK_RET GetAltDirectorySeparatorChar();
/// 0x08c79c3e
UNK_RET GetFileNameWithoutExtension(wchar_t const* a1, unsigned int a2, unsigned int& a3);
/// 0x08c79c98
UNK_RET GetPathSeparatorChar();
/// 0x08c79c9e
UNK_RET NormalizePath(wchar_t* a1, unsigned int a2, wchar_t a3);
}}

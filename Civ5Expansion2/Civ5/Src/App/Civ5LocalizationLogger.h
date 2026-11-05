/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "Civ5/Src/CvLocalization/ILocalizationLogger.h"
#include <memory>
#include <string>

/// size ?
/// vtable 0x0a2fa210
class Civ5LocalizationLogger : public Localization::ILocalizationLogger {
public:
    /// 0x086f4a98
    /// deleting 0x086f4a70
    virtual ~Civ5LocalizationLogger();
    /// 0x086f4a9a
    virtual void Log(char const* a1, Localization::ILocalizationLogger::LogTypes a2);
    /// 0x086f4a84
    void operator delete(void* a1);
    /// 0x086f4b14
    void* operator new(unsigned int a1);
};

/// 0x086f4b28
UNK_RET safecpy(CvString& a1, CvString const& a2, int a3);
/// 0x086f4b7c
UNK_RET safecpy(char* a1, char const* a2, int a3);

namespace Civ5NetMessage {
/// 0x086f4bc2
UNK_RET IsNetMessageDebugActive();
/// 0x086f4bef
UNK_RET NetMessageDebug(std::string const& a1);
/// 0x086f4cda
UNK_RET ResetNetMessageStatics();
/// 0x086f4de8
UNK_RET SetTurnSlicingActive(bool a1);
}

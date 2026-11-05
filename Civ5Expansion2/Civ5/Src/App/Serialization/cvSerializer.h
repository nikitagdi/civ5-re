/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FString.h"

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvPath;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class cvSerializer {
public:
    /// 0x08998380
    cvSerializer();
    /// 0x089983b8
    ~cvSerializer();
    /// 0x0899844a
    UNK_RET CatchDelayedSave(EventTemplate<Event_Int2Type<76>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089984de
    UNK_RET LoadGame(cvPath const& a1, bool a2);
    /// 0x089987d8
    UNK_RET SaveGame(cvPath const& a1, SaveGameTypes a2);
    /// 0x08998960
    UNK_RET CompleteSave();
    /// 0x089989e8
    UNK_RET QuickLoad();
    /// 0x08998d9c
    UNK_RET QuickSave();
    /// 0x08999180
    UNK_RET AutoSave(bool a1, bool a2);
    /// 0x0899a6b0
    UNK_RET MakeAutoSaveFileName(FStringW& a1, wchar_t const* a2, char const* a3);
    /// 0x0899a7a8
    UNK_RET SaveResyncAutoSave(FDataStream& a1);
    /// 0x0899ab68
    UNK_RET DoLoad(FDataStream& a1);
    /// 0x0899afa4
    UNK_RET ReadObjects(FDataStream& a1);
    /// 0x0899b338
    UNK_RET ClearLastSuccessfulAutoSaveName();
    /// 0x0899b35c
    UNK_RET DoSave(FDataStream& a1);
    /// 0x0899b382
    UNK_RET WriteObjects(FDataStream& a1);
    /// 0x0899bb06
    UNK_RET GetMostRecentMultiplayerAutoSaveName();
    /// 0x0899bef8
    UNK_RET GetMostRecentAutoSaveName();
    /// 0x0899c260
    UNK_RET GetLastSuccessfulAutoSaveName();
    /// 0x0899c28e
    UNK_RET HasLastSuccessfulAutoSaveName();
    /// 0x0899c2b6
    UNK_RET Initialize();
    /// 0x0899c2da
    UNK_RET Uninitialize();
    /// 0x0c078c74
    static UNK_DATA ms_szLastAutoSaveName[0x420];
};

namespace CvGameCoreSerializer {
/// 0x0899b52a
UNK_RET Read(FDataStream& a1);
/// 0x0899b832
UNK_RET Write(FDataStream& a1);
/// 0x0899c2fe
UNK_RET ValidateLoadedData();
}

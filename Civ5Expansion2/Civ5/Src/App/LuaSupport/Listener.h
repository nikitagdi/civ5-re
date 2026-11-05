/// origin guess
#pragma once
#include <lua.h>

class LinearEventDispatcher;
class NonSerialContainer;
class NullEventClass;
class cvEventSystemNonSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

namespace cvLuaArtInfoLibrary {
/// size ?
class Listener {
public:
    /// 0x08941bc0
    Listener(lua_State* a1);
    /// 0x08941c50
    ~Listener();
    /// 0x08940118
    /// lua ?
    static int lDestroy(lua_State* a1);
    /// 0x08941bd4
    UNK_RET SubscribeToEvents();
    /// 0x08941c7a
    UNK_RET UnsubscribeToEvents();
    /// 0x08941cf6
    UNK_RET OnLandmarkLibraryInitialized(EventTemplate<Event_Int2Type<20>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08941d34
    UNK_RET OnUnitLibraryInitialized(EventTemplate<Event_Int2Type<21>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08941d72
    UNK_RET OnWonderLibraryInitialized(EventTemplate<Event_Int2Type<22>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};
}

/// 0x08943853
UNK_RET IsGameCoreBusy();
/// 0x08943899
/// lua ?
int LuaThunk_IsGameCoreBusy(lua_State* a1);
/// 0x08943901
int LuaToString_IsGameCoreBusy(lua_State* a1);
/// 0x08943974
UNK_RET setEndTurnTimerLength(float a1);
/// 0x089439f5
/// lua ?
int LuaThunk_setEndTurnTimerLength(lua_State* a1);
/// 0x08943a5d
int LuaToString_setEndTurnTimerLength(lua_State* a1);
/// 0x08943ade
UNK_RET getEndTurnTimerLength();
/// 0x08943b70
/// lua ?
int LuaThunk_getEndTurnTimerLength(lua_State* a1);
/// 0x08943bd5
int LuaToString_getEndTurnTimerLength(lua_State* a1);

namespace cvLuaArtInfoLibrary {
/// 0x0894014c
int pRegisterLandmarkLibrary(lua_State* a1);
/// 0x0894155a
int pRegisterWonderLibrary(lua_State* a1);
/// 0x08941658
int pRegisterUnitLibrary(lua_State* a1);
}

namespace cvLuaDBLibrary {
/// 0x08942b30
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x08942bb2
int pRegisterDBLibrary(lua_State* a1);
/// 0x08942d46
int pRegisterGameInfoLibrary(lua_State* a1);
/// 0x08942d7e
int pRegisterGameDefinesLibrary(lua_State* a1);
/// 0x08942e4e
int ReleaseDatabaseCache(lua_State* a1);
/// 0x08942e82
int pReleaseDatabaseCache(lua_State* a1);
/// 0x08942f3c
/// lua ?
int lStatementCount(lua_State* a1);
/// 0x08942f7e
/// lua ?
int lStatementSQL(lua_State* a1);
/// 0x08942fd4
/// lua ?
int lQuery(lua_State* a1);
/// 0x0894313c
/// lua ?
int lCreateQuery(lua_State* a1);
/// 0x089431ac
/// lua ?
int lIndexGameDefinesTable(lua_State* a1);
/// 0x08943316
/// lua ?
int lIterateQuery(lua_State* a1);
/// 0x08943394
/// lua ?
int lIterateCachedQuery(lua_State* a1);
}

namespace cvLuaGameCoreLibrary {
/// 0x089434f8
int Register(lua_State* a1);
/// 0x08943536
int pRegister(lua_State* a1);
/// 0x0894360a
int pCreateGameCoreData(lua_State* a1);
/// 0x08943764
int pRegisterGameData(lua_State* a1);
}

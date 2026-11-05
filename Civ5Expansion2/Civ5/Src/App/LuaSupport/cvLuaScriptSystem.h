/// origin binary
#pragma once
#include "Civ5/Src/App/cvTunerListener.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include <lua.h>

class AppUpdateData;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaOutputData;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvPath;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fddf8
class cvLuaScriptSystem {
public:
    class MemoryUsage;
    /// size ?
    class MemoryUsage {
    public:
        /// 0x08970ed4
        MemoryUsage();
        /// 0x08970f0a
        UNK_RET AdjustCurrentUserAllocated(long a1);
    };
    /// 0x0897109e
    cvLuaScriptSystem();
    /// 0x08971b7a
    /// deleting 0x08971b52
    virtual ~cvLuaScriptSystem();
    /// 0x08970f20
    UNK_RET Alloc(void* a1, void* a2, unsigned int a3, unsigned int a4);
    /// 0x08971034
    UNK_RET ErrorHandler(char const* a1);
    /// 0x0897137c
    UNK_RET Log(char const* a1);
    /// 0x089713ca
    static int pInitializeMainState(lua_State* a1);
    /// 0x08971826
    UNK_RET GetLuaStates(FFastVector<cvTunerListener::LuaStateInfo, false, 14u, 0u, BaseVector<cvTunerListener::LuaStateInfo, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x08971a9a
    UNK_RET OnAppUpdate(EventTemplate<Event_Int2Type<2>, AppUpdateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08971b02
    UNK_RET OnLuaOutputMessage(EventTemplate<Event_Int2Type<67>, LuaOutputData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08971c8e
    UNK_RET CreateThread();
    /// 0x08971cd8
    static int pCreateThread(lua_State* a1);
    /// 0x08971d58
    static int DestroyThread(lua_State* a1);
    /// 0x08971e26
    static int pDestroyThread(lua_State* a1);
    /// 0x08971ee8
    static int ExposeStateToTuner(lua_State* a1);
    /// 0x08971f3e
    static int LockThread(lua_State* a1);
    /// 0x08971f88
    static int pLockThread(lua_State* a1);
    /// 0x08971fda
    static int UnlockThread(lua_State* a1);
    /// 0x08972024
    static int pUnlockThread(lua_State* a1);
    /// 0x0897206e
    UNK_RET LoadFile(lua_State* a1, cvPath const& a2);
    /// 0x089720a4
    static int pLoadFile(lua_State* a1);
    /// 0x0897210c
    /// lua ?
    static int lGetMemoryUsage(lua_State* a1);
    /// 0x089722a0
    /// lua ?
    static int lCollectMemoryUsage(lua_State* a1);
    /// 0x089723aa
    /// lua ?
    static int lGetSystemMemoryUsage(lua_State* a1);
    /// 0x08972532
    /// lua ?
    static int lCollectSystemMemoryUsage(lua_State* a1);
    /// 0x08972622
    /// lua ?
    static int lGetDllContextMemoryUsage(lua_State* a1);
    /// 0x08972750
    /// lua ?
    static int lCollectDllContextMemoryUsage(lua_State* a1);
    /// 0x08972906
    static int pMath_Clamp(lua_State* a1);
    /// 0x08972992
    static int pTable_Count(lua_State* a1);
    /// 0x08972a1a
    static int pTable_Fill(lua_State* a1);
    /// 0x08972b0c
    static int pGlobal_Print(lua_State* a1);
    /// 0x08972cd0
    static int pGlobal_ipairsaux(lua_State* a1);
    /// 0x08972d52
    static int pGlobal_ipairs(lua_State* a1);
    /// 0x08972df4
    static int pGlobal_where(lua_State* a1);
    /// 0x08972e38
    /// lua ?
    static int lCreateThread(lua_State* a1);
    /// 0x08972f18
    /// lua ?
    static int lDestroyThread(lua_State* a1);
    /// 0x08972f6e
    static int pClearLuaEnv(lua_State* a1);
    /// 0x0c077184
    static UNK_DATA ms_apSingletonInstance[0x4];
};

extern "C" {
    /// 0x0c0771a4
    extern UNK_DATA s_DllContextMemoryUsage[0x1c];
    /// 0x0c077188
    extern UNK_DATA s_SystemMemoryUsage[0x1c];
}

namespace cvLuaSteamLibrary {
/// 0x08973f2c
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x08973f6a
int pRegister(lua_State* a1);
/// 0x08974246
/// lua ?
int lActivateGameOverlay(lua_State* a1);
/// 0x08974292
/// lua ?
int lActivateGameOverlayToWebPage(lua_State* a1);
/// 0x08974506
/// lua ?
int lActivateGameOverlayToStore(lua_State* a1);
/// 0x08974568
/// lua ?
int lActivateInviteOverlay(lua_State* a1);
/// 0x089745da
/// lua ?
int lSetOverlayNotificationPosition(lua_State* a1);
/// 0x0897464c
/// lua ?
int lIsOverlayEnabled(lua_State* a1);
/// 0x08974690
/// lua ?
int lGetMySteamID(lua_State* a1);
/// 0x08974836
/// lua ?
int lGetPlayerSteamID(lua_State* a1);
/// 0x089749fc
/// lua ?
int lGetCloudSaveFileName(lua_State* a1);
/// 0x08974a42
/// lua ?
int lGetMaxCloudSaves(lua_State* a1);
/// 0x08974a70
/// lua ?
int lGetCloudSaves(lua_State* a1);
/// 0x08975052
/// lua ?
int lSaveGameToCloud(lua_State* a1);
/// 0x089752bc
/// lua ?
int lCopyLastAutoSaveToSteamCloud(lua_State* a1);
}

namespace cvLuaTCPConnection {
/// 0x089758bc
UNK_RET Create(lua_State* a1, char const* a2, int a3);
/// 0x08975d9e
/// lua ?
int lDestroy(lua_State* a1);
/// 0x08975de2
/// lua ?
int lSend(lua_State* a1);
/// 0x08975e4e
/// lua ?
int lReceive(lua_State* a1);
/// 0x08975eba
/// lua ?
int lPeek(lua_State* a1);
/// 0x08975f26
/// lua ?
int lDisconnect(lua_State* a1);
}

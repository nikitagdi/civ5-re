/// origin guess
#pragma once
#include "FirePlace/FireWorks/EventSystem/FastDelegate.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaCommon.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include "FirePlace/FireWorks/FString.h"
#include <lua.h>

class AppUpdateData;
class FGXVector2;
class FInetHostAddress;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SteamLobbyInviteData;
class SteamServerInviteData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
class cvTunerListener;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class cvTunerListener {
public:
    class ErrorListener;
    class ITreeNode;
    class LuaStateInfo;
    class StatTrackerInfo;
    class TreeNodeDataMember;
    class TreeNodeWeakPtr;
    /// size ?
    class ErrorListener {
    public:
        /// 0x08981a46
        ErrorListener();
        /// 0x08981a7e
        UNK_RET HandleError(char const* a1);
    };
    /// size ?
    /// vtable 0x0a2fde30
    class ITreeNode {
    public:
        class TextWriter;
        /// size ?
        class TextWriter {
        public:
            /// 0x0897c670
            TextWriter(cvTunerListener& a1);
            /// 0x0897c67c
            UNK_RET Write(char const* a1, ...);
        };
        /// 0x0897c5c0
        ITreeNode();
        /// 0x0897c620
        /// deleting 0x0897c5f8
        virtual ~ITreeNode();
        /// pure
        virtual UNK_RET GetText(cvTunerListener::ITreeNode::TextWriter& a1) = 0;
        /// pure
        virtual UNK_RET GetNodes(FFastVector<cvTunerListener::ITreeNode*, false, 0u, 0u, BaseVector<cvTunerListener::ITreeNode*, false>::FDefaultFastVectorAllocator>& a1) = 0;
        /// pure
        virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1) = 0;
    };
    /// size ?
    class LuaStateInfo {
    public:
        /// 0x08981a0a
        LuaStateInfo(char const* a1, lua_State* a2);
    };
    /// size ?
    class StatTrackerInfo {
    public:
        /// 0x0897c758
        StatTrackerInfo(char const* a1, fastdelegate::FastDelegate0<FStringA> const& a2);
    };
    /// size ?
    class TreeNodeDataMember {
    public:
        /// 0x0897c564
        TreeNodeDataMember(char const* a1, char const* a2, ...);
    };
    /// size ?
    class TreeNodeWeakPtr {
    public:
        /// 0x0897c6e2
        TreeNodeWeakPtr();
        /// 0x0897c6ee
        TreeNodeWeakPtr(cvTunerListener::ITreeNode* a1);
        /// 0x0897c728
        TreeNodeWeakPtr(cvTunerListener::TreeNodeWeakPtr const& a1);
        /// 0x0897c71c
        UNK_RET NodeDestroyed();
    };
    /// 0x0897b1e0
    cvTunerListener();
    /// 0x0897bece
    ~cvTunerListener();
    /// 0x0897b548
    UNK_RET OnUpdate(EventTemplate<Event_Int2Type<2>, AppUpdateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0897bd10
    UNK_RET OnMultiplayerGameLaunched(EventTemplate<Event_Int2Type<139>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0897be0a
    UNK_RET OnExitToMainMenu(EventTemplate<Event_Int2Type<161>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0897bea6
    UNK_RET OnMultiplayerGameLobbyInvite(EventTemplate<Event_Int2Type<137>, SteamLobbyInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0897beba
    UNK_RET OnMultiplayerGameServerInvite(EventTemplate<Event_Int2Type<138>, SteamServerInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0897c11a
    UNK_RET Disable();
    /// 0x0897c1b0
    UNK_RET UpdateTunerLuaStates();
    /// 0x0897c228
    UNK_RET SendOutputMessage(char const* a1);
    /// 0x0897c30e
    UNK_RET Enabled() const;
    /// 0x0897c31e
    UNK_RET Enable(FInetHostAddress const& a1, unsigned short a2);
    /// 0x0897c378
    UNK_RET EnteringMultiplayerStagingRoom();
    /// 0x0897c38c
    UNK_RET ExitingMultiplayerStagingRoom();
    /// 0x0897c428
    UNK_RET RegisterTree(cvTunerListener::ITreeNode* a1);
    /// 0x0897c4c4
    UNK_RET RegisterStatTracker(char const* a1, fastdelegate::FastDelegate0<FStringA> const& a2);
    /// 0x0897c7b8
    static int DestroyingLuaState(lua_State* a1);
    /// 0x0897c80c
    UNK_RET UpdateLuaStatesList();
    /// 0x0897c8ba
    UNK_RET ReadPending();
    /// 0x0897ca2a
    UNK_RET SendUpdate(unsigned int a1, char const* a2, bool a3);
    /// 0x0897caec
    UNK_RET ProcessInput(char* a1);
    /// 0x0897cf2a
    UNK_RET SendAppInfo();
    /// 0x0897d0a8
    UNK_RET HandleTreeRequest(char* a1);
    /// 0x0897d336
    UNK_RET HandleStatTrackersListRequest();
    /// 0x0897d414
    UNK_RET HandleStartStatTrackerRequest(char* a1);
    /// 0x0897d46e
    UNK_RET HandleStopStatTrackerRequest(char* a1);
    /// 0x0897d4ec
    UNK_RET RunConsoleCommand(lua_State* a1, char const* a2);
    /// 0x0897d5d4
    UNK_RET HandleHelpRequest(lua_State* a1, char* a2);
    /// 0x0897ddf8
    UNK_RET HandleTypedHelpRequest(lua_State* a1, char* a2);
    /// 0x0897e076
    UNK_RET HandleSetValueRequest(lua_State* a1, char* a2);
    /// 0x0897e802
    UNK_RET HandleQueryRequest(lua_State* a1, char* a2);
    /// 0x0897ee78
    static int GetQueryTable(lua_State* a1);
    /// 0x0897ef20
    UNK_RET HandleTableInfoRequest(lua_State* a1, char* a2);
    /// 0x0897f59e
    UNK_RET HandlePopulateListRequest(lua_State* a1, char* a2);
    /// 0x0897f95a
    UNK_RET HandleOnListSelectionRequest(lua_State* a1, char* a2);
    /// 0x0897fbe2
    UNK_RET HandlePopulateMultiselectListRequest(lua_State* a1, char* a2);
    /// 0x0898018a
    UNK_RET IsSymbolChar(char a1);
    /// 0x089801be
    UNK_RET HandleTableHelpRequest(FLua::Table a1);
    /// 0x08980544
    UNK_RET HandleArrayHelpRequest(FLua::Table a1);
    /// 0x08980730
    UNK_RET HandleMemberFuncHelpRequest(FLua::Table a1);
    /// 0x08980aca
    UNK_RET HandleTableHelpRequest(FLua::Table a1, FLua::Type a2);
    /// 0x08980c7c
    UNK_RET HandleArrayHelpRequest(FLua::Table a1, FLua::Type a2);
    /// 0x08980e2e
    UNK_RET OutputHelpCategory(FLua::Type a1);
    /// 0x08980f58
    UNK_RET GetKeyResolutionTable(FLua::Table const& a1, FLua::Table& a2);
    /// 0x089811ba
    UNK_RET ResolveTablePath(lua_State* a1, char* a2, FLua::Table& a3);
};

/// 0x08983b7c
UNK_RET Factorial(unsigned int a1);
/// 0x08983b9e
UNK_RET Multinomial(unsigned int a1, unsigned int a2, unsigned int const* a3);
/// 0x08983c6b
UNK_RET BernsteinPoly(unsigned int a1, unsigned int a2, unsigned int const* a3, float const* a4);
/// 0x08983d13
UNK_RET Mat3x3Determinant(float const (*a1)[3]);
/// 0x08983d87
UNK_RET SignedArea(FGXVector2 const& a1, FGXVector2 const& a2, FGXVector2 const& a3);
/// 0x08983df4
UNK_RET CartToBaryPoly(unsigned int a1, FGXVector2 const* a2, FGXVector2 a3, float* a4);

/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FGXUInt2;
class FGXVector2;
class FGXVector3;
class FGXVector4;
class FNetMessage;
class FXmlElement;
class LinearEventDispatcher;
class LuaContext;
class NullEventClass;
class SerialContainer;
class UIRenderBuffer;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
enum UIAABB : int;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fc570
class ReplayMapControl : public ControlBase {
public:
    /// 0x08754f5a
    ReplayMapControl();
    /// 0x0875504c
    /// deleting 0x08755024
    virtual ~ReplayMapControl();
    /// 0x087552a8
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x087554be
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x087557c4
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x08754d50
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x08755112
    virtual UNK_RET Initialize();
    /// 0x087555d0
    virtual UNK_RET PerspectiveRender(unsigned int a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x087558d6
    virtual UNK_RET PerspectiveRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x08754eda
    virtual int PushSelf(lua_State* a1);
    /// 0x087553c4
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x087556ca
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x08756438
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x0875643e
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x08756464
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x087559d0
    UNK_RET SetMapSize(FGXUInt2 const& a1, int a2, FGXVector4 const& a3);
    /// 0x08755afe
    UNK_RET SetPlot(FGXUInt2 const& a1, int a2, FGXVector4 const& a3);
    /// 0x08755b70
    /// lua ?
    static int lSetMapSize(lua_State* a1);
    /// 0x08755f2e
    /// lua ?
    static int lSetPlot(lua_State* a1);
    /// 0x0a3e3044
    static UNK_DATA s_Cache[0x1c];
};

/// 0x08756099
/// lua ?
int LuaThunk_ReplayMapControl_SetMapSize(lua_State* a1);
/// 0x087560ad
int LuaToString_ReplayMapControl_SetMapSize(lua_State* a1);
/// 0x0875610c
/// lua ?
int LuaThunk_ReplayMapControl_SetPlot(lua_State* a1);
/// 0x08756120
int LuaToString_ReplayMapControl_SetPlot(lua_State* a1);

namespace CvGameController {
/// 0x08756e54
UNK_RET Initialize();
/// 0x08756ee6
UNK_RET GetPlayerController(PlayerTypes a1);
/// 0x08756efe
UNK_RET RestoreTurnCompleteStatus();
/// 0x08757002
UNK_RET SubscribeToMessages();
/// 0x08757040
UNK_RET OnSerialEventQueueFlushed(GameCoreEventTemplate<Event_Int2Type<152>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x0875709a
UNK_RET UnsubscribeFromMessages();
/// 0x087570d8
UNK_RET GetActivePlayer();
/// 0x08757150
UNK_RET HasReceivedTurnCompleteFromAllPlayers();
/// 0x08757304
UNK_RET HasReceivedTurnAllCompleteFromAllPlayers();
/// 0x0875745e
UNK_RET HasReceivedAICivsProcessedFromAllPlayers();
/// 0x0875763c
UNK_RET SendTurnComplete(PlayerTypes a1);
/// 0x08757700
UNK_RET HasSentTurnComplete(PlayerTypes a1);
/// 0x08757716
UNK_RET SetSentTurnComplete(PlayerTypes a1, bool a2);
/// 0x087577b8
UNK_RET SendTurnAllComplete(PlayerTypes a1);
/// 0x08757814
UNK_RET HasSentTurnAllComplete(PlayerTypes a1);
/// 0x0875782a
UNK_RET SetSentTurnAllComplete(PlayerTypes a1, bool a2);
/// 0x087578b0
UNK_RET HasReceivedTurnAllComplete(PlayerTypes a1);
/// 0x087578dc
UNK_RET GetNumTurnAllComplete();
/// 0x08757a2a
UNK_RET GetNumTurnAllCompleteRequired();
/// 0x08757b60
UNK_RET SetLastMPBarrierReached();
/// 0x08757bdc
UNK_RET HasReceivedTurnComplete(PlayerTypes a1);
/// 0x08757c08
UNK_RET GetNumTurnComplete();
/// 0x08757d20
UNK_RET GetNumTurnCompleteRequired();
/// 0x08757e1e
UNK_RET HasReceivedAICivsProcessed(PlayerTypes a1);
/// 0x08757eb6
UNK_RET GetNumAICivsProcessed();
/// 0x08757ffe
UNK_RET GetNumAICivsProcessedRequired();
/// 0x08758146
UNK_RET HasReceivedInitialAIProcessed(PlayerTypes a1);
/// 0x087581de
UNK_RET HasReceivedInitialAIProcessedFromAllPlayers();
/// 0x087583bc
UNK_RET GetNumInitialAIProcessed();
/// 0x08758504
UNK_RET GetNumInitialAIProcessedRequired();
/// 0x0875864c
UNK_RET SendTurnUnready(PlayerTypes a1);
/// 0x087589ea
UNK_RET HasSentTurnUnready(PlayerTypes a1);
/// 0x08758a00
UNK_RET SetSentTurnUnready(PlayerTypes a1, bool a2);
/// 0x08758a86
UNK_RET CanAdvanceTurn();
/// 0x08758b9a
UNK_RET NetLoopBarrierCompleted();
/// 0x08758c22
UNK_RET TurnChanged(int a1);
/// 0x08758c7e
UNK_RET ActivePlayerChanged(PlayerTypes a1);
/// 0x08758d76
UNK_RET BroadcastGameplayMessage(FNetMessage& a1);
/// 0x08758dd6
UNK_RET RecordGameCoreMessages(bool a1, bool a2);
/// 0x08758eb2
UNK_RET IsRecordingGameCoreMessages();
/// 0x08758ec6
UNK_RET IsGameCoreRecordQueueEmpty();
/// 0x08758ee4
UNK_RET SuspendGameCoreUpdate(bool a1);
/// 0x08758f20
UNK_RET IsGameCoreUpdateSuspended();
/// 0x08758f3e
UNK_RET Update();
/// 0x0a3e3770
extern UNK_DATA m_aPlayers[0x600];
/// 0x0a3e376c
extern UNK_DATA m_lastMPBarrierReached[0x4];
/// 0x0a3e3769
extern UNK_DATA m_sentTurnAllComplete[0x1];
/// 0x0a3e3768
extern UNK_DATA m_sentTurnComplete[0x1];
/// 0x0a3e376a
extern UNK_DATA m_sentTurnUnready[0x1];
}

namespace GameCore {
/// 0x0a3e5088
extern UNK_DATA ms_GameCoreLock[0x4];
/// 0x0a3e5090
extern UNK_DATA ms_GameCoreLockCount[0x4];
/// 0x0a3e508c
extern UNK_DATA ms_GameCoreLockOwnerID[0x4];
}

namespace cvUISkinManager {
/// 0x0a3e4754
extern UNK_DATA m_baseColors[0x438];
/// 0x0a3e4b8c
extern UNK_DATA ms_kUISkins[0xc];
/// 0x0a3e4b98
extern UNK_DATA ms_szSkinPlatform[0x4];
}

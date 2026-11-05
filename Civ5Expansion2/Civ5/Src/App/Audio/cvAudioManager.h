/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/ICvDLLScriptSystem.h"
#include "FirePlace/FireWorks/FAudioManager.h"
#include <lua.h>

class EraChangedData;
class HEXVec_i;
class HexFOWStateChangedData;
class LandmarkCreatedData;
class LandmarkHandleData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class SpecialTileAddedData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvLandmarkVis_Init;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace cvSpecialCaseSimSystem {
class ImprovementPlotData;
}

/// size ?
/// vtable 0x0a2fc3f0
class cvAudioManager : public FAudioManager {
public:
    /// 0x08748210
    cvAudioManager();
    /// 0x087483a8
    /// deleting 0x08748380
    virtual ~cvAudioManager();
    /// 0x0874ab36
    virtual UNK_RET DoHotLoad2DScripts(char const* a1);
    /// 0x0874ac9a
    virtual UNK_RET DoHotLoad3DScripts(char const* a1);
    /// 0x0874ae02
    virtual UNK_RET DoHotLoadXMLAudioDefines(char const* a1);
    /// 0x0874ae90
    virtual UNK_RET LoadAudioParams(char const* a1);
    /// 0x08748344
    UNK_RET SubscribeToEvents();
    /// 0x087484ca
    UNK_RET UnSubscribeFromEvents();
    /// 0x08748506
    UNK_RET LoadExtraContentScripts();
    /// 0x08748800
    UNK_RET ReloadModContent();
    /// 0x0874b1f2
    UNK_RET AddAudioTagHash(unsigned int a1, char const* a2);
    /// 0x0874b228
    UNK_RET FindAudioTagHash(unsigned int a1);
    /// 0x0874b256
    UNK_RET OnAfterModsDeactivate(EventTemplate<Event_Int2Type<93>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};

namespace cvLuaGameEventLibrary {
/// 0x0874ca4c
int Register(lua_State* a1);
/// 0x0874ca8a
int pRegister(lua_State* a1);
/// 0x0874cb3a
UNK_RET CallHook(lua_State* a1, char const* a2, ICvEngineScriptSystemArgs1* a3, bool& a4);
/// 0x0874ccce
UNK_RET CallAccumulator(lua_State* a1, char const* a2, ICvEngineScriptSystemArgs1* a3, int& a4);
/// 0x0874ce58
UNK_RET CallAccumulator(lua_State* a1, char const* a2, ICvEngineScriptSystemArgs1* a3, float& a4);
/// 0x0874cfea
UNK_RET CallTestAll(lua_State* a1, char const* a2, ICvEngineScriptSystemArgs1* a3, bool& a4);
/// 0x0874d058
UNK_RET CallTestAny(lua_State* a1, char const* a2, ICvEngineScriptSystemArgs1* a3, bool& a4);
/// 0x0874d0c6
int GetEventFactory(lua_State* a1);
/// 0x0874d114
UNK_RET PushScriptSystemArgs(lua_State* a1, ICvEngineScriptSystemArgs1* a2);
/// 0x0874d11a
int CreateLuaOnlyEventFactory(lua_State* a1);
/// 0x0874d25c
/// lua ?
int lEventFactoryIndexer(lua_State* a1);
/// 0x0874d3ca
int RemoveAllEventHandlers(lua_State* a1);
/// 0x0874d3fe
int pRemoveAllEventHandlers(lua_State* a1);
}

namespace cvSpecialCaseSimSystem {
/// 0x0874d62c
UNK_RET SubscribeToEvents();
/// 0x0874d702
UNK_RET OnImprovementCreated(GameCoreEventTemplate<Event_Int2Type<59>, LandmarkCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x0874db38
UNK_RET OnImprovementDestroyed(GameCoreEventTemplate<Event_Int2Type<60>, LandmarkHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x0874dc4a
UNK_RET OnEraChanged(GameCoreEventTemplate<Event_Int2Type<48>, EraChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x0874dc4c
UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x0874de3e
UNK_RET UnSubscribeToEvents();
/// 0x0874df14
UNK_RET Startup(bool a1);
/// 0x0874dff6
UNK_RET Shutdown();
/// 0x0874e046
UNK_RET FillLandmarkSimInfo_MoaiStatues(cvSpecialCaseSimSystem::ImprovementPlotData& a1, HEXVec_i a2, cvLandmarkVis_Init& a3);
/// 0x0874e97a
UNK_RET FillLandmarkSimInfo_Feitoria(cvSpecialCaseSimSystem::ImprovementPlotData& a1, HEXVec_i a2, cvLandmarkVis_Init& a3);
/// 0x0874f0b2
UNK_RET FillLandmarkSimInfo_PontoonBridge(cvSpecialCaseSimSystem::ImprovementPlotData& a1, HEXVec_i a2, cvLandmarkVis_Init& a3);
/// 0x08750068
UNK_RET OnSpecialTileAdded(GameCoreEventTemplate<Event_Int2Type<68>, SpecialTileAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x087500ea
UNK_RET StartupGame();
/// 0x08750288
UNK_RET ShutdownGame();
/// 0x087502ba
UNK_RET DestroyImprovement(HEXVec_i a1);
}

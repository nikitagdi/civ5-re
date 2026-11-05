/// origin guess
#pragma once
#include "Civ5/Src/App/BuilderContext_Base.h"
#include "FirePlace/FireEngine2/CompositeContainerBuilderContext.h"
#include "FirePlace/FireWorks/FSingleton.h"

class FGXInt2;
class FGXVector3;
class FGXVector4;
class FGameIniParser;
class LinearEventDispatcher;
class LuaContext;
class NullEventClass;
class SerialContainer;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace GraphicsSettings { namespace LeaderSettings {
class LeaderQualityLevel;
class LeaderSettingsStore;
}}
namespace WorldGlobals {
enum WORLD_GLOBALS_MODE : int;
}

/// size ?
/// vtable 0x0a2f8fc0
class ForestVisBuilderContext : public CompositeContainerBuilderContext {
public:
    /// 0x08670fbc
    /// deleting 0x08670f94
    virtual ~ForestVisBuilderContext();
    /// 0x08671066
    UNK_RET NotifyVBCreated(BuilderContext_Base::VBCreationInfo const& a1);
    /// 0x08671102
    UNK_RET NotifyIBCreated(BuilderContext_Base::IBCreationInfo const& a1);
};

/// 0x0867144c
UNK_RET ColorIdentity(FGXVector4* a1, FGXVector3 const& a2, FGXVector3 const& a3, void* a4);

// Methods of the g_ColorKeySystem storage (ColorKeyData): `this` is the ColorKeyData object.
/// size ?
class ColorKeySystem {
public:
    /// 0x08671478
    UNK_RET Startup(bool a1);
    /// 0x0867166c
    void OnScreenShotTaken(GameCoreEventTemplate<Event_Int2Type<28>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086717aa
    void LoadKeyTexture(unsigned int a1, unsigned int a2, char* a3);
    /// 0x086718ba
    UNK_RET Shutdown();
    /// 0x08671964
    UNK_RET RegisterDebugMenu(LuaContext* a1);
    /// 0x08671afe
    void SystemSelect(unsigned int a1, unsigned int a2);
    /// 0x08671b66
    void TypeSelect(unsigned int a1, unsigned int a2);
    /// 0x08671bce
    void ReloadColorKey(unsigned int a1, unsigned int a2);
    /// 0x08671c54
    UNK_RET SaveColorKey(unsigned int a1, unsigned int a2);
    /// 0x08671ca6
    void ResetColorKey(unsigned int a1, unsigned int a2);
};

namespace GraphicsSettings { namespace LeaderSettings {
/// 0x0867319a
UNK_RET SetQuality(GraphicsSettings::LeaderSettings::LeaderQualityLevel a1, GraphicsSettings::LeaderSettings::LeaderSettingsStore& a2);
/// 0x086732f5
UNK_RET WriteSettings(GraphicsSettings::LeaderSettings::LeaderSettingsStore const& a1, FGameIniParser* a2);
/// 0x0867381f
UNK_RET ValidateLoadedSettings();
}}

namespace WorldGlobals {
/// 0x086725b4
UNK_RET GetPaddedMapSize(FGXInt2 a1, unsigned int a2);
/// 0x086725f9
UNK_RET SetupWorldGlobals(FGXInt2 a1, WorldGlobals::WORLD_GLOBALS_MODE a2);
/// 0x08672d82
UNK_RET SetupTerrainPieceGlobals();
}

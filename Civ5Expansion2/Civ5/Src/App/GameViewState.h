/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"
#include "Civ5/Src/App/Graphics/CvCamera.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FTimer.h"
#include <Windows.h>
#include <lua.h>

class ActivePlayerData;
class AssetTypeChangedData;
class CameraViewData;
class EffectTransferTriggeredData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NonSerialContainer;
class NullEventClass;
class SerialContainer;
class SingleSubscriberContainer;
class SystemUpdateUIData;
class TextInfoLayerManager;
class WonderStateChangedData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
}

/// size 0xce0
/// vtable 0x0a2fd3d0
class GameViewState : public Civ5GameState {
public:
    class ParticleVisUpdateJob;
    /// size ?
    /// vtable 0x0a2fd3f4
    class ParticleVisUpdateJob : public FJob {
    public:
        /// 0x08832fb8
        virtual void Execute(unsigned int a1);
    };
    // SetViewType switches the strategic view on exactly for this value; the constructor starts at 1.
    enum ViewType {
        VIEWTYPE_GAME = 1,
        VIEWTYPE_STRATEGIC = 2,
    };
    /// 0x08833016
    GameViewState();
    /// 0x0883742c
    /// deleting 0x08837404
    virtual ~GameViewState();
    /// 0x08837568
    virtual UNK_RET Begin();
    /// 0x0883761e
    virtual UNK_RET End();
    /// 0x0883aa90
    virtual UNK_RET Update(float a1);
    /// 0x0883e48a
    virtual void Render(unsigned int a1);
    /// 0x08837cc4
    virtual bool ProcessInput(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, long lParam);
    /// 0x088333c0
    UNK_RET ResetValues();
    /// 0x088333f4
    UNK_RET Startup();
    /// 0x0883349a
    UNK_RET SubscribeToEvents();
    /// 0x08833612
    UNK_RET InitializeSystemsBasedOnInitializedSystemsFlags();
    /// 0x088339e8
    UNK_RET StartupGame();
    /// 0x08834db0
    UNK_RET InitDebugMenu();
    /// 0x08835108
    UNK_RET PublishWorldGlobals();
    /// 0x088352c2
    UNK_RET PublishMapGlobals();
    /// 0x08835334
    UNK_RET ShutdownGame(bool a1);
    /// 0x08835c38
    UNK_RET HandleSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08835d06
    UNK_RET GraphicsSystemToggle(unsigned int a1, unsigned int a2);
    /// 0x08835ee6
    UNK_RET GraphicsMIPSelect(unsigned int a1, unsigned int a2);
    /// 0x08835f28
    UNK_RET ThreadedSubmit(unsigned int a1, unsigned int a2);
    /// 0x08835f44
    UNK_RET CBRebase(unsigned int a1, unsigned int a2);
    /// 0x08835f46
    UNK_RET HexDebugTextSelect(unsigned int a1, unsigned int a2);
    /// 0x088360a2
    UNK_RET HexDebugPlayerDown(unsigned int a1, unsigned int a2);
    /// 0x08836280
    UNK_RET HexDebugPlayerUp(unsigned int a1, unsigned int a2);
    /// 0x08836460
    UNK_RET GenerateAvailableImprovements(unsigned int a1, unsigned int a2);
    /// 0x088367e2
    UNK_RET GenerateAvailableRawResources(unsigned int a1, unsigned int a2);
    /// 0x08836b1c
    UNK_RET GenerateAvailableFeatures(unsigned int a1, unsigned int a2);
    /// 0x08836e4e
    UNK_RET ChangeMode(unsigned int uMode, unsigned int a2);
    /// 0x08836e5e
    UNK_RET OnUnitTypeChange(EventTemplate<Event_Int2Type<12>, AssetTypeChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x08836eaa
    UNK_RET OnWonderTypeChange(EventTemplate<Event_Int2Type<23>, AssetTypeChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x08836ee0
    UNK_RET OnWonderStateChange(EventTemplate<Event_Int2Type<24>, WonderStateChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x08836ef2
    UNK_RET OnWonderTogglePlacement(EventTemplate<Event_Int2Type<26>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x08836f1a
    UNK_RET OnEffectTransferTriggered(EventTemplate<Event_Int2Type<50>, EffectTransferTriggeredData, LinearEventDispatcher, SingleSubscriberContainer, unsigned int, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088370e4
    UNK_RET OnLoadingScreenClose(EventTemplate<Event_Int2Type<170>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08837194
    UNK_RET PreProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x088371f6
    UNK_RET Shutdown();
    /// 0x0883728c
    UNK_RET UnSubscribeToEvents();
    /// 0x0883770a
    UNK_RET CalcCurrentHex();
    /// 0x08837d30
    bool ProcessMouse(unsigned int uMsg, unsigned int wParam, long lParam);
    /// 0x08838cf6
    UNK_RET ProcessKeyboard(unsigned int uMsg, unsigned int wParam, long lParam);
    /// 0x08839062
    UNK_RET GenerateUnitBenchmark();
    /// 0x0883985a
    UNK_RET ProcessDebugKeyboard(unsigned int a1, unsigned int a2, long a3);
    /// 0x0883a2ac
    UNK_RET DoBenchmark(float a1);
    /// 0x0883a754
    UNK_RET GenerateLandmarkBenchmark();
    /// 0x0883b2fe
    UNK_RET UpdateSystems();
    /// 0x0883b68e
    UNK_RET GenerateLateGameViewBenchmark();
    /// 0x0883b7b6
    UNK_RET SetupFrameGlobals();
    /// 0x0883beac
    GameViewState::ViewType GetViewType() const;
    /// 0x0883beb8
    UNK_RET CalcVisibleRegion();
    /// 0x0883c71c
    UNK_RET PublishFrameGlobals();
    /// 0x0883ccd6
    void PreRender();
    /// 0x0883cd16
    void RenderGame(unsigned int hRenderTarget);
    /// 0x0883d34e
    void RenderShadows();
    /// 0x0883d62c
    void RenderReflections();
    /// 0x0883d76a
    UNK_RET PrintDebugInfo_noAlloc(unsigned int a1);
    /// 0x0883dfba
    UNK_RET RenderBenchmark(unsigned int a1);
    /// 0x0883e4b4
    UNK_RET PrintDebugInfo(unsigned int a1);
    /// 0x0883e83e
    UNK_RET OnCameraMoved(EventTemplate<Event_Int2Type<4>, CameraViewData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0883e89a
    UNK_RET OnEndTurn(GameCoreEventTemplate<Event_Int2Type<125>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x0883e8c6
    UNK_RET OnBeginTurn(GameCoreEventTemplate<Event_Int2Type<124>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x0883e90c
    UNK_RET OnGameplaySetActivePlayer(GameCoreEventTemplate<Event_Int2Type<131>, ActivePlayerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0883ecf4
    UNK_RET SetViewType(GameViewState::ViewType eViewType);
    /// 0x0883ed5e
    UNK_RET SetStrategicView(bool a1);
    /// 0x0883f2c6
    UNK_RET ToggleGrid(unsigned int a1, unsigned int a2);
    /// 0x0883f324
    UNK_RET PublishRecordCommandStreams(bool a1);
    /// 0x0883f480
    UNK_RET TogglePauseGameplay(bool a1);
    /// 0x0883f5d8
    UNK_RET FOW_SetAll(unsigned int a1);
    /// 0x0883f77a
    UNK_RET Water_SetAutoReload(bool a1);
    /// 0x0883f888
    UNK_RET Water_SetAmplitude(int a1);
    /// 0x0883f96a
    UNK_RET Water_SetGradientScale(int a1);
    /// 0x0883fa4c
    UNK_RET Water_SetSpeed(int a1);
    /// 0x0883fb70
    UNK_RET Water_SetFlowParameters(int a1, int a2, int a3);
    /// 0x0883fd3e
    UNK_RET Water_SetScale(int a1);
    /// 0x0883fe62
    UNK_RET Water_SetSpec(int a1, int a2);
    /// 0x0884000c
    UNK_RET Ice_Reload();
    /// 0x0884003e
    UNK_RET Waves_Reload();

    FireGrafix::CCommandStream* m_pCommandStream;     ///< +0x10
    // Frames per second, sampled every 10 frames in RenderGame.
    float m_fFramesPerSecond;                         ///< +0x14
    FTimer m_kFpsTimer;                               ///< +0x18
    FTimer m_kTurnTimer;                              ///< +0x24
    float field_30;                                   ///< +0x30
    float m_fTurnProcessingTime;                      ///< +0x34
    TextInfoLayerManager* m_pTextInfoLayerManager;    ///< +0x38
    CvCamera m_kCamera;                               ///< +0x3c
    // Bit 0: events subscribed (Startup); bit 1: hex grid shown (ToggleGrid).
    unsigned int m_uStateFlags;                       ///< +0x1a0
    int field_1a4;                                    ///< +0x1a4
    int field_1a8;                                    ///< +0x1a8
    // Bit 18 gates the strategic-view paths of CalcCurrentHex and ProcessMouse.
    unsigned int m_uInitializedSystemsFlags;          ///< +0x1ac
    int field_1b0;                                    ///< +0x1b0
    int field_1b4;                                    ///< +0x1b4
    bool m_bRenderEnabled;                            ///< +0x1b8
    bool field_1b9;                                   ///< +0x1b9
    unsigned char unk_1ba[0x2];                       ///< +0x1ba
    // The in-game LuaContext (Assets/UI/InGame/InGame) lives here; LuaContext has no layout yet.
    unsigned char unk_1bc[0x9ac];                     ///< +0x1bc
    GameViewState::ViewType m_eViewType;              ///< +0xb68
    unsigned int m_uMode;                             ///< +0xb6c
    // Projection-space cursor position, copied from the UI manager.
    FGXVector2 m_vMousePos;                           ///< +0xb70
    HEXVec_i m_kCurrentHex;                           ///< +0xb78
    HEXVec_i field_b84;                               ///< +0xb84
    HEXVec_i field_b90;                               ///< +0xb90
    // Eased shadow opacity, steered toward the zoom band by RenderShadows.
    float m_fShadowFade;                              ///< +0xb9c
    FGXVector3 m_vMouseWorldPos;                      ///< +0xba0
    FStringA m_strUnitType;                           ///< +0xbac
    char m_szWonderType[0x80];                        ///< +0xbb0
    int field_c30;                                    ///< +0xc30
    int field_c34;                                    ///< +0xc34
    int field_c38;                                    ///< +0xc38
    int m_iWonderState;                               ///< +0xc3c
    bool field_c40;                                   ///< +0xc40
    unsigned char unk_c41[0x3];                       ///< +0xc41
    // Corners last published as VisibilityUpdatedData.
    FGXInt2 m_akPublishedGridTopLeftVisible[4];       ///< +0xc44
    FGXInt2 m_akPublishedGridBottomRightVisible[4];   ///< +0xc64
    unsigned int m_uWonderPlacement;                  ///< +0xc84
    bool field_c88;                                   ///< +0xc88
    int field_c8c;                                    ///< +0xc8c
    int field_c90;                                    ///< +0xc90
    int field_c94;                                    ///< +0xc94
    int field_c98;                                    ///< +0xc98
    int field_c9c;                                    ///< +0xc9c
    int field_ca0;                                    ///< +0xca0
    int field_ca4;                                    ///< +0xca4
    int field_ca8;                                    ///< +0xca8
    int field_cac;                                    ///< +0xcac
    int field_cb0;                                    ///< +0xcb0
    FJobSet field_cb4;                                ///< +0xcb4
    ParticleVisUpdateJob m_kParticleVisUpdateJob;     ///< +0xcb8
    int field_cc8;                                    ///< +0xcc8
    int field_ccc;                                    ///< +0xccc
    int field_cd0;                                    ///< +0xcd0
    int field_cd4;                                    ///< +0xcd4
    void* field_cd8;                                  ///< +0xcd8
    int field_cdc;                                    ///< +0xcdc

    /// 0x0bff8738
    static UNK_DATA s_PlayerUnitCounts[0xc];
    /// 0x0bff8744
    static UNK_DATA s_PlayerUnitIDs[0xc];
    /// 0x0bff8750
    static UNK_DATA s_UnitMoveLocs[0xc];
};

// Set by RenderShadows to 0, 0.5 or 1 with the zoom band that decides the shadow fade target.
/// 0x0a30fa14
extern "C" float g_fShadowZoomBand;

/// 0x08840dac
UNK_RET Fun_08840dac(UNK_PARAMS);

/// 0x0883f028
/// lua ?
int LuaThunk_SetGameViewRenderType(lua_State* a1);
/// 0x0883f0ba
UNK_RET SetGameViewRenderType(int a1);
/// 0x0883f131
int LuaToString_SetGameViewRenderType(lua_State* a1);
/// 0x0883f1b2
/// lua ?
int LuaThunk_GetGameViewRenderType(lua_State* a1);
/// 0x0883f22c
UNK_RET GetGameViewRenderType();
/// 0x0883f253
int LuaToString_GetGameViewRenderType(lua_State* a1);
/// 0x0883f365
/// lua ?
int LuaThunk_PublishRecordCommandStreams(lua_State* a1);
/// 0x0883f3ff
int LuaToString_PublishRecordCommandStreams(lua_State* a1);
/// 0x0883f528
/// lua ?
int LuaThunk_TogglePauseGameplay(lua_State* a1);
/// 0x0883f557
int LuaToString_TogglePauseGameplay(lua_State* a1);
/// 0x0883f63a
/// lua ?
int LuaThunk_FOW_SetAll(lua_State* a1);
/// 0x0883f6f8
int LuaToString_FOW_SetAll(lua_State* a1);
/// 0x0883f797
/// lua ?
int LuaThunk_Water_SetAutoReload(lua_State* a1);
/// 0x0883f806
int LuaToString_Water_SetAutoReload(lua_State* a1);
/// 0x0883f889
/// lua ?
int LuaThunk_Water_SetAmplitude(lua_State* a1);
/// 0x0883f8e9
int LuaToString_Water_SetAmplitude(lua_State* a1);
/// 0x0883f96b
/// lua ?
int LuaThunk_Water_SetGradientScale(lua_State* a1);
/// 0x0883f9cb
int LuaToString_Water_SetGradientScale(lua_State* a1);
/// 0x0883fa75
/// lua ?
int LuaThunk_Water_SetSpeed(lua_State* a1);
/// 0x0883faef
int LuaToString_Water_SetSpeed(lua_State* a1);
/// 0x0883fbd3
/// lua ?
int LuaThunk_Water_SetFlowParameters(lua_State* a1);
/// 0x0883fcad
int LuaToString_Water_SetFlowParameters(lua_State* a1);
/// 0x0883fd67
/// lua ?
int LuaThunk_Water_SetScale(lua_State* a1);
/// 0x0883fde1
int LuaToString_Water_SetScale(lua_State* a1);
/// 0x0883fec3
/// lua ?
int LuaThunk_Water_SetSpec(lua_State* a1);
/// 0x0883ff82
int LuaToString_Water_SetSpec(lua_State* a1);
/// 0x08840061
/// lua ?
int LuaThunk_Ice_Reload(lua_State* a1);
/// 0x088400cd
int LuaToString_Ice_Reload(lua_State* a1);
/// 0x08840140
/// lua ?
int LuaThunk_Waves_Reload(lua_State* a1);
/// 0x0884019d
int LuaToString_Waves_Reload(lua_State* a1);

/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"
#include "Civ5/Src/App/CvLeaderHeadErrorHook.h"
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class ParticleEffectStatsResponseData;
class SerialContainer;
class SystemUpdateUIData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fd370
class CvLeaderHeadViewer : public Civ5GameState, public CvLeaderHeadErrorHook {
public:
    /// 0x0882c128
    CvLeaderHeadViewer();
    /// 0x0882c350
    /// deleting 0x0882c328
    virtual ~CvLeaderHeadViewer();
    /// 0x0882f4a2
    virtual UNK_RET Begin();
    /// 0x0882f7ae
    virtual UNK_RET End();
    /// 0x0882f7e8
    virtual UNK_RET Update(float a1);
    /// 0x0882fb66
    virtual UNK_RET Render(unsigned int a1);
    /// 0x0883001c
    virtual UNK_RET ProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x0883034a
    /// thunk 0x088305c0
    virtual UNK_RET ErrorMessage(FStringA const& a1);
    /// 0x0882c5a2
    UNK_RET ConfigureButtons();
    /// 0x0882d69a
    UNK_RET OnLoadLeader(unsigned int a1, unsigned int a2);
    /// 0x0882d69c
    UNK_RET OnReload(unsigned int a1, unsigned int a2);
    /// 0x0882d7dc
    UNK_RET OnReloadParticles(unsigned int a1, unsigned int a2);
    /// 0x0882d80c
    UNK_RET OnResetCamera(unsigned int a1, unsigned int a2);
    /// 0x0882d830
    UNK_RET OnToggleSkin(unsigned int a1, unsigned int a2);
    /// 0x0882d890
    UNK_RET OnSkinDebug(unsigned int a1, unsigned int a2);
    /// 0x0882d8de
    UNK_RET OnUnwrapSkin(unsigned int a1, unsigned int a2);
    /// 0x0882d936
    UNK_RET OnShowPR(unsigned int a1, unsigned int a2);
    /// 0x0882d97e
    UNK_RET OnPause(unsigned int a1, unsigned int a2);
    /// 0x0882d98a
    UNK_RET OnChangeLeader(unsigned int a1, unsigned int a2);
    /// 0x0882d98c
    UNK_RET OnChangeBackground(unsigned int a1, unsigned int a2);
    /// 0x0882d98e
    UNK_RET OnToggleSM41(unsigned int a1, unsigned int a2);
    /// 0x0882d9f4
    UNK_RET OnShowDOFBlurButton(unsigned int a1, unsigned int a2);
    /// 0x0882da3c
    UNK_RET OnShowGlowButton(unsigned int a1, unsigned int a2);
    /// 0x0882da84
    UNK_RET OnShowDistortButton(unsigned int a1, unsigned int a2);
    /// 0x0882dacc
    UNK_RET OnForceAllShadows(unsigned int a1, unsigned int a2);
    /// 0x0882db0e
    UNK_RET OnForceNoShadows(unsigned int a1, unsigned int a2);
    /// 0x0882db50
    UNK_RET OnNormalShadows(unsigned int a1, unsigned int a2);
    /// 0x0882db92
    UNK_RET OnTransparencyDebug(unsigned int a1, unsigned int a2);
    /// 0x0882dbe0
    UNK_RET OnToggleLeader(unsigned int a1, unsigned int a2);
    /// 0x0882dc38
    UNK_RET OnToggleBGObjects(unsigned int a1, unsigned int a2);
    /// 0x0882dc90
    UNK_RET OnToggleDOFButton(unsigned int a1, unsigned int a2);
    /// 0x0882dcfa
    UNK_RET OnToggleBloomButton(unsigned int a1, unsigned int a2);
    /// 0x0882dd64
    UNK_RET OnToggleMipDisplay(unsigned int a1, unsigned int a2);
    /// 0x0882de42
    UNK_RET OnCreateColorKey(unsigned int a1, unsigned int a2);
    /// 0x0882de90
    UNK_RET OnReloadColorKey(unsigned int a1, unsigned int a2);
    /// 0x0882deb8
    UNK_RET OnToggleColorKey(unsigned int a1, unsigned int a2);
    /// 0x0882ded2
    UNK_RET OnDOFBlurWidthSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882df10
    UNK_RET OnDOFBlurRampSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882df4e
    UNK_RET OnDOFBlurDistanceSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882df8c
    UNK_RET OnBloomWidthSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882dfca
    UNK_RET OnBloomScaleSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e008
    UNK_RET OnBloomMinSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e046
    UNK_RET OnShadowTintSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e074
    UNK_RET OnDiffuseEnvSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e0a2
    UNK_RET OnSpecularEnvSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e0d0
    UNK_RET On3DConvergeSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e10e
    UNK_RET OnBackgroundRedSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e13c
    UNK_RET OnBackgroundGreenSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e16a
    UNK_RET OnBackgroundBlueSlider(float a1, unsigned int a2, unsigned int a3);
    /// 0x0882e198
    UNK_RET OnLightBackground(unsigned int a1, unsigned int a2);
    /// 0x0882e1dc
    UNK_RET OnMediumBackground(unsigned int a1, unsigned int a2);
    /// 0x0882e220
    UNK_RET OnDarkBackground(unsigned int a1, unsigned int a2);
    /// 0x0882e264
    UNK_RET OnHideUIOnAnimation(unsigned int a1, unsigned int a2);
    /// 0x0882e270
    UNK_RET OnQuit(unsigned int a1, unsigned int a2);
    /// 0x0882e290
    UNK_RET OnDemoMode(unsigned int a1, unsigned int a2);
    /// 0x0882e55c
    UNK_RET OnStopDemoMode(unsigned int a1, unsigned int a2);
    /// 0x0882e5de
    UNK_RET OnChangeLeaderDemo(unsigned int a1, unsigned int a2);
    /// 0x0882e5e0
    UNK_RET OnNeutralYesAnimation(unsigned int a1, unsigned int a2);
    /// 0x0882e630
    UNK_RET OnNeutralNoAnimation(unsigned int a1, unsigned int a2);
    /// 0x0882e680
    UNK_RET OnHateYesAnimation(unsigned int a1, unsigned int a2);
    /// 0x0882e6d0
    UNK_RET OnHateNoAnimation(unsigned int a1, unsigned int a2);
    /// 0x0882e720
    UNK_RET OnUnhideUI(unsigned int a1, unsigned int a2);
    /// 0x0882e776
    UNK_RET ResetDemoMode(bool a1);
    /// 0x0882e7c0
    UNK_RET Initialize(char const* a1, FStringA const& a2, bool a3, bool a4);
    /// 0x0882ea72
    UNK_RET LoadScene(char const* a1, char const* a2);
    /// 0x0882ee72
    UNK_RET OnParticleEffectStats(EventTemplate<Event_Int2Type<46>, ParticleEffectStatsResponseData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0882eea2
    UNK_RET HandleSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0882ef70
    UNK_RET OnScreenShotTaken(GameCoreEventTemplate<Event_Int2Type<28>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0882f13c
    UNK_RET SyncSliders();
    /// 0x0882f30e
    UNK_RET ShowHideUI(bool a1);
    /// 0x0882f4f8
    UNK_RET RegenerateErrorText();
    /// 0x0882faf2
    UNK_RET OnForceAnimation(unsigned int a1);
    /// 0x0882fdea
    UNK_RET RegenerateRenderStateText();
    /// 0x088305ec
    UNK_RET ShowFileDialog(char* a1);
    /// 0x088305f0
    UNK_RET UnloadScene();
    /// 0x08830680
    UNK_RET ShowHideDemoModeButtons(bool a1);
    /// 0x088307dc
    UNK_RET SyncSliderLabels();
};

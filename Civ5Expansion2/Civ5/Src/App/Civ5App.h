/// origin guess
#pragma once
#include "Civ5/Src/App/cvEventSubscriber.h"
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/Win32/FWinMainApp.h"
#include <Windows.h>

class Civ5GameState;
class CvDLLUtility;
class GameLobbyJoinRequested_t;
class GameOverlayActivated_t;
class GameServerChangeRequested_t;
class InterfaceBuddy;
class LanguageChangingData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NonSerialContainer;
class NullEventClass;
class ScreenShotData;
class SerialContainer;
class SystemInitData;
class SystemUpdateUIData;
class UserInputData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2f9ee0
class Civ5App : public FWinMainApp, public cvEventSubscriber {
public:
    class JobA;
    class JobB;
    class JobC;
    class JobD;
    class JobE;
    class JobF;
    class GameState;
    /// size ?
    /// vtable 0x0a2f9f24
    class JobA : public FJob {
    public:
        /// 0x086df80a
        virtual void Execute(unsigned int a1);
    };
    /// size ?
    /// vtable 0x0a2f9f3c
    class JobB : public FJob {
    public:
        /// 0x086df81a
        virtual void Execute(unsigned int a1);
    };
    /// size ?
    /// vtable 0x0a2f9f54
    class JobC : public FJob {
    public:
        /// 0x086df830
        virtual void Execute(unsigned int a1);
    };
    /// size ?
    /// vtable 0x0a2f9f6c
    class JobD : public FJob {
    public:
        /// 0x086df878
        virtual void Execute(unsigned int a1);
    };
    /// size ?
    /// vtable 0x0a2f9f84
    class JobE : public FJob {
    public:
        /// 0x086df88c
        virtual void Execute(unsigned int a1);
    };
    /// size ?
    /// vtable 0x0a2f9f9c
    class JobF : public FJob {
    public:
        /// 0x086df8dc
        virtual void Execute(unsigned int a1);
    };
    /// 0x086df8fc
    Civ5App();
    /// 0x086e040e
    /// deleting 0x086e03e6
    virtual ~Civ5App();
    /// 0x086e988a
    virtual UNK_RET OnIdle();
    /// 0x086e9b9a
    virtual UNK_RET OnMessage(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x086ee654
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x086ee660
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x086e00da
    UNK_RET OnSteamOverlayActivatedCallback(GameOverlayActivated_t* a1);
    /// 0x086e00f8
    UNK_RET OnSteamLobbyJoinRequestedCallback(GameLobbyJoinRequested_t* a1);
    /// 0x086e0206
    UNK_RET OnSteamServerChangeRequestedCallback(GameServerChangeRequested_t* a1);
    /// 0x086e02c8
    UNK_RET ResetValues();
    /// 0x086e08f2
    UNK_RET Release();
    /// 0x086e0e92
    UNK_RET ReadBiosInfo();
    /// 0x086e0f3a
    UNK_RET PlayOpeningMovie();
    /// 0x086e10a2
    UNK_RET PumpMessagesToGameState();
    /// 0x086e12d8
    UNK_RET Init(char const* a1);
    /// 0x086e2218
    UNK_RET BeginInit(char const* a1);
    /// 0x086e38e6
    UNK_RET WriteToLogFile(char const* a1);
    /// 0x086e391e
    UNK_RET SetupInitForSpecificSystems(char const* a1);
    /// 0x086e3bbe
    UNK_RET SetupFileSystem(char const* a1);
    /// 0x086e41d2
    UNK_RET SetupGlobals();
    /// 0x086e4222
    UNK_RET SetupApp();
    /// 0x086e43ac
    UNK_RET SetupLogging();
    /// 0x086e4562
    UNK_RET GetLogFile();
    /// 0x086e456e
    UNK_RET SetupScriptSystem();
    /// 0x086e45d8
    UNK_RET SetupNetwork(char const* a1);
    /// 0x086e462a
    UNK_RET SetupDLC();
    /// 0x086e464e
    UNK_RET SetupEngine();
    /// 0x086e4678
    UNK_RET SetupJobManager();
    /// 0x086e476a
    UNK_RET CreateUIAndSetupScreenMode();
    /// 0x086e47f0
    UNK_RET RenderFrame(float a1);
    /// 0x086e48e2
    UNK_RET SetupDatabase();
    /// 0x086e494c
    UNK_RET SetupAudio();
    /// 0x086e4a70
    UNK_RET SetupModding();
    /// 0x086e4aa4
    UNK_RET SetupLocalization();
    /// 0x086e548e
    UNK_RET SetupDLL();
    /// 0x086e5532
    UNK_RET SetupIndependentSystems(unsigned long a1, unsigned long a2);
    /// 0x086e69dc
    UNK_RET SetupUI();
    /// 0x086e6af8
    UNK_RET ParsePostDLLLoadIniSettings();
    /// 0x086e7356
    UNK_RET SetupEventRecording(char const* a1);
    /// 0x086e7748
    UNK_RET SetupAutomation(char const* a1);
    /// 0x086e7778
    UNK_RET FinishInit(char const* a1);
    /// 0x086e8c38
    UNK_RET SetGameInterface(InterfaceBuddy* a1);
    /// 0x086e8c74
    UNK_RET SetDLLUtility(CvDLLUtility* a1);
    /// 0x086e8cb8
    UNK_RET ShutdownEventRecording();
    /// 0x086e8d5e
    UNK_RET SetBoundMouse(bool a1);
    /// 0x086e8dd2
    UNK_RET UnSubscribeToEvents();
    /// 0x086e8eec
    UNK_RET SubscribeToEvents();
    /// 0x086e9006
    UNK_RET OnScreenShot(GameCoreEventTemplate<Event_Int2Type<27>, ScreenShotData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e9020
    UNK_RET OnPlaybackBegin(EventTemplate<Event_Int2Type<8>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e905c
    UNK_RET OnPlaybackEnd(EventTemplate<Event_Int2Type<9>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e9098
    UNK_RET OnSystemInit(EventTemplate<Event_Int2Type<102>, SystemInitData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e912a
    UNK_RET OnLanguageChanging(EventTemplate<Event_Int2Type<89>, LanguageChangingData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e92ba
    UNK_RET OnGraphicsOptionsChanged(EventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e92bc
    UNK_RET OnResolutionOptionsChanged(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e92f6
    UNK_RET OnCloseBenchmark(EventTemplate<Event_Int2Type<104>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e9302
    UNK_RET ProcessRecordedInput(EventTemplate<Event_Int2Type<107>, UserInputData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086e9346
    UNK_RET TakeScreenShot();
    /// 0x086e979c
    UNK_RET TryHandleMinimizedMode();
    /// 0x086e9c3c
    UNK_RET ProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x086e9e72
    UNK_RET ClipCursorToClientRect();
    /// 0x086e9eae
    UNK_RET FlushInputMessages();
    /// 0x086e9f46
    UNK_RET LaunchTunerProcess();
    /// 0x086e9fee
    UNK_RET SetNextGameState(Civ5GameState* a1);
    /// 0x086ea002
    UNK_RET ParseStartupSettings();
    /// 0x086eacf4
    UNK_RET SetupLowResGraphicsCatalog();
    /// 0x086eae96
    UNK_RET SetupDirectoryFileCatalog(char const* a1, int a2, char const* a3, char const* a4);
    /// 0x086eb1fc
    UNK_RET SetupLanguageSpecificAudioCatalog(char const* a1, bool a2);
    /// 0x086eb720
    UNK_RET EngineQuickInit();
    /// 0x086eba08
    UNK_RET GetLanguageTypeFromIni(FStringA* a1, FStringA* a2);
    /// 0x086ebe7a
    UNK_RET GetDisableFallbackLanguageSupport() const;
    /// 0x086ebefc
    UNK_RET GetValidateGameDatabase() const;
    /// 0x086ebf10
    UNK_RET ApplyBindMouse();
    /// 0x086ebf6a
    UNK_RET SetBenchmarkPreGameSettings();
    /// 0x086ebfce
    UNK_RET GetGameState(Civ5App::GameState a1);
    /// 0x086ec01c
    UNK_RET LoadPreGameSettings();
    /// 0x086ec824
    UNK_RET GetGameBuildID();
    /// 0x086ec90c
    UNK_RET SavePreGameSettings();
    /// 0x086eceee
    UNK_RET StringToBools(char const* a1, int* a2, bool** a3);
    /// 0x086ecf58
    UNK_RET SetAutoSaveDisabled(bool a1);
    /// 0x086ecf92
    UNK_RET GetIniParser();
    /// 0x086ecf9e
    UNK_RET GetHotLoader();
    /// 0x086ed0a8
    UNK_RET GuessResolution(bool a1);
    /// 0x086ed22a
    UNK_RET InfiniteSpinlockAcquireDispatchLockEventSystems();
    /// 0x086ed31c
    UNK_RET ReleaseEventSystemsDispatchLock();
    /// 0x086ed36a
    UNK_RET ReleaseIndependentSystems();
    /// 0x086ed69a
    UNK_RET GetSteamAppID() const;
    /// 0x0a308878
    static UNK_DATA SystemStrings[0x64];
    /// 0x0a3088dc
    static UNK_DATA TextureTypes[0x84];

    // Bit n enables SystemStrings[n] (bit 14 FOW, bit 15 FluidFOW); SetupInitForSpecificSystems fills it.
    unsigned char unk_b0[0xea4];     ///< +0xb0
    unsigned int m_uEnabledSystems;  ///< +0xf54
};

/// 0x086df8fa
UNK_RET DoCheckTempHeap();
/// 0x086eac1c
UNK_RET ActivateDefaultUISkins();
/// 0x086ecfa9
UNK_RET SortDisplayModes(void* a1, void const* a2, void const* a3);
extern "C" {
    /// 0x0a308974
    extern UNK_DATA DeviceList[0x20a0];
}

namespace Civ5NetMessage {
/// 0x0a308970
extern UNK_DATA s_iMaxTurnSliceMessages[0x4];
/// 0x0a308964
extern UNK_DATA s_lastForceSyncTurn[0x4];
/// 0x0a308968
extern UNK_DATA s_lastTurnAICivsProcessedCollected[0x4];
/// 0x0a308960
extern UNK_DATA s_lastTurnCompleteSent[0x4];
/// 0x0a30896c
extern UNK_DATA s_lastTurnInitialAIProcessed[0x4];
}

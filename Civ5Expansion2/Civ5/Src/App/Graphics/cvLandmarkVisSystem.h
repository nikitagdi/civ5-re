/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "FirePlace/FireWorks/FJob.h"
#include "Civ5/Src/App/FrameGlobalsData.h"

class AssetSwappedData;
class FrameGlobalsData;
class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class NonSerialContainer;
class NullEventClass;
class SerialContainer;
class WorldGlobalsData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvEventSystemSerialAccessor;
class cvLandmarkVis;
class cvTerrainSampler;
class cvLandmarkVisDataDef;
class cvLandmarkVisSystem;
class cvLandmarkVis_Init;
class cvLandmark_State;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <class> class cvLandmarkVisSystem_Accessor;
template <int T0> class Event_Int2Type;

/// size ?
class cvLandmarkVisSystem {
public:
    class LandmarkRenderJob;
    /// size ?
    /// vtable 0x0a2fd898
    class LandmarkRenderJob : public FJob {
    public:
        /// 0x0887c3d4
        virtual void Execute(unsigned int a1);
        /// 0x0c035870
        static UNK_DATA sm_pThreadRenderer[0x4];
    };
    /// 0x08879de8
    cvLandmarkVisSystem();
    /// 0x0887a282
    ~cvLandmarkVisSystem();
    /// 0x0887a32a
    UNK_RET Startup();
    /// 0x0887a53a
    UNK_RET Shutdown();
    /// 0x0887a634
    UNK_RET DestroyVises();
    /// 0x0887a6d0
    UNK_RET InitializeVisualizer(cvLandmarkVis& a1, cvGeneralRefHandle<cvLandmarkVisSystem, unsigned int, cvLandmarkVisDataDef, cvLandmarkVisSystem_Accessor>& a2);
    /// 0x0887b2dc
    UNK_RET DestroyFromHandle(cvGeneralRefHandle<cvLandmarkVisSystem, unsigned int, cvLandmarkVisDataDef, cvLandmarkVisSystem_Accessor>& a1);
    /// 0x0887b354
    UNK_RET ReleaseVisualizersOfType(int a1);
    /// 0x0887b3d6
    UNK_RET DestroyVis(cvLandmarkVis& a1);
    /// 0x0887b424
    UNK_RET CreateVisualizersOfType(int a1);
    /// 0x0887b4ba
    UNK_RET OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0887b4f0
    UNK_RET OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0887b62a
    UNK_RET OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0887b660
    UNK_RET OnTestAnimations(GameCoreEventTemplate<Event_Int2Type<2>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0887ba68
    UNK_RET OnLibrarySwap(EventTemplate<Event_Int2Type<15>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0887ba6a
    UNK_RET OnAssetSwap(EventTemplate<Event_Int2Type<128>, AssetSwappedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0887ba6c
    UNK_RET Update(float a1);
    /// 0x0887bb08
    UNK_RET UpdateGlobals(int a1);
    /// 0x0887bd86
    UNK_RET StateSort();
    /// 0x0887bdb4
    UNK_RET OnRenderView(unsigned int a1);
    /// 0x0887be30
    UNK_RET OnRenderShadow(unsigned int a1);
    /// 0x0887beac
    UNK_RET OnRenderReflection(unsigned int a1);
    /// 0x0887bf28
    UNK_RET RenderInternal();
    /// 0x0887bf5a
    UNK_RET RenderInternalMT();
    /// 0x0887c110
    UNK_RET InitHandle(cvGeneralRefHandle<cvLandmarkVisSystem, unsigned int, cvLandmarkVisDataDef, cvLandmarkVisSystem_Accessor>& a1, cvLandmarkVis_Init const& a2);
    /// 0x0887c1b6
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvLandmarkVisSystem, unsigned int, cvLandmarkVisDataDef, cvLandmarkVisSystem_Accessor> a1, cvLandmarkVis_Init const& a2);
    /// 0x0887c258
    UNK_RET DestroyVis(cvGeneralRefHandle<cvLandmarkVisSystem, unsigned int, cvLandmarkVisDataDef, cvLandmarkVisSystem_Accessor> a1, cvGeneralRefHandle<cvLandmarkVisSystem, unsigned int, cvLandmarkVisDataDef, cvLandmarkVisSystem_Accessor> const& a2);
    /// 0x0887c2ea
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvLandmarkVisSystem, unsigned int, cvLandmarkVisDataDef, cvLandmarkVisSystem_Accessor> a1, cvLandmark_State const& a2);
    char unk_0[0xf8];                      ///< +0x0
    FrameGlobalsData m_kFrameGlobals;      ///< +0xf8
    cvTerrainSampler* m_pTerrainSampler;   ///< +0x944
    char unk_948[0x20c];                   ///< +0x948
    unsigned char* m_pTypeRecords;         ///< +0xb54
    char unk_b58[0x7c];                    ///< +0xb58
    unsigned int m_uiTypeRecordCount;      ///< +0xbd4
    char unk_bd8[0x18718];                 ///< +0xbd8
    float m_afDecalBaseMatrix[16];         ///< +0x192f0
    float m_fPrevTime;                     ///< +0x19330
    float m_fTime;                         ///< +0x19334
    float field_19338;                     ///< +0x19338
    float field_1933c;                     ///< +0x1933c
    char unk_19340[0x1];                   ///< +0x19340
    unsigned char m_bDirty;                ///< +0x19341
};

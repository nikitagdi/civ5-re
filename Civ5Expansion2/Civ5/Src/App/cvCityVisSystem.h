/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FrameGlobalsData.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/cvCityPopulationBuildingLibrary.h"
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "Civ5/Src/App/Graphics/cvLandmarkLibrary.h"
#include "Civ5/Src/App/Graphics/cvWonderLibrary.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FTempHeap.h"
#include <d3d9.h>

class BuildingLibrarySwapData;
class CityBuildingStateData;
class DecalVertex;
class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class NonSerialContainer;
class NullEventClass;
class SerialContainer;
class TerrainStencilVertex;
class WonderGlobalVarsData;
class WonderSelectData;
class cvCityVis;
class cvCityVisDataDef;
class cvCityVisSystem;
class cvCityVis_AddBuilding;
class cvCityVis_AddWonderData;
class cvCityVis_BeginUpdate;
class cvCityVis_ClearCity;
class cvCityVis_EditWonderData;
class cvCityVis_EndUpdate;
class cvCityVis_InitCity;
class cvCityVis_ReloadAssets;
class cvCityVis_RemoveWonderData;
class cvCityVis_SetCityState;
class cvCity_FogState;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
class cvWonderVis;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <class> class cvCityVisSystem_Accessor;
template <int T0> class Event_Int2Type;

// Control points of one wall piece; the next message of the same wall repeats the last two points.
/// size 0x4c
class cvCityVis_AddWallSegment {
public:
    FGXVector3 m_aPoints[6];          ///< +0x0
    unsigned char m_ucCount;          ///< +0x48
    unsigned char unk_49[0x3];        ///< +0x49
};

/// size 0x10
class cvCityVis_AddWallTower {
public:
    FGXVector2 m_vPos;                ///< +0x0
    float m_fHeight;                  ///< +0x8
    float m_fHeading;                 ///< +0xc
};

/// size 0x10
class cvCityVis_AddWallGate {
public:
    FGXVector2 m_vPos;                ///< +0x0
    float m_fHeight;                  ///< +0x8
    float m_fHeading;                 ///< +0xc
};

/// size 0x4
class cvCityVis_SetWallScale {
public:
    float m_fScale;                   ///< +0x0
};

/// size ?
class cvCityVis_BuildWall {
};

/// size ?
class cvCityVis_ClearCityWall {
};

// Singleton g_cvCityVisSystem. City and wonder visuals live in index pools; the render queues at +0x10f4 and +0xd35c are filled from them.
/// size 0x1fad4
class cvCityVisSystem {
public:
    class CityRenderJob;
    class WonderRenderJob;
    // One 8-byte entry per camera and pass (index = camera + pass * 2); m_uConstantSet is the BuildingGlobal constant set UpdateGlobals fills.
    /// size 0x8
    struct cvGlobalsSlot {
        unsigned int m_uConstantSet;                                 ///< +0x0
        unsigned int unk_4;                                          ///< +0x4
    };
    /// size ?
    /// vtable 0x0a2fd558
    class CityRenderJob : public FJob {
    public:
        cvCityVis* m_pCityVis;                                       ///< +0x10
        unsigned int m_uCamera;                                      ///< +0x14
        /// 0x08857dd6
        virtual void Execute(unsigned int a1);
        /// 0x0c0199d4
        static UNK_DATA sm_pThreadRenderer[0x4];
    };
    /// size ?
    /// vtable 0x0a2fd574
    class WonderRenderJob : public FJob {
    public:
        cvWonderVis* m_pWonderVis;                                   ///< +0x10
        unsigned int m_uCamera;                                      ///< +0x14
        float m_fPrevTime;                                           ///< +0x18
        float m_fTime;                                               ///< +0x1c
        /// 0x08857e64
        virtual void Execute(unsigned int a1);
        /// 0x0c0199d8
        static UNK_DATA sm_pThreadRenderer[0x4];
    };
    unsigned int field_0;                                            ///< +0x0
    unsigned char unk_4[0x8];                                        ///< +0x4
    unsigned int m_uBuildingSize;                                    ///< +0xc
    // Debug selection of a wonder: city slot and wonder index, -1 when none.
    unsigned int m_uDebugCity;                                       ///< +0x10
    unsigned int m_uDebugWonder;                                     ///< +0x14
    unsigned char unk_18;                                            ///< +0x18
    // Set by every city update.
    unsigned char m_bDirty;                                          ///< +0x19
    unsigned char unk_1a[0x2];                                       ///< +0x1a
    // Four TextControl instances (debug info lines), stride 0x178.
    unsigned char m_aDebugText[0x5e0];                               ///< +0x1c
    unsigned int m_uRandomIndex;                                     ///< +0x5fc
    // Per-building random values 0 to 3, handed out in order by AddBuilding.
    float m_afRandom[0x21];                                          ///< +0x600
    float m_fPrevTime;                                               ///< +0x684
    float m_fTime;                                                   ///< +0x688
    float field_68c;                                                 ///< +0x68c
    float field_690;                                                 ///< +0x690
    // Translation by the map extent then scale (1, 1.1547, 1): hex grid to world; set by OnMapGlobalsChanged.
    D3DXMATRIX m_mMapTransform;                                      ///< +0x694
    FrameGlobalsData m_kFrameGlobals;                                ///< +0x6d4
    unsigned char unk_f20[0x84];                                     ///< +0xf20
    WorldGlobalsData m_kWorldGlobals;                                ///< +0xfa4
    unsigned char unk_10f0[0x4];                                     ///< +0x10f0
    unsigned char unk_10f4[0x224];                                   ///< +0x10f4
    cvGlobalsSlot m_aCityGlobals[6];                                 ///< +0x1318
    unsigned char unk_1348[0xc];                                     ///< +0x1348
    FJobSet m_kCityJobSet;                                           ///< +0x1354
    CityRenderJob* m_pCityJob;                                       ///< +0x1358
    unsigned char unk_135c[0xc000];                                  ///< +0x135c
    unsigned char unk_d35c[0x224];                                   ///< +0xd35c
    cvGlobalsSlot m_aWonderGlobals[6];                               ///< +0xd580
    unsigned char unk_d5b0[0x243c];                                  ///< +0xd5b0
    FJobSet m_kWonderJobSet;                                         ///< +0xf9ec
    WonderRenderJob* m_pWonderJob;                                   ///< +0xf9f0
    unsigned char unk_f9f4[0x10000];                                 ///< +0xf9f4
    unsigned int field_1f9f4;                                        ///< +0x1f9f4
    unsigned int field_1f9f8;                                        ///< +0x1f9f8
    // City pool: free-list head, next unused index, slot array (stride 0x4f0), slots in use.
    unsigned int m_uCityFreeHead;                                    ///< +0x1f9fc
    unsigned int m_uCityNextSlot;                                    ///< +0x1fa00
    cvCityVis* m_pCityVis;                                           ///< +0x1fa04
    unsigned int m_uCityCount;                                       ///< +0x1fa08
    unsigned int field_1fa0c;                                        ///< +0x1fa0c
    unsigned int field_1fa10;                                        ///< +0x1fa10
    unsigned int field_1fa14;                                        ///< +0x1fa14
    unsigned int field_1fa18;                                        ///< +0x1fa18
    unsigned int field_1fa1c;                                        ///< +0x1fa1c
    unsigned int field_1fa20;                                        ///< +0x1fa20
    // Wonder-list pool: one entry per city that has wonders (stride 0x18, +0x10 is the first cvWonderVis index of its list).
    unsigned int field_1fa24;                                        ///< +0x1fa24
    unsigned int field_1fa28;                                        ///< +0x1fa28
    unsigned char* m_pWonderLists;                                   ///< +0x1fa2c
    unsigned int field_1fa30;                                        ///< +0x1fa30
    unsigned int field_1fa34;                                        ///< +0x1fa34
    unsigned int field_1fa38;                                        ///< +0x1fa38
    unsigned int field_1fa3c;                                        ///< +0x1fa3c
    unsigned int field_1fa40;                                        ///< +0x1fa40
    cvCityPopulationBuildingLibrary m_kBuildingLibrary;              ///< +0x1fa44
    cvWonderLibrary m_kWonderLibrary;                                ///< +0x1fa9c
    /// 0x08853d30
    cvCityVisSystem();
    /// 0x0885416e
    ~cvCityVisSystem();
    /// 0x0885423c
    UNK_RET Startup();
    /// 0x088546c2
    UNK_RET SubscribeToEvents();
    /// 0x08854824
    UNK_RET Shutdown();
    /// 0x0885496a
    UNK_RET UnSubscribeToEvents();
    /// 0x08854acc
    UNK_RET DestroyVises();
    /// 0x08854b9e
    UNK_RET ShutdownGame();
    /// 0x08854bb2
    void OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08854c10
    UNK_RET OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08854d4a
    UNK_RET OnLibrarySwap(EventTemplate<Event_Int2Type<17>, BuildingLibrarySwapData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08854e62
    UNK_RET DebugOnWonderGlobalVarsChanged(EventTemplate<Event_Int2Type<25>, WonderGlobalVarsData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08854ef4
    UNK_RET OnToggleDebugInfoDisplay(EventTemplate<Event_Int2Type<19>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08854f9a
    UNK_RET OnTestAnimations(GameCoreEventTemplate<Event_Int2Type<2>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088550c4
    UNK_RET OnBuildingSizeChanged(EventTemplate<Event_Int2Type<125>, CityBuildingStateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088551ba
    UNK_RET DebugOnSelectWonder(EventTemplate<Event_Int2Type<27>, WonderSelectData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08855326
    UNK_RET DestroyCityVis(cvCityVis& a1);
    /// 0x088553de
    void UpdateGlobals(unsigned int uCamera, unsigned int uPass);
    /// 0x0885587e
    void OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088558b4
    cvWonderVis* FindWonder(unsigned int a1, unsigned int a2);
    /// 0x088558e4
    UNK_RET UpdateDebugText();
    /// 0x08855a5c
    UNK_RET ReloadCitiesOfEraAndCulture(EraTypes const& a1, CultureTypes const& a2);
    /// 0x08855b6c
    UNK_RET StateSort();
    /// 0x08855b9a
    UNK_RET OnRenderView(unsigned int a1);
    /// 0x08855c54
    UNK_RET OnRenderShadow(unsigned int a1);
    /// 0x08855d0e
    UNK_RET OnRenderReflection(unsigned int a1);
    /// 0x08855d76
    void Update(float a1);
    /// 0x08855d9a
    UNK_RET RenderInternal();
    /// 0x08855dae
    UNK_RET DrawStaticBuildingsMT();
    /// 0x0885603c
    UNK_RET DrawStaticBuildingsST();
    /// 0x088563aa
    UNK_RET InitMeshDecals(cvCityPopulationBuildingLibrary::cvCityBuildingSet const& a1, unsigned int a2, unsigned int& a3, D3DXMATRIX const& a4);
    /// 0x08856482
    UNK_RET ClearMeshDecals(cvCityPopulationBuildingLibrary::cvCityBuildingSet const& a1, unsigned int a2, unsigned int& a3, D3DXMATRIX const& a4);
    /// 0x0885654c
    UNK_RET DestroyFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1);
    /// 0x088565b0
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor>& a1, cvCityVis_InitCity const& a2);
    /// 0x08856750
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_ClearCity const& a2);
    /// 0x088568c6
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_AddBuilding const& a2);
    /// 0x08856b0a
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_SetCityState const& a2);
    /// 0x08856b68
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_BeginUpdate const& a2);
    /// 0x08856b6a
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_EndUpdate const& a2);
    /// 0x08856c0e
    UNK_RET ReloadBuildings(cvCityVis& a1);
    /// 0x08856ec4
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_AddWonderData const& a2);
    /// 0x0885720c
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_RemoveWonderData const& a2);
    /// 0x08857354
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_EditWonderData const& a2);
    /// 0x08857472
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCity_FogState const& a2);
    /// 0x088574cc
    void UpdateVisFOWState(cvCityVis& a1, unsigned char a2);
    /// 0x088574fe
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_ReloadAssets const& a2);
    /// 0x088575b4
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_AddWallSegment const& a2);
    /// 0x08857636
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_AddWallGate const& a2);
    /// 0x088576aa
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_AddWallTower const& a2);
    /// 0x0885771e
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_ClearCityWall const& a2);
    /// 0x0885782e
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_SetWallScale const& a2);
    /// 0x08857868
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvCityVisSystem, unsigned int, cvCityVisDataDef, cvCityVisSystem_Accessor> const& a1, cvCityVis_BuildWall const& a2);
};

/// 0x08859002 InitializeMeshDecalsForModel<cvCityPopulationBuildingLibrary::cvCityBuildingSet>
/// 0x0887e576 InitializeMeshDecalsForModel<cvLandmarkLibrary::cvLandmarkLibraryEntry>
/// 0x08866f4e InitializeMeshDecalsForModel<cvWonderLibrary::cvWonderLibraryEntry>
template <class T0>
void InitializeMeshDecalsForModel(T0 const& a1, unsigned int a2, unsigned int* a3, unsigned int a4, unsigned int& a5, D3DXMATRIX const& a6, unsigned char a7, FFastVector<DecalVertex, true, 43u, 260u, BaseVector<DecalVertex, true>::FDefaultFastVectorAllocator>& a8, FFastVector<TerrainStencilVertex, false, 43u, 260u, FTempHeapFastVectorAllocator>& a9);
/// 0x08859511 ClearStencilMeshDecalsForModel<cvCityPopulationBuildingLibrary::cvCityBuildingSet>
/// 0x0887e306 ClearStencilMeshDecalsForModel<cvLandmarkLibrary::cvLandmarkLibraryEntry>
/// 0x08867432 ClearStencilMeshDecalsForModel<cvWonderLibrary::cvWonderLibraryEntry>
template <class T0>
void ClearStencilMeshDecalsForModel(T0 const& a1, unsigned int a2, D3DXMATRIX const& a3, unsigned char a4, FFastVector<TerrainStencilVertex, false, 43u, 260u, FTempHeapFastVectorAllocator>& a5);

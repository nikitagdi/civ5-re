/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FUnrolledList_Core.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"

class DynamicGeometryVis;
class FGXVector2;
class FGXVector3;
class FrameGlobalsData;
class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class Material;
class NullEventClass;
class TempForestStencilVert;
class WorldGlobalsData;
class cvDynamicGeometryDataDef;
class cvDynamicGeometryVisSystem;
class cvDynamicGeometry_AddBridge;
class cvDynamicGeometry_AddFence;
class cvDynamicGeometry_AddGreatWallTower;
class cvDynamicGeometry_AddGreatWallVerts;
class cvDynamicGeometry_BuildGreatWall;
class cvDynamicGeometry_ClearGreatWallForestStencil;
class cvDynamicGeometry_InitGreatWall;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class FUnrolledList_Tail_Pointer;
template <class T0, unsigned int T1, unsigned int T2> class NodeArray;
template <class> class cvDynamicGeometryVisSystem_Accessor;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class BuildingShader_CODE;
class CCommandStream;
template <int T0> class VStruct;
}

// One baked mesh: static VB and IB, a surface for the draw, and the recentring translation; the Great Wall also keeps its spline points and towers here.
/// size 0x80
class DynamicGeometryVis {
public:
    unsigned char m_ucFree;           ///< +0x0
    unsigned char unk_1[3];           ///< +0x1
    unsigned int unk_4;               ///< +0x4
    int m_iGreatWallType;             ///< +0x8
    unsigned int m_uiSurface;         ///< +0xc
    unsigned int m_uiVertexBuffer;    ///< +0x10
    unsigned int m_uiIndexBuffer;     ///< +0x14
    float m_fMinX;                    ///< +0x18
    float m_fMinY;                    ///< +0x1c
    float m_fMaxX;                    ///< +0x20
    float m_fMaxY;                    ///< +0x24
    float m_afCenterTranslation[16];  ///< +0x28
    FGXVector3* m_pWallPoints;        ///< +0x68
    unsigned int m_uiNumWallPoints;   ///< +0x6c
    unsigned int m_uiMaxWallPoints;   ///< +0x70
    float* m_pTowers;                 ///< +0x74
    unsigned int m_uiNumTowers;       ///< +0x78
    unsigned int m_uiMaxTowers;       ///< +0x7c
};

/// size ?
class cvDynamicGeometryVisSystem {
public:
    int field_0;                                      ///< +0x0
    WorldGlobalsData m_kWorldGlobals;                 ///< +0x4
    char unk_150[0x8];                                ///< +0x150
    DynamicGeometryVis* m_pSlots;                     ///< +0x158
    unsigned int m_uiNumSlots;                        ///< +0x15c
    char unk_160[4];                                  ///< +0x160
    FireGrafix::BuildingShader_CODE* m_pShader;       ///< +0x164
    FireGrafix::CCommandStream* m_pCommandStream;     ///< +0x168
    float m_fGlobalParam140;                          ///< +0x16c
    float m_fGlobalParam150;                          ///< +0x170

    /// 0x087cfb98
    cvDynamicGeometryVisSystem();
    /// 0x087cfcc0
    ~cvDynamicGeometryVisSystem();
    /// 0x087cfce6
    UNK_RET Startup();
    /// 0x087cfd70
    UNK_RET Shutdown();
    /// 0x087cfdd6
    UNK_RET StartupGame();
    /// 0x087cfdd8
    UNK_RET ShutdownGame();
    /// 0x087cfea2
    UNK_RET InitHandle(cvGeneralRefHandle<cvDynamicGeometryVisSystem, unsigned int, cvDynamicGeometryDataDef, cvDynamicGeometryVisSystem_Accessor>& a1, cvDynamicGeometry_AddBridge const& a2);
    /// 0x087d00b4
    UNK_RET DestroyFromHandle(cvGeneralRefHandle<cvDynamicGeometryVisSystem, unsigned int, cvDynamicGeometryDataDef, cvDynamicGeometryVisSystem_Accessor>& a1);
    /// 0x087d0196
    UNK_RET BuildVisFromDynamicGeometry(DynamicGeometryVis* a1, FireGrafix::VStruct<1> const* a2, unsigned int a3, unsigned long const* a4, unsigned int a5, Material const& a6) const;
    /// 0x087d06d2
    UNK_RET InitHandle(cvGeneralRefHandle<cvDynamicGeometryVisSystem, unsigned int, cvDynamicGeometryDataDef, cvDynamicGeometryVisSystem_Accessor>& a1, cvDynamicGeometry_AddFence const& a2);
    /// 0x087d06d4
    UNK_RET InitHandle(cvGeneralRefHandle<cvDynamicGeometryVisSystem, unsigned int, cvDynamicGeometryDataDef, cvDynamicGeometryVisSystem_Accessor>& a1, cvDynamicGeometry_InitGreatWall const& a2);
    /// 0x087d0860
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvDynamicGeometryVisSystem, unsigned int, cvDynamicGeometryDataDef, cvDynamicGeometryVisSystem_Accessor>& a1, cvDynamicGeometry_AddGreatWallVerts const& a2);
    /// 0x087d08fa
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvDynamicGeometryVisSystem, unsigned int, cvDynamicGeometryDataDef, cvDynamicGeometryVisSystem_Accessor>& a1, cvDynamicGeometry_ClearGreatWallForestStencil const& a2);
    /// 0x087d0a8e
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvDynamicGeometryVisSystem, unsigned int, cvDynamicGeometryDataDef, cvDynamicGeometryVisSystem_Accessor>& a1, cvDynamicGeometry_AddGreatWallTower const& a2);
    /// 0x087d0b08
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvDynamicGeometryVisSystem, unsigned int, cvDynamicGeometryDataDef, cvDynamicGeometryVisSystem_Accessor>& a1, cvDynamicGeometry_BuildGreatWall const& a2);
    /// 0x087d0d9c
    void Update(float a1);
    /// 0x087d0d9e
    void RenderInternal();
    /// 0x087d0da0
    UNK_RET OnRenderView(unsigned int a1);
    /// 0x087d12fe
    UNK_RET OnRenderShadow(unsigned int a1);
    /// 0x087d1868
    void OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d186a
    void OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d186c
    void OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};

/// 0x08a1ed25 AddWorldSpaceMeshToTerrainStencil<FGXVector2*>
/// 0x08a1eb85 AddWorldSpaceMeshToTerrainStencil<FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 54u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator>
/// 0x087d1d9a AddWorldSpaceMeshToTerrainStencil<TempForestStencilVert*>
template <class T0>
void AddWorldSpaceMeshToTerrainStencil(T0 a1, unsigned int a2, unsigned short const* a3, unsigned int a4, unsigned char a5, bool a6);

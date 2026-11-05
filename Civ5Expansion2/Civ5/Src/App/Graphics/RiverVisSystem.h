/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTempHeap.h"
#include <lua.h>

class RiverVisSystem;
class RiverVisSystemDataDef;
struct RiverVisSystem_AddSplines;
class RiverVisSystem_Init;
template <class> class RiverVisSystem_Accessor;
namespace FireGrafix {
template <int T0> class VStruct;
}

namespace FireGrafix {
class CCommandStream;
class WaterShader;
}
class RiverSimSpline;

// Vertex of the river render mesh (VStruct<10>): one pair per tessellation step, left bank then right bank.
/// size 0x20
struct RiverVisVertex {
    float m_fX;     ///< +0x0
    float m_fY;     ///< +0x4
    float m_fZ;     ///< +0x8
    // Distance along the river / 100, starting at 6.4 for rivers that do not end at a mouth.
    float m_fFlowU; ///< +0xc
    // Across-river texture coordinate: left (0.5 - w) * 4 / 5, right ((0.5 + w) * 4 + 1) / 5.
    float m_fAcrossV; ///< +0x10
    float m_fFade;  ///< +0x14
    // 0 on the left bank vertex, 1 on the right bank vertex.
    float m_fSide;  ///< +0x18
    char unk_1c[0x4]; ///< +0x1c
};

// The four sim containers TessellateMesh reads, borrowed only during UpdateFromHandle.
/// size 0x10
struct RiverVisSystem_AddSplines {
    FFastVector<RiverSimSpline, true, 50u, 0u, FTempHeapFastVectorAllocator>* m_pLeftBanks;   ///< +0x0
    FFastVector<RiverSimSpline, true, 50u, 0u, FTempHeapFastVectorAllocator>* m_pRightBanks;  ///< +0x4
    FFastVector<RiverSimSpline, true, 50u, 0u, FTempHeapFastVectorAllocator>* m_pRivers;      ///< +0x8
    FFastVector<FFastVector<float, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator>* m_pHeights;  ///< +0xc
};

/// size 0x30
class RiverVisSystem {
public:
    // The four fields below are set by UpdateFromHandle and cleared (this+0xc, 0, 4) after the build.
    FFastVector<RiverSimSpline, true, 50u, 0u, FTempHeapFastVectorAllocator>* m_pLeftBanks;   ///< +0x0
    FFastVector<RiverSimSpline, true, 50u, 0u, FTempHeapFastVectorAllocator>* m_pRightBanks;  ///< +0x4
    FFastVector<RiverSimSpline, true, 50u, 0u, FTempHeapFastVectorAllocator>* m_pRivers;      ///< +0x8
    FFastVector<FFastVector<float, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator>* m_pHeights;  ///< +0xc
    FireGrafix::CCommandStream* m_pCommandStream;  ///< +0x10
    FireGrafix::WaterShader* m_pShader;            ///< +0x14
    unsigned int m_hSurface;                       ///< +0x18
    unsigned int m_hVertexBuffer;                  ///< +0x1c
    unsigned int m_hIndexBuffer;                   ///< +0x20
    // 32x32 zero-filled texture created in Startup.
    unsigned int m_hBlankTexture;                  ///< +0x24
    unsigned int m_uiTriangleCount;                ///< +0x28
    bool m_bReady;                                 ///< +0x2c
    char unk_2d[0x3];                              ///< +0x2d

    /// 0x088d9490
    RiverVisSystem();
    /// 0x088d94de
    UNK_RET Startup();
    /// 0x088d95fc
    UNK_RET StartupGame();
    /// 0x088d95fe
    UNK_RET ShutdownGame();
    /// 0x088d9652
    UNK_RET Shutdown();
    /// 0x088d96ea
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<RiverVisSystem, unsigned int, RiverVisSystemDataDef, RiverVisSystem_Accessor>& a1, RiverVisSystem_Init a2);
    /// 0x088d96ec
    UNK_RET Update(float a1);
    /// 0x088d96ee
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<RiverVisSystem, unsigned int, RiverVisSystemDataDef, RiverVisSystem_Accessor>& a1, RiverVisSystem_AddSplines a2);
    /// 0x088d97be
    UNK_RET TessellateMesh(FFastVector<unsigned int, true, 50u, 0u, FTempHeapFastVectorAllocator>& a1, FFastVector<FireGrafix::VStruct<10>, true, 50u, 0u, FTempHeapFastVectorAllocator>& a2);
    /// 0x088da2fa
    UNK_RET BuildRenderingBuffers(FFastVector<unsigned int, true, 50u, 0u, FTempHeapFastVectorAllocator>& a1, FFastVector<FireGrafix::VStruct<10>, true, 50u, 0u, FTempHeapFastVectorAllocator>& a2);
    /// 0x088da586
    UNK_RET DestroyFromHandle(cvGeneralRefHandle<RiverVisSystem, unsigned int, RiverVisSystemDataDef, RiverVisSystem_Accessor>& a1);
    /// 0x088da5a4
    UNK_RET GetGeometry(unsigned int& hVertexBuffer, unsigned int& hIndexBuffer, unsigned int& uiTriangleCount);
    /// 0x088da5c8
    UNK_RET Render(unsigned int a1);
};

/// 0x088da9fc
/// lua ?
int LuaThunk_ToggleStrategicView(lua_State* a1);
/// 0x088daa50
UNK_RET ToggleStrategicView();
/// 0x088daa97
int LuaToString_ToggleStrategicView(lua_State* a1);
/// 0x088dab0a
/// lua ?
int LuaThunk_InStrategicView(lua_State* a1);
/// 0x088dab9c
UNK_RET InStrategicView();
/// 0x088dabd8
int LuaToString_InStrategicView(lua_State* a1);
/// 0x088dac4b
/// lua ?
int LuaThunk_ProcessStrategicViewMouseClick(lua_State* a1);
/// 0x088dac78
UNK_RET ProcessStrategicViewMouseClick();
/// 0x088dacaf
int LuaToString_ProcessStrategicViewMouseClick(lua_State* a1);

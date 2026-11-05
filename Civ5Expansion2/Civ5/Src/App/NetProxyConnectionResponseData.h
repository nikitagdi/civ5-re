/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireGrafix/TextureData.h"

namespace FireGrafix {
class CCommandStream;
class SurfaceSet;
}

/// size ?
class NetProxyConnectionResponseData {
public:
    /// 0x08778616
    NetProxyConnectionResponseData();
    /// 0x08778640
    UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087786c8
    UNK_RET SetFromBuffer(FDataStream& a1);
};

namespace FireGrafix {
// The constant set g_pArrowShaderConstants points at; ArrowVisSystem::Render writes ViewProj at +0x0, alpha at +0x40, ambient at +0x50.
/// size ?
class ArrowShaderConstants : public FGXConstantSet {
public:
    /// 0x08779cea
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix {
// The constant set at CompressBC::m_pConstantSet; the callers pass it as this.
/// size ?
class BCDynamics : public FGXConstantSet {
public:
    /// 0x08779d1c
    float* LockDynamic(FireGrafix::CCommandStream* pStream);
    /// 0x08779d4e
    UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix { namespace BGColor {
/// 0x08778994
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix {
// The constant set at BuildingShader_CODE::m_pDynamicConstantSet, created with 0x40-byte buffers.
/// size ?
class BuildingDataDynamic : public FGXConstantSet {
public:
    /// 0x0877909c
    UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
    /// 0x087790ce
    UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix {
// LockGlobal locks 0x160 bytes of the constant set.
/// size ?
class BuildingGlobal : public FGXConstantSet {
public:
    /// 0x08778d4a
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix { namespace BuildingSkinnedDataDynamic {
/// 0x08779100
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779132
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace BumpFilterDynamicData {
/// 0x08779b8c
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779bbe
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace CameraParameters {
/// 0x087787a0
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace CompositeDynamicData {
/// 0x08779fa6
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779fd8
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix {
// LockDynamic locks 0x70 bytes, the size TerrainDecalShader creates its constant set with.
/// size ?
class DecalDynamics : public FGXConstantSet {
public:
    /// 0x08779f42
    void* LockDynamic(FireGrafix::CCommandStream* a1);
    /// 0x08779f74
    void* LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix { namespace ErrorDataDynamic {
/// 0x087793bc
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x087793ee
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace ErrorGlobalConstants {
/// 0x0877938a
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix {
// The constant set at FOWBaseLayer::m_pConstantSet; FluidFOWSystem::RenderFogLayer fills 0xc50 bytes.
/// size ?
class FOWBaseLayerDynamics : public FGXConstantSet {
public:
    /// 0x0877a06e
    void* LockDynamic(FireGrafix::CCommandStream* pStream);
    /// 0x0877a0a0
    UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix {
// The constant set at FOWCell::m_pConstantSet; LockDynamic locks 0x40 bytes, one world-view-projection matrix.
/// size ?
class FOWDynamics : public FGXConstantSet {
public:
    /// 0x08779d80
    void* LockDynamic(FireGrafix::CCommandStream* pStream);
    /// 0x08779db2
    void* LockDynamic(FireGrafix::SurfaceSet* pSurfaceSet);
};
}

namespace FireGrafix {
// The constant set at FOWDrawMask::m_pConstantSet; FluidFOWSystem::RenderFOWMask fills 0x600 bytes, 24 matrices.
/// size ?
class FOWMaskDynamics : public FGXConstantSet {
public:
    /// 0x08779e48
    void* LockDynamic(FireGrafix::CCommandStream* pStream);
    /// 0x08779e7a
    UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix { namespace FXShaderDynamicData {
/// 0x08778e12
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08778e44
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace FXShaderGlobalData {
/// 0x08778cb4
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace FXShaderMaxData {
/// 0x08778c1e
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace FXShaderSkinnedDynamicData {
/// 0x08779038
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x0877906a
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix {
/// size ?
class FogGlobalData_Terrain : public FGXConstantSet {
public:
    /// 0x08778f70
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix { namespace FontShaderDynamic {
/// 0x08779452
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779484
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace FontShaderGlobal {
/// 0x08779420
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix {
/// size ?
class ForestDataStatic : public FGXConstantSet {
public:
    /// 0x087796dc
    void* LockMutable(unsigned int a1);
};
}

namespace FireGrafix { namespace ForestGlobalConstants {
/// 0x087794b6
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace ForkBrightnessDataMutable {
/// 0x0877957e
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace ForkDefaultShaderGlobal {
/// 0x08778ce6
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace ForkGlowDataMutable {
/// 0x087794e8
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace ForkMaskDataMutable {
/// 0x08779358
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace FurParameters {
/// 0x08778bec
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace GridShaderDynamic {
/// 0x08779740
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779772
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace GridShaderGlobal {
/// 0x0877970e
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace IceGlobalConstants {
/// 0x0877a0d2
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace LandmarkShaderDynamicData {
/// 0x08779ac4
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779af6
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace LandmarkShaderGlobalData {
/// 0x08779a92
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace LeaderDecompressHeader {
/// 0x08779ede
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779f10
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace LeaderDynamic {
/// 0x08778836
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08778868
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace LeaderFurDynamic {
/// 0x08778c50
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08778c82
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace LeaderGlowScale {
/// 0x087789c6
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace LeaderLights {
/// 0x087787d2
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace LeaderResolveHeader {
/// 0x0877a104
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x0877a136
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace Leader_Glass_Consts {
/// 0x08778a8e
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_Glass_HackHilight_PREVIEW {
/// 0x08778a5c
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_LightShape_Dynamic {
/// 0x08779c86
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779cb8
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace Leader_MatteParameters {
/// 0x08778a2a
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_VFX_CameraParameters {
/// 0x087788fe
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_VFX_Distort_Parameters {
/// 0x08778ac0
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_VFX_Fog_Parameters {
/// 0x08778bba
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_VFX_Globals {
/// 0x08778b24
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_VFX_LightShaft_Parameters {
/// 0x08778d18
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_VFX_Sky_Parameters {
/// 0x08778b88
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_VFX_Time {
/// 0x0877889a
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace Leader_VFX_Transform {
/// 0x08778930
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08778962
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace Leader_VFX_Wave_Parameters {
/// 0x08778b56
UNK_RET LockMutable(unsigned int a1);
}}

namespace FireGrafix { namespace MenuShaderDynamic {
/// 0x08779678
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x087796aa
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace MenuShaderGlobal {
/// 0x08779b28
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace MiniMapCameraGlobal {
/// 0x08778d7c
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix {
// The constant set at cvMinimapRenderer+0x24; the callers pass it as this. The culture pass writes the owner colour
// as three linear floats at +0x10, +0x14 and +0x18.
/// size ?
class MiniMapDynamic : public FGXConstantSet {
public:
    /// 0x08778dae
    void* LockDynamic(FireGrafix::CCommandStream* pStream);
    /// 0x08778de0
    UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix { namespace MiniMapGlobal {
/// 0x08779006
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix {
// The constant set g_pNormalGlobs points at; the callers pass it as this.
/// size ?
class NormalGlobs : public FGXConstantSet {
public:
    /// 0x08779eac
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix {
// The constant set at DrawSimpleOverlay+0x14; HexOverlay::Render fills 0xc0 bytes, three matrices.
/// size ?
class OverlayDynamics : public FGXConstantSet {
public:
    /// 0x087797d6
    void* LockDynamic(FireGrafix::CCommandStream* pStream);
    /// 0x08779808
    UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix {
// The constant set g_pOverlayGlobals points at; OverlaySystem::Render writes the overlay projection matrix at +0x0.
/// size ?
class OverlayGlobals : public FGXConstantSet {
public:
    /// 0x087797a4
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix { namespace PartDynamics {
/// 0x08779de4
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779e16
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace PostSettings {
/// 0x08778804
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace ReplayMapShaderDataDynamic {
/// 0x0877986c
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x0877989e
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace ReplayMapShaderGlobalConstants {
/// 0x0877983a
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace RiverShaderDynamics {
/// 0x08779902
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779934
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace RiverShaderGlobals {
/// 0x087798d0
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix {
/// size ?
class ShadowGlobals : public FGXConstantSet {
public:
    /// 0x0877a00a
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix { namespace ShadowMapParameters {
/// 0x087788cc
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace SimpleDataDynamic {
/// 0x08779998
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x087799ca
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace SimpleGlobalConstants {
/// 0x08779966
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace SimpleSkinnedDataDynamic {
/// 0x08779a2e
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779a60
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace SimpleSkinnedGlobalConstants {
/// 0x087799fc
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace SkinBlurDynamic {
/// 0x08779c22
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779c54
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace SkinBlurGlobal {
/// 0x08779bf0
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace SkinBlurWeights {
/// 0x087789f8
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix {
// The constant set at TerrainShaderScreen::m_pConstantSet; the callers pass it as this.
/// size ?
class TerrainCompositeDynamicData : public FGXConstantSet {
public:
    /// 0x08778eda
    void* LockDynamic(FireGrafix::CCommandStream* pStream);
    /// 0x08778f0c
    UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix {
// The constant set at TerrainShader::m_pConstantSet; the callers pass it as this. LockDynamic locks 0x70 bytes.
/// size ?
class TerrainDynamicData : public FGXConstantSet {
public:
    /// 0x08778e76
    void* LockDynamic(FireGrafix::CCommandStream* pStream);
    /// 0x08778ea8
    UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix {
/// size ?
class TerrainGlobalData : public FGXConstantSet {
public:
    /// 0x08778af2
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix {
// LockGlobal locks 0x600 bytes; Terrain::SetTransformWeights writes one float4 per texture projection.
/// size ?
class TerrainGlobalTextureData : public FGXConstantSet {
public:
    /// 0x08778f3e
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix {
/// size ?
class TradeArrowGlobals : public FGXConstantSet {
public:
    /// 0x0877a03c
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix { namespace UILineShaderDynamic {
/// 0x08779164
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779196
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace UILineShaderGlobal {
/// 0x08779b5a
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace UIShaderDynamicData {
/// 0x087791c8
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x087791fa
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix {
// The constant set g_pUIShaderGlobalData points at; OverlaySystem::Render writes the overlay projection matrix at +0x40.
/// size ?
class UIShaderGlobalData : public FGXConstantSet {
public:
    /// 0x08778fa2
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix { namespace UIShaderSkinnedDynamicData {
/// 0x0877922c
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x0877925e
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace UnitShaderDynamicData {
/// 0x08779290
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x087792c2
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix {
// LockGlobal locks 0xc0 bytes of the constant set.
/// size ?
class UnitShaderGlobalData : public FGXConstantSet {
public:
    /// 0x08778fd4
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix { namespace UnitShaderSkinnedDynamicData {
/// 0x087792f4
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08779326
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace WaterDynamicData {
/// 0x0877951a
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x0877954c
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix {
// LockGlobal locks 0x250 bytes of the constant set (WaterGlobalConstants).
/// size ?
class WaterGlobalData : public FGXConstantSet {
public:
    /// 0x087795b0
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix {
/// size ?
class WaveData : public FGXConstantSet {
public:
    /// 0x08779614
    void* LockDynamic(FireGrafix::CCommandStream* a1);
    /// 0x08779646
    void* LockDynamic(FireGrafix::SurfaceSet* a1);
};
}

namespace FireGrafix {
/// size ?
class WaveGlobalConsts : public FGXConstantSet {
public:
    /// 0x087795e2
    void* LockGlobal(unsigned int a1);
};
}

namespace FireGrafix {
// LockGlobal locks 0x40 bytes of the constant set (WorldCurrentsConstants).
/// size ?
class WorldCurrentsData : public FGXConstantSet {
public:
    /// 0x0877a168
    void* LockGlobal(unsigned int a1);
};
}

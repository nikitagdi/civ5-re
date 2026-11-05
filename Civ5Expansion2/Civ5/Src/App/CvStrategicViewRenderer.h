/// origin guess
#pragma once

class FGXVector3;
namespace FireGrafix {
class MapShaderGlobalConstantsConsts;
}

/// size ?
class CvStrategicViewRenderer {
public:
    class SVSurface;
    /// size ?
    class SVSurface {
    public:
        /// 0x08c87f5e
        SVSurface();
    };
    /// 0x08c8537c
    CvStrategicViewRenderer();
    /// 0x08c8560c
    ~CvStrategicViewRenderer();
    /// 0x08c8560e
    UNK_RET StartUp(bool a1);
    /// 0x08c86510
    UNK_RET CreateSurface(char const* a1, unsigned int a2, CvStrategicViewRenderer::SVSurface& a3, bool a4);
    /// 0x08c86820
    UNK_RET SetContinentBorderTypes(FireGrafix::MapShaderGlobalConstantsConsts* a1) const;
    /// 0x08c86912
    UNK_RET ShutDown();
    /// 0x08c86dac
    UNK_RET SafeReleaseSurface(CvStrategicViewRenderer::SVSurface& a1) const;
    /// 0x08c86e08
    UNK_RET ReloadTextures();
    /// 0x08c870e6
    UNK_RET UpdateTerrainTextures();
    /// 0x08c8714e
    UNK_RET UpdateFeatureTextures();
    /// 0x08c871b8
    UNK_RET UpdateNaturalWonderTextures();
    /// 0x08c87222
    UNK_RET UpdateResourceTextures();
    /// 0x08c8728c
    UNK_RET UpdateImprovementTextures();
    /// 0x08c872f6
    UNK_RET UpdateUnitTextures();
    /// 0x08c87360
    UNK_RET SetTerrainTextures(char const** a1, unsigned int a2);
    /// 0x08c873dc
    UNK_RET SetContinentTypeColors(FGXVector3 const* a1, unsigned int a2);
    /// 0x08c874a0
    UNK_RET SetFeatureTextures(char const** a1, unsigned int a2);
    /// 0x08c8751c
    UNK_RET SetNaturalWonderTextures(char const** a1, unsigned int a2);
    /// 0x08c87598
    UNK_RET SetResourceTextures(char const** a1, unsigned int a2);
    /// 0x08c87614
    UNK_RET SetImprovementTextures(char const** a1, unsigned int a2);
    /// 0x08c87690
    UNK_RET SetUnitTextures(char const** a1, unsigned int a2);
    /// 0x08c8770c
    UNK_RET SetUnitBackgroundTextures(char const** a1, char const** a2, unsigned int a3);
    /// 0x08c877d4
    UNK_RET SetMapIconTextures(char const** a1, unsigned int a2);
    /// 0x08c8783e
    UNK_RET SetTeamColors(FGXVector3 const* a1, FGXVector3 const* a2, unsigned int a3);
    /// 0x08c87938
    UNK_RET SetTeamColor(FGXVector3 const& a1, FGXVector3 const& a2, unsigned int a3);
    /// 0x08c87a16
    UNK_RET SetUnitCombatTypeColors(FGXVector3 const* a1, unsigned int a2);
    /// 0x08c87c3c
    UNK_RET AddTerrainTexture(char const* a1);
    /// 0x08c87cae
    UNK_RET AddFeatureTexture(char const* a1);
    /// 0x08c87d20
    UNK_RET AddResourceTexture(char const* a1);
    /// 0x08c87d92
    UNK_RET AddImprovementTexture(char const* a1);
    /// 0x08c87e04
    UNK_RET AddUnitTexture(char const* a1);
    /// 0x08c87e78
    UNK_RET UpdateContinentTypeColors();
    /// 0x08c87f00
    UNK_RET RefreshGlobalShaderConsts(FireGrafix::MapShaderGlobalConstantsConsts* a1) const;
    /// 0x09238d48
    static UNK_DATA MaxCombatTypeColors[0x4];
    /// 0x09238d40
    static UNK_DATA MaxContinentTypes[0x4];
    /// 0x09238d4c
    static UNK_DATA MaxFeatureTypes[0x4];
    /// 0x09238d58
    static UNK_DATA MaxImprovementTypes[0x4];
    /// 0x09238d64
    static UNK_DATA MaxMapIcons[0x4];
    /// 0x09238d50
    static UNK_DATA MaxNaturalWonderTypes[0x4];
    /// 0x09238d54
    static UNK_DATA MaxResourceTypes[0x4];
    /// 0x09238d44
    static UNK_DATA MaxTeamColors[0x4];
    /// 0x09238d3c
    static UNK_DATA MaxTerrainTypes[0x4];
    /// 0x09238d60
    static UNK_DATA MaxUnitBackgrounds[0x4];
    /// 0x09238d5c
    static UNK_DATA MaxUnitTypes[0x4];
};

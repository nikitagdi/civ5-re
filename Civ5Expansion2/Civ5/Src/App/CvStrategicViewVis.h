/// origin guess
#pragma once

class CvStrategicViewCamera;
class CvStrategicViewRenderer;
class CvStrategicViewUnitData;
class FGXUInt2;
class FGXVector2;
class FGXVector4;

/// size ?
class CvStrategicViewVis {
public:
    class CullingRect;
    class Location;
    /// 0x08c87f8a
    CvStrategicViewVis(CvStrategicViewRenderer const& a1);
    /// 0x08c880b4
    ~CvStrategicViewVis();
    /// 0x08c880f2
    UNK_RET StartUp();
    /// 0x08c8818c
    UNK_RET ShutDown();
    /// 0x08c88234
    UNK_RET SetMapSize(unsigned int a1, unsigned int a2);
    /// 0x08c882de
    UNK_RET SetWindowSize(unsigned int a1, unsigned int a2);
    /// 0x08c88372
    UNK_RET GetMapWidthWorldSpace() const;
    /// 0x08c883b0
    UNK_RET GetMapHeightWorldSpace() const;
    /// 0x08c883ee
    UNK_RET AdjustCameraForWorldWrap();
    /// 0x08c884ae
    UNK_RET CameraPhase(bool a1);
    /// 0x08c88518
    UNK_RET Render(unsigned int a1) const;
    /// 0x08c88a80
    UNK_RET GetWorldWrapAdjustedCamera(CvStrategicViewCamera& a1) const;
    /// 0x08c88af4
    UNK_RET GetCullingRect(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect& a2) const;
    /// 0x08c88c4c
    UNK_RET RenderWorld(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c88f1e
    UNK_RET RenderUnitsCrossingWorldWrap() const;
    /// 0x08c89686
    UNK_RET RenderVisibilityBox(unsigned int a1, FGXVector2 a2, FGXVector2 a3) const;
    /// 0x08c897c8
    UNK_RET GetPlotAt(FGXVector2 const& a1, CvStrategicViewVis::Location& a2) const;
    /// 0x08c89a78
    UNK_RET GetVertexAt(FGXVector2 const& a1, CvStrategicViewVis::Location& a2) const;
    /// 0x08c89d0e
    UNK_RET GetVertexCullingRect(CvStrategicViewVis::CullingRect const& a1, CvStrategicViewVis::CullingRect& a2) const;
    /// 0x08c89d3a
    UNK_RET GetCityBannerAt(FGXVector2 const& a1, int& a2) const;
    /// 0x08c89da8
    UNK_RET GridToWorld(CvStrategicViewVis::Location const& a1, FGXVector2 const& a2, FGXVector2& a3) const;
    /// 0x08c89e28
    UNK_RET WorldToGrid(FGXVector2 const& a1, CvStrategicViewVis::Location& a2, FGXVector2& a3) const;
    /// 0x08c89f2e
    UNK_RET GetPlotPosition(CvStrategicViewVis::Location const& a1, FGXVector2& a2) const;
    /// 0x08c89fb4
    UNK_RET GetPlotPosition(CvStrategicViewVis::Location const& a1, FGXVector2& a2, unsigned int a3);
    /// 0x08c8a036
    UNK_RET FindNeighbors(CvStrategicViewVis::Location const& a1, CvStrategicViewVis::Location* a2);
    /// 0x08c8a0b6
    UNK_RET FindPlotsNeighboringVertex(CvStrategicViewVis::Location const& a1, CvStrategicViewVis::Location* a2);
    /// 0x08c8a120
    UNK_RET GetActiveCameras(CvStrategicViewCamera* a1) const;
    /// 0x08c8a246
    UNK_RET RenderTerrain(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8a2ba
    UNK_RET RenderRivers(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8a5a4
    UNK_RET RenderFeatures(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8a7ca
    UNK_RET RenderNaturalWonders(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8a9ec
    UNK_RET RenderPlotHeight(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8ac80
    UNK_RET RenderRoads(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8aeee
    UNK_RET RenderResources(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8b15a
    UNK_RET RenderImprovements(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8b5ba
    UNK_RET RenderCities(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8b79a
    UNK_RET RenderMapIcons(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8b98a
    UNK_RET RenderContinentBorders(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8bdb0
    UNK_RET RenderCultureBorders(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8c1fa
    UNK_RET RenderSelection(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8c416
    UNK_RET RenderFOW(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8c70e
    UNK_RET RenderGlowEffect(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8c9d2
    UNK_RET RenderBorderGlowEffect(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8cc36
    UNK_RET RenderUnits(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8cc5c
    UNK_RET RenderCustomVisuals(CvStrategicViewCamera const& a1) const;
    /// 0x08c8cdf4
    UNK_RET RenderCityText(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8d5aa
    UNK_RET RenderRiverWidgets(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8d940
    UNK_RET RenderTradeRoutes(CvStrategicViewCamera const& a1) const;
    /// 0x08c8da6a
    UNK_RET RenderTerrainNormal(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8dda8
    UNK_RET RenderTerrainContinentType(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8e1ac
    UNK_RET RenderTerrainCultureType(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8e5e0
    UNK_RET RenderTerrainUnitCulture(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8ea3a
    UNK_RET RenderTerrainUnitType(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8ef60
    UNK_RET RenderTerrainUnitDomain(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8f5ce
    UNK_RET RenderTerrainUnitCombatType(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c8fc7c
    UNK_RET RenderTerrainResourceUsage(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c900f8
    UNK_RET RenderTerrainTradeRoutes(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c904e0
    UNK_RET RenderTerrainPassable(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c908f4
    UNK_RET GetContinentBorderForPlot(CvStrategicViewVis::CullingRect const& a1, CvStrategicViewVis::Location const& a2, unsigned int& a3) const;
    /// 0x08c90d66
    UNK_RET GetCultureBorderForPlot(CvStrategicViewVis::CullingRect const& a1, CvStrategicViewVis::Location const& a2, unsigned int& a3) const;
    /// 0x08c911d8
    UNK_RET RenderSmallUnits(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c91658
    UNK_RET RenderLargeUnits(CvStrategicViewCamera const& a1, CvStrategicViewVis::CullingRect const& a2) const;
    /// 0x08c91bd2
    UNK_RET RenderUnit(CvStrategicViewUnitData const& a1, FGXVector4 const& a2) const;
    /// 0x08c921d6
    UNK_RET GetRiverPieceForVertexWithWorldWrap(CvStrategicViewVis::CullingRect const& a1, CvStrategicViewVis::Location const& a2, FGXUInt2& a3) const;
    /// 0x08c926a6
    UNK_RET GetRiverPieceForVertex(CvStrategicViewVis::CullingRect const& a1, CvStrategicViewVis::Location const& a2, FGXUInt2& a3) const;
    /// 0x08c92cdc
    UNK_RET GetVisibleRegion(CvStrategicViewCamera const& a1, FGXVector2& a2, FGXVector2& a3) const;
};

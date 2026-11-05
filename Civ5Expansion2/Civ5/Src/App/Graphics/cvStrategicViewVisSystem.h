/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"
#include <lua.h>

class CameraRephasedData;
class CvCamera;
class CvStrategicViewCamera;
class CvStrategicViewPlotData;
class CvStrategicViewPlotRange;
class CvStrategicViewVis;
class FGXInt2;
class FGXVector2;
class FGXVector3;
class LinearEventDispatcher;
class LocalMachineContainer;
class NonSerialContainer;
class NullEventClass;
class SystemUpdateUIData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class cvStrategicViewVisSystem {
public:
    /// 0x088dc402
    cvStrategicViewVisSystem();
    /// 0x088dc4a6
    ~cvStrategicViewVisSystem();
    /// 0x088dad22
    UNK_RET ProcessMouseClick(FGXVector2 const& a1);
    /// 0x088db3e6
    UNK_RET MouseOverResource() const;
    /// 0x088dc4d8
    UNK_RET Startup();
    /// 0x088dc882
    UNK_RET SetTerrainTextures(Database::Results& a1);
    /// 0x088dc908
    UNK_RET SetUnitCombatTypeColors();
    /// 0x088dc9d0
    UNK_RET SetResourceTextures(Database::Results& a1);
    /// 0x088dca5e
    UNK_RET SetFeatureTextures(Database::Results& a1);
    /// 0x088dcae4
    UNK_RET SetImprovementTextures(Database::Results& a1);
    /// 0x088dcb6a
    UNK_RET SetUnitTextures(Database::Results& a1);
    /// 0x088dcc7e
    UNK_RET SetMovementPathTextures();
    /// 0x088dcd16
    UNK_RET GetRenderData(CvStrategicViewPlotRange const& a1, CvStrategicViewPlotData* a2);
    /// 0x088de06a
    UNK_RET OnGraphicsOptionsChanged(EventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088de0a8
    UNK_RET OnSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088de110
    UNK_RET OnCameraRephased(EventTemplate<Event_Int2Type<59>, CameraRephasedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088de160
    UNK_RET Shutdown();
    /// 0x088de35a
    UNK_RET StartupGame();
    /// 0x088de6d4
    UNK_RET SetTeamColors();
    /// 0x088dea18
    UNK_RET ShutdownGame();
    /// 0x088deb20
    UNK_RET ReloadTextures();
    /// 0x088dee12
    UNK_RET Render(unsigned int a1);
    /// 0x088defa0
    UNK_RET SyncStrategicViewCamera(CvCamera const& a1, CvStrategicViewVis& a2) const;
    /// 0x088df03e
    UNK_RET UpdateMouseover();
    /// 0x088df82e
    UNK_RET UpdateMovingUnits();
    /// 0x088dfb36
    UNK_RET UpdateCities();
    /// 0x088e018e
    UNK_RET UpdateRoads();
    /// 0x088e07ae
    UNK_RET UpdateMovementPath();
    /// 0x088e171a
    UNK_RET UpdateTradeRoutes();
    /// 0x088e1852
    UNK_RET GetPlot(FGXVector2 const& a1, FGXInt2& a2) const;
    /// 0x088e1934
    UNK_RET GetWorldSpace(FGXVector2 const& a1, FGXVector3& a2) const;
    /// 0x088e1a6e
    UNK_RET GetWorldAnchorPos(CvStrategicViewCamera const& a1, FGXVector2 const& a2, FGXVector2& a3);
    /// 0x088e1b2a
    unsigned int GetActiveCameras(CvStrategicViewCamera* a1);
    /// 0x088e1b9a
    UNK_RET GetVisibleRegion(CvStrategicViewCamera const& a1, CvStrategicViewPlotRange& a2) const;
    /// 0x088e1bec
    UNK_RET GetVisibleRegion(CvStrategicViewCamera const& a1, FGXVector2& a2, FGXVector2& a3) const;
    /// 0x088e1c8c
    UNK_RET Zoom(FGXVector2 const& a1, bool a2);
    /// 0x088e2016
    UNK_RET SetContinentTypeColors();
    /// 0x088e20e6
    UNK_RET CalcScale(CvCamera const& a1) const;
    /// 0x088e2188
    UNK_RET SetMouseOverUnit(ICvUnit1* a1);
    /// 0x088e21e6
    UNK_RET AddCustomVisual(unsigned int a1, FGXInt2 const& a2, float a3, bool a4);
    /// 0x088e2268
    UNK_RET BuildMovementPathConnection(unsigned int a1, FGXInt2 const& a2, FGXInt2 const& a3);
    /// 0x088e26dc
    UNK_RET AddCustomVisual(unsigned int a1, FGXVector2 const& a2, float a3, bool a4);
    /// 0x088e273a
    UNK_RET GetMouseOverUnit();
};

/// 0x088db301
/// lua ?
int LuaThunk_MouseOverStrategicViewResource(lua_State* a1);
/// 0x088db32e
UNK_RET MouseOverStrategicViewResource();
/// 0x088db373
int LuaToString_MouseOverStrategicViewResource(lua_State* a1);
/// 0x088db578
UNK_RET OverlayNone();
/// 0x088db595
UNK_RET OverlayCulture();
/// 0x088db5b2
UNK_RET OverlayPassableTerrain();
/// 0x088db5cf
UNK_RET OverlayUnitCulture();
/// 0x088db5ec
UNK_RET OverlayUnitType();
/// 0x088db609
UNK_RET OverlayUnitDomain();
/// 0x088db626
UNK_RET OverlayUnitCombatType();
/// 0x088db643
UNK_RET OverlayTradeRoutes();
/// 0x088db660
UNK_RET OverlayResourceUsage();
/// 0x088db67d
/// lua ?
int LuaThunk_GetStrategicViewOverlays(lua_State* a1);
/// 0x088db691
int GetStrategicViewOverlays(lua_State* a1);
/// 0x088db702
int LuaToString_GetStrategicViewOverlays(lua_State* a1);
/// 0x088db761
/// lua ?
int LuaThunk_SetStrategicViewOverlay(lua_State* a1);
/// 0x088db7f3
UNK_RET SetStrategicViewOverlay(int a1);
/// 0x088db818
int LuaToString_SetStrategicViewOverlay(lua_State* a1);
/// 0x088db899
UNK_RET CreateLegendEntry(lua_State* a1, char const* a2, float a3, float a4, float a5);
/// 0x088db9dc
/// lua ?
int LuaThunk_GetOverlayLegend(lua_State* a1);
/// 0x088db9f0
int GetOverlayLegend(lua_State* a1);
/// 0x088dbf05
int LuaToString_GetOverlayLegend(lua_State* a1);
/// 0x088dbf64
UNK_RET IconsAll();
/// 0x088dbf81
UNK_RET IconsNone();
/// 0x088dbf9e
UNK_RET IconsUnits();
/// 0x088dbfc8
UNK_RET IconsResources();
/// 0x088dbfef
UNK_RET IconsImprovements();
/// 0x088dc019
/// lua ?
int LuaThunk_GetStrategicViewIconSettings(lua_State* a1);
/// 0x088dc02d
int GetStrategicViewIconSettings(lua_State* a1);
/// 0x088dc09e
int LuaToString_GetStrategicViewIconSettings(lua_State* a1);
/// 0x088dc0fd
/// lua ?
int LuaThunk_SetStrategicViewIconSetting(lua_State* a1);
/// 0x088dc12c
UNK_RET SetStrategicViewIconSetting(int a1);
/// 0x088dc151
int LuaToString_SetStrategicViewIconSetting(lua_State* a1);
/// 0x088dc1d2
/// lua ?
int LuaThunk_StrategicViewShowFeatures(lua_State* a1);
/// 0x088dc26c
UNK_RET StrategicViewShowFeatures(bool a1);
/// 0x088dc2a6
int LuaToString_StrategicViewShowFeatures(lua_State* a1);
/// 0x088dc327
/// lua ?
int LuaThunk_StrategicViewShowFogOfWar(lua_State* a1);
/// 0x088dc356
UNK_RET StrategicViewShowFogOfWar(bool a1);
/// 0x088dc381
int LuaToString_StrategicViewShowFogOfWar(lua_State* a1);

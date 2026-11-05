/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include <steam.h>

class ReconnectInfo;
class TransportType;
namespace FireGrafix {
enum DisplayMode : int;
}

/// size ?
class CvInitMgr {
public:
    /// 0x08725548
    CvInitMgr();
    /// 0x0872554a
    ~CvInitMgr();
    /// 0x0872554c
    UNK_RET GetInstance();
    /// 0x0872557c
    UNK_RET InitGame();
    /// 0x08725740
    UNK_RET SetupRandomGameVariables();
    /// 0x08725efe
    UNK_RET PadAIPlayers();
    /// 0x0872615a
    UNK_RET SetupRandomPlayers();
    /// 0x08726daa
    UNK_RET End();
    /// 0x08726f02
    UNK_RET GameCoreNew();
    /// 0x087275ce
    UNK_RET LaunchGame();
    /// 0x0a3e123c
    static UNK_DATA m_pInst[0x4];
};

/// 0x0872d8d4
UNK_RET operator<<(FDataStream& a1, CSteamID a2);
/// 0x0872d90d
UNK_RET operator>>(FDataStream& a1, CSteamID& a2);
/// 0x0872d94a
UNK_RET operator<<(FDataStream& a1, TransportType const& a2);
/// 0x0872d981
UNK_RET operator>>(FDataStream& a1, TransportType& a2);
/// 0x0872d9bb
UNK_RET operator<<(FDataStream& a1, ReconnectInfo const& a2);
/// 0x0872da37
UNK_RET operator>>(FDataStream& a1, ReconnectInfo& a2);
extern "C" {
    /// 0x0a3e1424
    extern UNK_DATA g_GraphicsProfile[0x4];
    /// 0x0a3e1850
    extern UNK_DATA s_reconnectInfo[0x1c];
    /// 0x0a3e186c
    extern UNK_DATA s_reconnectInfoWasCached[0x1];
}

namespace cvOptionsManager {
/// 0x08727e31
UNK_RET disp_mode_sort(void const* a1, void const* a2);
/// 0x08727e69
UNK_RET patch_redundant_mode(FireGrafix::DisplayMode const& a1);
/// 0x08727f4e
UNK_RET SortAndCullGraphicsModes();
/// 0x08728c5c
UNK_RET ValidateResolutionOptions(unsigned int a1);
/// 0x08728d0a
UNK_RET ValidateGraphicsOptions(unsigned int a1);
/// 0x087290fa
UNK_RET LoadGameOptions();
/// 0x0872a3fc
UNK_RET LoadGraphicsOptions();
/// 0x0872be0a
UNK_RET ResetTutorial();
/// 0x0872be42
UNK_RET ResetDefaultGameOptions();
/// 0x0872bf90
UNK_RET ResetDefaultGraphicsOptions();
/// 0x0872c43d
UNK_RET UpdatePreGameOptions();
/// 0x0a3e15f0
extern UNK_DATA g_GameOptionsCache[0xa4];
/// 0x0a3e154c
extern UNK_DATA g_GameOptionsStore[0xa4];
/// 0x0a3e13e8
extern UNK_DATA g_GraphicsIniParser[0x3c];
/// 0x0a3e1510
extern UNK_DATA g_UserIniParser[0x3c];
}

namespace cvOptionsManager { namespace GameOptions {
/// 0x0872a31a
UNK_RET SyncGameOptionsCache();
/// 0x0872c67b
UNK_RET CommitGameOptions(bool a1);
/// 0x0872d6b6
UNK_RET SetBindMouseMode_Cached(unsigned int a1);
}}

namespace cvOptionsManager { namespace GraphicsOptions {
/// 0x08727d8c
UNK_RET GetFullscreen();
/// 0x08727da0
UNK_RET SetFullscreen(bool a1);
/// 0x0872d6e8
UNK_RET IsGPUTextureDecodeSupported();
/// 0x0a3e1428
extern UNK_DATA dwDeviceFlags[0x4];
}}

namespace cvOptionsManager { namespace GraphicsOptions { namespace CachedOptions {
/// 0x0872803e
UNK_RET SyncGraphicsOptionsCache();
/// 0x08728142
UNK_RET CommitRuntimeGraphicsOptions();
/// 0x08728182
UNK_RET CommitGraphicsOptions();
/// 0x08728736
UNK_RET HasUserChangedGraphicsOptions();
/// 0x08728891
UNK_RET HasMaxGraphicsOptions();
/// 0x0a3e145a
extern UNK_DATA bFadeShadows[0x1];
/// 0x0a3e142c
extern UNK_DATA bHDStrategicView[0x1];
/// 0x0a3e1459
extern UNK_DATA bMinimizeGrayTiles[0x1];
/// 0x0a3e1458
extern UNK_DATA bVSync[0x1];
/// 0x0a3e145c
extern UNK_DATA leaderSettings[0x34];
/// 0x0a3e143c
extern UNK_DATA uFOWLevel[0x4];
/// 0x0a3e1454
extern UNK_DATA uNumContinents[0x4];
/// 0x0a3e1430
extern UNK_DATA uOverlayLevel[0x4];
/// 0x0a3e1438
extern UNK_DATA uReflectionLevel[0x4];
/// 0x0a3e1434
extern UNK_DATA uShadowLevel[0x4];
/// 0x0a3e1448
extern UNK_DATA uShadowQuality[0x4];
/// 0x0a3e1440
extern UNK_DATA uTerrainDetailLevel[0x4];
/// 0x0a3e1444
extern UNK_DATA uTerrainTessellationLevel[0x4];
/// 0x0a3e1450
extern UNK_DATA uTextureQuality[0x4];
/// 0x0a3e144c
extern UNK_DATA uWaterQuality[0x4];
}}}

namespace cvOptionsManager { namespace GraphicsOptions { namespace CachedResolutionOptions {
/// 0x087289df
UNK_RET SyncResolutionOptionsCache();
/// 0x08728a30
UNK_RET CommitResolutionOptions();
/// 0x08728c96
UNK_RET HasUserChangedResolution();
/// 0x0a3e14fc
extern UNK_DATA bFullScreen[0x1];
/// 0x0a3e1508
extern UNK_DATA i2WindowResolution[0x8];
/// 0x0a3e14f8
extern UNK_DATA uAASamples[0x4];
/// 0x0a3e14f4
extern UNK_DATA uRefreshRate[0x4];
/// 0x0a3e1500
extern UNK_DATA uResolutionID[0x4];
}}}

namespace cvOptionsManager { namespace GraphicsOptions { namespace WrittenOptions {
/// 0x087288d9
UNK_RET SyncWrittenOptions();
/// 0x0a3e14be
extern UNK_DATA bFadeShadows[0x1];
/// 0x0a3e1490
extern UNK_DATA bHDStrategicView[0x1];
/// 0x0a3e14bd
extern UNK_DATA bMinimizeGrayTiles[0x1];
/// 0x0a3e14bc
extern UNK_DATA bVSync[0x1];
/// 0x0a3e14c0
extern UNK_DATA leaderSettings[0x34];
/// 0x0a3e14a0
extern UNK_DATA uFOWLevel[0x4];
/// 0x0a3e14b8
extern UNK_DATA uNumContinents[0x4];
/// 0x0a3e1494
extern UNK_DATA uOverlayLevel[0x4];
/// 0x0a3e149c
extern UNK_DATA uReflectionLevel[0x4];
/// 0x0a3e1498
extern UNK_DATA uShadowLevel[0x4];
/// 0x0a3e14ac
extern UNK_DATA uShadowQuality[0x4];
/// 0x0a3e14a4
extern UNK_DATA uTerrainDetailLevel[0x4];
/// 0x0a3e14a8
extern UNK_DATA uTerrainTessellationLevel[0x4];
/// 0x0a3e14b4
extern UNK_DATA uTextureQuality[0x4];
/// 0x0a3e14b0
extern UNK_DATA uWaterQuality[0x4];
}}}

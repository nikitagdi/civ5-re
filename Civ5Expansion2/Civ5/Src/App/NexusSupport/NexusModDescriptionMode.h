/// origin guess
#pragma once
#include "Civ5/Src/App/NexusSupport/Mode.h"

/// size ?
/// vtable 0x0a3006b0
class NexusModDescriptionMode : public Nexus::Mode {
public:
    /// 0x08b5a2f8
    NexusModDescriptionMode();
    /// 0x08b5a35a
    /// deleting 0x08b5a324
    virtual ~NexusModDescriptionMode();
    /// 0x08b60460
    virtual UNK_RET Shutdown();
    /// 0x08b5a38a
    virtual UNK_RET Startup();
    /// 0x08b60462
    virtual UNK_RET Update();
    /// 0x08b5ac48
    UNK_RET GetMapVersion();
    /// 0x08b5ac7e
    UNK_RET SendMods();
    /// 0x08b5af58
    UNK_RET EnableMod(char const* a1, bool a2);
    /// 0x08b5afbe
    UNK_RET EnableModWithVersion(char const* a1, int a2, bool a3);
    /// 0x08b5b016
    UNK_RET ActiveEnabledMods();
    /// 0x08b5b0aa
    UNK_RET SendGlobals();
    /// 0x08b5b10a
    UNK_RET SendTerrainData();
    /// 0x08b5b830
    UNK_RET SendFeatureData();
    /// 0x08b5bf0a
    UNK_RET SendFeatureRestrictions();
    /// 0x08b5c1ce
    UNK_RET SendResourceData();
    /// 0x08b5c7b6
    UNK_RET SendResourceRestrictions();
    /// 0x08b5ca9a
    UNK_RET SendImprovementData();
    /// 0x08b5cffc
    UNK_RET SendImprovementRestrictions();
    /// 0x08b5d276
    UNK_RET SendUnitTypes();
    /// 0x08b5d8c0
    UNK_RET SendBuildingTypes();
    /// 0x08b5da8c
    UNK_RET SendCivilizationTypes();
    /// 0x08b5df3e
    UNK_RET SendCivilizationCityNames();
    /// 0x08b5e666
    UNK_RET SendTeamColors();
    /// 0x08b5e9fe
    UNK_RET SendEras();
    /// 0x08b5ebca
    UNK_RET SendHandicaps();
    /// 0x08b5ee0c
    UNK_RET SendGameSpeeds();
    /// 0x08b5f04e
    UNK_RET SendTechs();
    /// 0x08b5f28a
    UNK_RET SendPolicies();
    /// 0x08b5f458
    UNK_RET SendPromotions();
    /// 0x08b5f626
    UNK_RET SendVictoryTypes();
    /// 0x08b5f7f4
    UNK_RET SendGameOptions();
    /// 0x08b5fa1e
    UNK_RET SendWorldTypes();
    /// 0x08b5fdfc
    UNK_RET SendMapScripts();
    /// 0x08b60434
    UNK_RET SendCompletionMessage();
};

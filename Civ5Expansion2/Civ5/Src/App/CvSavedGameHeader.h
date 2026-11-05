/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include <Windows.h>
#include <memory>
#include <vector>

class CvSavedGameHeader;

/// size ?
class CvSavedGameHeader {
public:
    class DlcInfo;
    class ModInfo;
    /// size ?
    class DlcInfo {
    public:
        /// 0x08993ac0
        DlcInfo();
        /// 0x08993af4
        DlcInfo(_GUID a1, int a2, char const* a3);
        /// 0x08993b50
        DlcInfo(CvSavedGameHeader::DlcInfo const& a1);
        /// 0x08993bae
        UNK_RET operator=(CvSavedGameHeader::DlcInfo const& a1);
        /// 0x08993c10
        UNK_RET operator==(CvSavedGameHeader::DlcInfo const& a1) const;
        /// 0x08993c7c
        UNK_RET GetId() const;
        /// 0x08993c9a
        UNK_RET GetDlcName() const;
        /// 0x08993ca2
        UNK_RET GetVersion() const;
    };
    /// size ?
    class ModInfo {
    public:
        /// 0x08993f26
        ModInfo();
        /// 0x08993f58
        ModInfo(char const* a1, int a2, char const* a3);
        /// 0x08993fc0
        ModInfo(CvSavedGameHeader::ModInfo const& a1);
        /// 0x0899402c
        UNK_RET operator=(CvSavedGameHeader::ModInfo const& a1);
        /// 0x0899409a
        UNK_RET operator==(CvSavedGameHeader::ModInfo const& a1) const;
        /// 0x089940e4
        UNK_RET GetModId() const;
        /// 0x089940ea
        UNK_RET GetVersion() const;
        /// 0x089940f6
        UNK_RET GetModName() const;
    };
    /// 0x0899443e
    CvSavedGameHeader();
    /// 0x08994978
    CvSavedGameHeader(CvSavedGameHeader const& a1);
    /// 0x08994fd0
    ~CvSavedGameHeader();
    /// 0x089952dc
    UNK_RET operator=(CvSavedGameHeader const& a1);
    /// 0x089954be
    UNK_RET FromPreGame();
    /// 0x089964fc
    UNK_RET SetGameVersionString(char const* a1);
    /// 0x08996546
    UNK_RET SetGameVersionExtendedInfo(char const* a1);
    /// 0x08996590
    UNK_RET SetGameCoreGUID(_GUID const& a1);
    /// 0x089965ac
    UNK_RET SetGameCoreVersionString(char const* a1);
    /// 0x08996606
    UNK_RET SetTurnNumber(int a1);
    /// 0x08996612
    UNK_RET SetGameType(GameTypes a1);
    /// 0x0899661e
    UNK_RET SetMapScriptName(char const* a1);
    /// 0x08996668
    UNK_RET SetPlayerCivilization(char const* a1);
    /// 0x089966b2
    UNK_RET SetCustomCivilizationName(char const* a1);
    /// 0x089966fc
    UNK_RET SetCustomLeaderName(char const* a1);
    /// 0x08996746
    UNK_RET SetPlayerColor(char const* a1);
    /// 0x08996790
    UNK_RET SetDifficultType(char const* a1);
    /// 0x089967da
    UNK_RET SetStartEraType(char const* a1);
    /// 0x08996824
    UNK_RET SetCurrentEraType(char const* a1);
    /// 0x0899686e
    UNK_RET SetGameSpeedType(char const* a1);
    /// 0x089968b8
    UNK_RET SetWorldSizeType(char const* a1);
    /// 0x08996902
    UNK_RET SetActivatedMods(std::vector<CvSavedGameHeader::ModInfo, std::allocator<CvSavedGameHeader::ModInfo> > const& a1);
    /// 0x0899693c
    UNK_RET SetActivatedDlc(std::vector<CvSavedGameHeader::DlcInfo, std::allocator<CvSavedGameHeader::DlcInfo> > const& a1);
    /// 0x08996976
    UNK_RET GetLatestHeaderVersion();
    /// 0x0899697c
    UNK_RET GetHeaderVersion();
    /// 0x08996984
    UNK_RET Write(FDataStream& a1);
    /// 0x08996b0c
    UNK_RET Read(FDataStream& a1);
    /// 0x08996e22
    UNK_RET GetGameVersionString() const;
    /// 0x08996e2a
    UNK_RET GetGameVersionExtendedInfo() const;
    /// 0x08996e32
    UNK_RET GetGameCoreGUID() const;
    /// 0x08996e3a
    UNK_RET GetGameCoreVersionString() const;
    /// 0x08996e42
    UNK_RET GetPlayerCivilization() const;
    /// 0x08996e4a
    UNK_RET GetDifficulty() const;
    /// 0x08996e52
    UNK_RET GetStartEraType() const;
    /// 0x08996e5a
    UNK_RET GetCurrentEraType() const;
    /// 0x08996e62
    UNK_RET GetTurnNumber() const;
    /// 0x08996e6a
    UNK_RET GetGameSpeedType() const;
    /// 0x08996e72
    UNK_RET GetIsSinglePlayer() const;
    /// 0x08996e82
    UNK_RET GetGameType() const;
    /// 0x08996e8a
    UNK_RET GetWorldSizeType() const;
    /// 0x08996e92
    UNK_RET GetMapScriptName() const;
    /// 0x08996e9a
    UNK_RET GetActivatedDlc() const;
    /// 0x08996ea2
    UNK_RET GetActivatedMods() const;
    /// 0x08996eaa
    UNK_RET GetCustomCivilizationName() const;
    /// 0x08996eb2
    UNK_RET GetCustomLeaderName() const;
    /// 0x08996eba
    UNK_RET GetPlayerColor() const;
};

/// 0x08993cad
UNK_RET operator>>(FDataStream& a1, CvSavedGameHeader::DlcInfo& a2);
/// 0x08993ded
UNK_RET operator<<(FDataStream& a1, CvSavedGameHeader::DlcInfo const& a2);
/// 0x089940fe
UNK_RET operator>>(FDataStream& a1, CvSavedGameHeader::ModInfo& a2);
/// 0x08994290
UNK_RET operator<<(FDataStream& a1, CvSavedGameHeader::ModInfo const& a2);

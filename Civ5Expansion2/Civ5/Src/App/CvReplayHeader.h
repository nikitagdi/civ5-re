/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include <Windows.h>
#include <memory>
#include <vector>

class CvReplayHeader;

/// size ?
class CvReplayHeader {
public:
    class DlcInfo;
    class ModInfo;
    /// size ?
    class DlcInfo {
    public:
        /// 0x089ede14
        DlcInfo();
        /// 0x089ede48
        DlcInfo(_GUID a1, int a2, char const* a3);
        /// 0x089edea4
        DlcInfo(CvReplayHeader::DlcInfo const& a1);
        /// 0x089edf02
        UNK_RET operator=(CvReplayHeader::DlcInfo const& a1);
        /// 0x089edf64
        UNK_RET operator==(CvReplayHeader::DlcInfo const& a1) const;
        /// 0x089edfd0
        UNK_RET GetId() const;
        /// 0x089edfee
        UNK_RET GetDlcName() const;
        /// 0x089edff6
        UNK_RET GetVersion() const;
    };
    /// size ?
    class ModInfo {
    public:
        /// 0x089ee27a
        ModInfo();
        /// 0x089ee2ac
        ModInfo(char const* a1, int a2, char const* a3);
        /// 0x089ee314
        ModInfo(CvReplayHeader::ModInfo const& a1);
        /// 0x089ee380
        UNK_RET operator=(CvReplayHeader::ModInfo const& a1);
        /// 0x089ee3ee
        UNK_RET operator==(CvReplayHeader::ModInfo const& a1) const;
        /// 0x089ee438
        UNK_RET GetModId() const;
        /// 0x089ee43e
        UNK_RET GetVersion() const;
        /// 0x089ee44a
        UNK_RET GetModName() const;
    };
    /// 0x089ee792
    CvReplayHeader();
    /// 0x089eeb38
    CvReplayHeader(CvReplayHeader const& a1);
    /// 0x089ef104
    ~CvReplayHeader();
    /// 0x089ef3d8
    UNK_RET operator=(CvReplayHeader const& a1);
    /// 0x089ef56a
    UNK_RET FromPreGame();
    /// 0x089f04a0
    UNK_RET SetGameVersionString(char const* a1);
    /// 0x089f04ea
    UNK_RET SetGameVersionExtendedInfo(char const* a1);
    /// 0x089f0534
    UNK_RET SetTurnNumber(int a1);
    /// 0x089f0540
    UNK_RET SetIsSinglePlayer(bool a1);
    /// 0x089f054c
    UNK_RET SetMapScriptName(char const* a1);
    /// 0x089f0596
    UNK_RET SetPlayerCivilization(char const* a1);
    /// 0x089f05e0
    UNK_RET SetCustomCivilizationName(char const* a1);
    /// 0x089f062a
    UNK_RET SetCustomLeaderName(char const* a1);
    /// 0x089f0674
    UNK_RET SetPlayerColor(char const* a1);
    /// 0x089f06be
    UNK_RET SetDifficultType(char const* a1);
    /// 0x089f0708
    UNK_RET SetStartEraType(char const* a1);
    /// 0x089f0752
    UNK_RET SetCurrentEraType(char const* a1);
    /// 0x089f079c
    UNK_RET SetGameSpeedType(char const* a1);
    /// 0x089f07e6
    UNK_RET SetWorldSizeType(char const* a1);
    /// 0x089f0830
    UNK_RET SetActivatedMods(std::vector<CvReplayHeader::ModInfo, std::allocator<CvReplayHeader::ModInfo> > const& a1);
    /// 0x089f086a
    UNK_RET SetActivatedDlc(std::vector<CvReplayHeader::DlcInfo, std::allocator<CvReplayHeader::DlcInfo> > const& a1);
    /// 0x089f08a4
    UNK_RET GetLatestHeaderVersion();
    /// 0x089f08aa
    UNK_RET GetHeaderVersion();
    /// 0x089f08b2
    UNK_RET Write(FDataStream& a1);
    /// 0x089f09fe
    UNK_RET Read(FDataStream& a1);
    /// 0x089f0b5a
    UNK_RET GetGameVersionString() const;
    /// 0x089f0b62
    UNK_RET GetGameVersionExtendedInfo() const;
    /// 0x089f0b6a
    UNK_RET GetPlayerCivilization() const;
    /// 0x089f0b72
    UNK_RET GetDifficulty() const;
    /// 0x089f0b7a
    UNK_RET GetStartEraType() const;
    /// 0x089f0b82
    UNK_RET GetCurrentEraType() const;
    /// 0x089f0b8a
    UNK_RET GetTurnNumber() const;
    /// 0x089f0b92
    UNK_RET GetGameSpeedType() const;
    /// 0x089f0b9a
    UNK_RET GetIsSinglePlayer() const;
    /// 0x089f0ba4
    UNK_RET GetWorldSizeType() const;
    /// 0x089f0bac
    UNK_RET GetMapScriptName() const;
    /// 0x089f0bb4
    UNK_RET GetActivatedDlc() const;
    /// 0x089f0bbc
    UNK_RET GetActivatedMods() const;
    /// 0x089f0bc4
    UNK_RET GetCustomCivilizationName() const;
    /// 0x089f0bcc
    UNK_RET GetCustomLeaderName() const;
    /// 0x089f0bd4
    UNK_RET GetPlayerColor() const;
};

/// 0x089ee001
UNK_RET operator>>(FDataStream& a1, CvReplayHeader::DlcInfo& a2);
/// 0x089ee141
UNK_RET operator<<(FDataStream& a1, CvReplayHeader::DlcInfo const& a2);
/// 0x089ee452
UNK_RET operator>>(FDataStream& a1, CvReplayHeader::ModInfo& a2);
/// 0x089ee5e4
UNK_RET operator<<(FDataStream& a1, CvReplayHeader::ModInfo const& a2);

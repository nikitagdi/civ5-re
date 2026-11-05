/// origin guess
#pragma once

class RemoteStoragePublishedFileSubscribed_t;
class RemoteStoragePublishedFileUnsubscribed_t;
namespace Modding {
class System;
}

namespace Modding {
/// size ?
class SteamWorkshopMonitor {
public:
    /// 0x08c7781c
    SteamWorkshopMonitor(Modding::System& a1);
    /// 0x08c778dc
    UNK_RET OnPublishedFileSubscribed(RemoteStoragePublishedFileSubscribed_t* a1);
    /// 0x08c778e6
    UNK_RET OnPublishedFileUnsubscribed(RemoteStoragePublishedFileUnsubscribed_t* a1);
    /// 0x08c778f0
    UNK_RET Initalize(wchar_t const* a1);
    /// 0x08c7793a
    UNK_RET TriggerFullRefresh();
    /// 0x08c77944
    UNK_RET UpdateMod(char const* a1, int a2);
    /// 0x08c77c12
    UNK_RET CheckDownloadProgress(unsigned long long const& a1, int& a2, int& a3);
    /// 0x08c77fe8
    UNK_RET GetDownloadingFiles();
    /// 0x08c78124
    UNK_RET GetDownloadingFilesCount();
    /// 0x08c78152
    UNK_RET Update();
    /// 0x08c781bc
    UNK_RET CanUnsubscribe(char const* a1, int a2);
    /// 0x08c7827e
    UNK_RET Unsubscribe(char const* a1, int a2);
    /// 0x08c78366
    UNK_RET Download(unsigned long long a1, unsigned long long a2);
};
}

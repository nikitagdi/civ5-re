/// origin guess
#pragma once

class RemoteStorageDownloadUGCResult_t;

/// size ?
class SteamDownloadFileCommand {
public:
    /// 0x08c789fc
    SteamDownloadFileCommand(unsigned long long a1);
    /// 0x08c78afc
    UNK_RET OnResponse(RemoteStorageDownloadUGCResult_t* a1, bool a2);
};

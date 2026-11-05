/// origin guess
#pragma once

class RemoteStorageEnumerateUserSubscribedFilesResult_t;

/// size ?
class EnumerateUserSubscribedFilesCommand {
public:
    /// 0x08c78b60
    EnumerateUserSubscribedFilesCommand();
    /// 0x08c78c6c
    UNK_RET OnResponse(RemoteStorageEnumerateUserSubscribedFilesResult_t* a1, bool a2);
};

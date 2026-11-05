/// origin guess
#pragma once
#include <memory>
#include <vector>

class RemoteStorageDownloadUGCResult_t;
class RemoteStorageGetPublishedFileDetailsResult_t;
class SubscribedFile_t;

/// size ?
class GetPublishedFileDetailsCommand {
public:
    /// 0x08c78db2
    GetPublishedFileDetailsCommand(unsigned long long a1);
    /// 0x08c78eb6
    UNK_RET OnResponse(RemoteStorageGetPublishedFileDetailsResult_t* a1, bool a2);
};

/// 0x08c78f15
UNK_RET BlockingEnumerateUserSubscribedFiles(std::vector<SubscribedFile_t, std::allocator<SubscribedFile_t> >& a1);
/// 0x08c78fab
UNK_RET BlockingGetPublishedFileDetails(unsigned long long a1, RemoteStorageGetPublishedFileDetailsResult_t& a2);
/// 0x08c79046
UNK_RET BlockingDownloadFile(unsigned long long a1, RemoteStorageDownloadUGCResult_t& a2);

/// origin guess
#pragma once

class FResource;
class FResourceBackgroundLoaderCallback;
class FResourceBackgroundLoaderContext;
class FResourceLoadHelper;
enum FRTTI : int;

/// size ?
/// vtable 0x0a301d90
class FResourceBackgroundLoader {
public:
    /// 0x08cdbe56
    FResourceBackgroundLoader();
    /// 0x08cdbec2
    /// deleting 0x08cdbe8c
    virtual ~FResourceBackgroundLoader();
    /// 0x08cdc3c0
    virtual UNK_RET Update();
    /// 0x08cdbef2
    UNK_RET QueueResource(FResource* a1, unsigned int a2, FResourceBackgroundLoaderCallback* a3, FResourceLoadHelper* a4, bool a5);
    /// 0x08cdc03c
    UNK_RET GetQueuedContext(FResource* a1);
    /// 0x08cdc074
    UNK_RET QueueContext(FResourceBackgroundLoaderContext* a1);
    /// 0x08cdc0de
    UNK_RET QueueFile(char const* a1, FResourceBackgroundLoaderContext* a2, bool a3);
    /// 0x08cdc15e
    UNK_RET GetQueuedContext(char const* a1);
    /// 0x08cdc1ca
    UNK_RET GetQueuedContext(char const* a1, FRTTI const* a2, bool a3);
    /// 0x08cdc2a4
    UNK_RET RemoveQueuedContext(char const* a1);
    /// 0x08cdc316
    UNK_RET RemoveQueuedContext(FResource* a1);
    /// 0x08cdc35a
    UNK_RET RemoveAllQueuedContexts();
    /// 0x08cdc38e
    UNK_RET StartLoading(FResourceBackgroundLoaderContext* a1);
    /// 0x08cdc43a
    UNK_RET ContextLoaded(FResourceBackgroundLoaderContext* a1);
    /// 0x08cdc43c
    UNK_RET CleanupContexts();
    /// 0x08cdc59c
    UNK_RET SetDisabled(bool a1);
};

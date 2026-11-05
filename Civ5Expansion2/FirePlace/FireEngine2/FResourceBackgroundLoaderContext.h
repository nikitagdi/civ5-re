/// origin guess
#pragma once

class FResource;
class FResourceBackgroundLoaderCallback;

/// size ?
/// vtable 0x0a301d10
class FResourceBackgroundLoaderContext {
public:
    /// 0x08cdb948
    FResourceBackgroundLoaderContext(FResource* a1, unsigned int a2);
    /// 0x08cdb9a2
    FResourceBackgroundLoaderContext(unsigned int a1);
    /// 0x08cdba98
    /// deleting 0x08cdba70
    virtual ~FResourceBackgroundLoaderContext();
    /// 0x08cdb9ee
    virtual UNK_RET CreateLoader(char const* a1);
    /// 0x08cdbc16
    virtual UNK_RET FinishedLoading(bool a1);
    /// 0x08cdbafc
    UNK_RET AddCallback(FResourceBackgroundLoaderCallback* a1);
    /// 0x08cdbb6a
    UNK_RET RemoveCallback(FResourceBackgroundLoaderCallback* a1);
    /// 0x08cdbbf6
    UNK_RET RemoveAllCallbacks();
};

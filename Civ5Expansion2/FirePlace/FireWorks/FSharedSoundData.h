/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FSharedSoundDataInit;
enum ESoundLoadType : int;

/// size ?
class FSharedSoundData {
public:
    /// 0x08d2fb5c
    FSharedSoundData();
    /// 0x08d2fc58
    ~FSharedSoundData();
    /// 0x08d2fc08
    UNK_RET Clear();
    /// 0x08d2fd44
    UNK_RET Term();
    /// 0x08d2fd9c
    UNK_RET Init(FSharedSoundDataInit const& a1);
    /// 0x08d2fe14
    UNK_RET GetInitStructure(FSharedSoundDataInit& a1) const;
    /// 0x08d2fe78
    UNK_RET Unload();
    /// 0x08d2ff3c
    UNK_RET Destroy();
    /// 0x08d2ffa8
    UNK_RET AppendVariation(ESoundLoadType a1, int a2, bool a3, bool a4);
    /// 0x08d301ce
    UNK_RET DoLoad(FStringA const& a1, int a2, bool a3, bool a4);
    /// 0x08d3032c
    UNK_RET Load(int a1, ESoundLoadType a2, int a3, FStringA* a4, bool a5, bool a6, bool a7);
    /// 0x08d3077a
    UNK_RET GetScriptSpecificVariationOverride(int a1);
    /// 0x08d30924
    UNK_RET DoLoad_Sample(FStringA const& a1, int a2);
    /// 0x08d30988
    UNK_RET DoLoad_Stream(FStringA const& a1, int a2, bool a3);
    /// 0x08d30b12
    UNK_RET ComputeStreamFilenameToLoad(FStringA const& a1, int a2, FStringA& a3);
    /// 0x08d30ba2
    UNK_RET LoadStream(bool a1, FStringA const& a2, int a3);
    /// 0x08d30d0e
    UNK_RET IncNumInstancesInUse();
    /// 0x08d30d1e
    UNK_RET DecNumInstancesInUse();
    /// 0x08d30d3e
    UNK_RET GetTotalBufferSize() const;
    /// 0x08d30f46
    UNK_RET PopulateBackgroundLoadContext(int a1, FStringA const& a2, char const* a3);
    /// 0x08d31152
    UNK_RET QueueVariationForLoading(FStringA const& a1, char const* a2, int a3);
    /// 0x08d312d8
    UNK_RET QueueFileForLoading(FStringA const& a1);
    /// 0x08d31494
    UNK_RET OnBufferFinishedBackgroundLoading(unsigned char* a1, int a2);
    /// 0x08d3156c
    UNK_RET SetMarkedForDeletion(bool a1);
    /// 0x0ff85d50
    static UNK_DATA s_ObjPool[0x18];
};

namespace FAudioBuffer {
/// 0x08d30dd4
UNK_RET FillFromFile(char const* a1);
}

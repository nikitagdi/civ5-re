/// origin guess
#pragma once
#include "FirePlace/FireWorks/FGrannyAssetMiscDataLoader.h"

class FGrannyDecisionTreeMgr;

/// size ?
class FGrannyDecisionTreeMgr {
public:
    class MiscDataLoader;
    /// size ?
    /// vtable 0x0a3012e0
    class MiscDataLoader : public FGrannyAssetMiscDataLoader {
    public:
        /// 0x08cad110
        MiscDataLoader(FGrannyDecisionTreeMgr& a1);
        /// 0x08cad134
        virtual UNK_RET CanLoadExtension(char const* a1);
        /// 0x08cad27e
        virtual UNK_RET GetDataID();
        /// 0x08cad284
        virtual UNK_RET Load(char const* a1, unsigned int a2);
        /// 0x08cad3a6
        virtual UNK_RET LoadFromBuffer(char const* a1, int a2, char const* a3, char const* a4);
        /// 0x08cad3ea
        virtual UNK_RET DestroyData(void*& a1);
    };
    /// 0x08cad058
    FGrannyDecisionTreeMgr();
    /// 0x08cad064
    ~FGrannyDecisionTreeMgr();
    /// 0x08cad08e
    UNK_RET Shutdown();
    /// 0x08cad0ce
    UNK_RET Initialize();
};

/// origin guess
#pragma once
#include "FirePlace/FireWorks/FGrannyAssetMiscDataLoader.h"

class FTriggerTimelineSetMgr;
class FXmlDocument;

/// size ?
/// vtable 0x0a301318
class FTriggerTimelineSetMgr {
public:
    class MiscDataLoader;
    /// size ?
    /// vtable 0x0a301330
    class MiscDataLoader : public FGrannyAssetMiscDataLoader {
    public:
        /// 0x08cad542
        MiscDataLoader(FTriggerTimelineSetMgr& a1);
        /// 0x08cad566
        virtual UNK_RET CanLoadExtension(char const* a1);
        /// 0x08cad6b0
        virtual UNK_RET GetDataID();
        /// 0x08cad6b6
        virtual UNK_RET Load(char const* a1, unsigned int a2);
        /// 0x08cad7d8
        virtual UNK_RET LoadFromBuffer(char const* a1, int a2, char const* a3, char const* a4);
        /// 0x08cae18c
        virtual UNK_RET DestroyData(void*& a1);
        /// 0x08cad80a
        UNK_RET ParseTriggerFile(FXmlDocument const& a1, char const* a2);
    };
    /// 0x08cad424
    FTriggerTimelineSetMgr();
    /// 0x08cad45a
    ~FTriggerTimelineSetMgr();
    /// pure
    virtual UNK_RET vf_00(UNK_PARAMS) = 0;
    /// 0x08cad4aa
    UNK_RET Shutdown();
    /// 0x08cad4f8
    UNK_RET Initialize();
};

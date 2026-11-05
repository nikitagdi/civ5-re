/// origin guess
#pragma once
#include "FirePlace/FireWorks/FGrannyAssetMiscDataLoader.h"

class FITimedTriggerSystem;
class FTimedTriggerMgr;
class FXmlDocument;
class FXmlElement;

/// size ?
/// vtable 0x0a302690
class FTimedTriggerMgr {
public:
    class FTimedTriggerDataLoader;
    /// size ?
    /// vtable 0x0a3026b0
    class FTimedTriggerDataLoader : public FGrannyAssetMiscDataLoader {
    public:
        /// 0x08ce6290
        FTimedTriggerDataLoader(FTimedTriggerMgr& a1);
        /// 0x08ce62b4
        virtual UNK_RET CanLoadExtension(char const* a1);
        /// 0x08ce63fe
        virtual UNK_RET GetDataID();
        /// 0x08ce6404
        virtual UNK_RET Load(char const* a1, unsigned int a2);
        /// 0x08ce6526
        virtual UNK_RET LoadFromBuffer(char const* a1, int a2, char const* a3, char const* a4);
        /// 0x08ce6914
        virtual UNK_RET DestroyData(void*& a1);
        /// 0x08ce6560
        UNK_RET ParseTriggerFile(FXmlDocument const& a1, char const* a2);
        /// 0x08ce6c06
        UNK_RET ParseTriggers(FXmlElement const& a1, FITimedTriggerSystem& a2);
    };
    /// 0x08ce60f4
    FTimedTriggerMgr();
    /// 0x08ce6174
    /// deleting 0x08ce614c
    virtual ~FTimedTriggerMgr();
    /// pure
    virtual UNK_RET InitializeFactories() = 0;
    /// 0x08ce61da
    UNK_RET Shutdown();
    /// 0x08ce6234
    UNK_RET Initialize();
    /// 0x08ce6280
    UNK_RET GetTriggerFactory();
    /// 0x08ce6288
    UNK_RET GetSystemFactory();
    /// 0x08ce6942
    UNK_RET SaveTriggerSystem(FITimedTriggerSystem* a1, char const* a2);
};

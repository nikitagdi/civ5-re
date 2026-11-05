/// origin guess
#pragma once
#include "FirePlace/FireWorks/FGrannyAssetMiscDataLoader.h"

class FIFiniteStateMachine;
class FStateMachineMgr;
class FXmlDocument;
class FXmlElement;

/// size ?
/// vtable 0x0a301710
class FStateMachineMgr {
public:
    class FStateMachineDataLoader;
    /// size ?
    /// vtable 0x0a301720
    class FStateMachineDataLoader : public FGrannyAssetMiscDataLoader {
    public:
        /// 0x08cb8c18
        FStateMachineDataLoader(FStateMachineMgr& a1);
        /// 0x08cb8c3c
        virtual UNK_RET CanLoadExtension(char const* a1);
        /// 0x08cb8d86
        virtual UNK_RET GetDataID();
        /// 0x08cb8d8c
        virtual UNK_RET Load(char const* a1, unsigned int a2);
        /// 0x08cb8eae
        virtual UNK_RET LoadFromBuffer(char const* a1, int a2, char const* a3, char const* a4);
        /// 0x08cb9394
        virtual UNK_RET DestroyData(void*& a1);
        /// 0x08cb8ee8
        UNK_RET ParseStateMachineFile(FXmlDocument const& a1, char const* a2);
        /// 0x08cb93c2
        UNK_RET ParseStates(FXmlElement const& a1, FIFiniteStateMachine& a2);
        /// 0x08cb9666
        UNK_RET ParseSockets(FXmlElement const& a1, FIFiniteStateMachine& a2);
    };
    /// 0x08cb89c0
    FStateMachineMgr();
    /// 0x08cb89fe
    ~FStateMachineMgr();
    /// 0x08cb8b7c
    virtual UNK_RET InitializeFactories();
    /// 0x08cb8a4e
    UNK_RET Shutdown();
    /// 0x08cb8aa2
    UNK_RET Initialize(unsigned int a1);
    /// 0x08cb8b74
    UNK_RET GetFSMFactory();
    /// 0x08cb9e28
    UNK_RET SaveGraph(FIFiniteStateMachine* a1, char const* a2);
};

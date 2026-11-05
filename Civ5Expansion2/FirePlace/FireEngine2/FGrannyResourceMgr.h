/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"

class FResourceBackgroundLoaderCallback;
class FResourceLoadHelper;
class FXmlDocument;
class FXmlElement;
class PredicateFXmlElemWithAttr;
template <class T0, class T1> class FXMLAccumulator;

/// size ?
/// vtable 0x0a301b90
class FGrannyResourceMgr {
public:
    /// 0x08cd3f30
    virtual UNK_RET LoadAsset(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x08cd3fda
    virtual UNK_RET LoadAssetInstance(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x08cd4096
    virtual UNK_RET BackgroundLoadAsset(char const* a1, unsigned int a2, FResourceBackgroundLoaderCallback* a3, FResourceLoadHelper* a4);
    /// 0x08cd4120
    virtual UNK_RET BackgroundLoadAsset(char const* a1, FResourceLoadHelper* a2);
    /// 0x08cd41aa
    virtual UNK_RET CreateEmptyAsset();
    /// 0x08cd41ee
    virtual UNK_RET LoadFile(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x08cd4298
    virtual UNK_RET LoadFileResource(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x0d276c84
    static UNK_DATA m_pInst[0x4];
};

/// 0x08cd4673 ForEachFXmlNode<FXMLAccumulator<PredicateFXmlElemWithAttr, FStaticVector<FXmlElement, 512u, false, 27u, 0u> > >
template <class T0>
void ForEachFXmlNode(FXmlDocument const& a1, T0& a2);
extern "C" {
    /// 0x0d278f58
    extern UNK_DATA g_DefaultGameViewObjectPoseCache[0x1800];
    /// 0x0d27a758
    extern UNK_DATA g_LeaderHeadPoseCache[0x60];
    /// 0x0d276e38
    extern UNK_DATA g_TmpTransformList[0x2000];
}

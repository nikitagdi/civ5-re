/// origin guess
#pragma once
#include "FirePlace/FireGrafix/core/FImageCoder.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FString.h"

class CvLeaderHeadSystem;
class CvLeaderHeadTextureManager;
class FXmlDocument;
class FXmlTraverser;
namespace FireGrafix {
class TextureData;
}

/// size ?
class CvLeaderHeadScene {
public:
    class InstanceInfo;
    /// 0x0888f490
    CvLeaderHeadScene();
    /// 0x0888f660
    ~CvLeaderHeadScene();
    /// 0x0888f922
    UNK_RET EvictAssets();
    /// 0x0888fa0c
    UNK_RET CreateFromFile(CvLeaderHeadSystem* a1, FStringA const& a2, bool a3, bool a4);
    /// 0x0889006e
    UNK_RET OpenLeaderXMLFile(char const* a1, FXmlDocument& a2, FXmlTraverser& a3);
    /// 0x08890290
    UNK_RET GetCubemapPrefix(FXmlTraverser& a1, FStringA& a2);
    /// 0x0889031c
    UNK_RET SetSceneName(FStringA const& a1);
    /// 0x0889035c
    UNK_RET InitSceneSettings();
    /// 0x08890408
    UNK_RET ParseSceneAttributes(CvLeaderHeadSystem* a1, FXmlTraverser& a2);
    /// 0x08890bb2
    UNK_RET OpenAuxillaryXMLFile(FStringA const& a1, FXmlDocument& a2, FXmlTraverser& a3);
    /// 0x08890eee
    UNK_RET ParseObjectTag(CvLeaderHeadSystem* a1, FXmlTraverser& a2, FFastVector<CvLeaderHeadScene::InstanceInfo, true, 46u, 0u, BaseVector<CvLeaderHeadScene::InstanceInfo, true>::FDefaultFastVectorAllocator>& a3, char const* a4);
    /// 0x088912e2
    UNK_RET ParseDOFSettings(FXmlTraverser& a1);
    /// 0x08891424
    UNK_RET ParseBloomSettings(FXmlTraverser& a1);
    /// 0x08891566
    UNK_RET ParseLineLightSettings(FXmlTraverser& a1);
    /// 0x088915ae
    UNK_RET ParseHackSpecularSettings(FXmlTraverser& a1);
    /// 0x088915da
    UNK_RET CreateInstances(FFastVector<CvLeaderHeadScene::InstanceInfo, true, 46u, 0u, BaseVector<CvLeaderHeadScene::InstanceInfo, true>::FDefaultFastVectorAllocator>& a1);
    /// 0x0889177e
    UNK_RET CreateFromFiles(CvLeaderHeadSystem* a1, FStringA const& a2, FStringA const& a3, bool a4, bool a5);
    /// 0x0889208a
    UNK_RET CreateFallbackScene(CvLeaderHeadSystem* a1, char const* a2);
    /// 0x0889253a
    UNK_RET ParseObjectTagFallback(CvLeaderHeadSystem* a1, FXmlTraverser& a2, FFastVector<CvLeaderHeadScene::InstanceInfo, true, 46u, 0u, BaseVector<CvLeaderHeadScene::InstanceInfo, true>::FDefaultFastVectorAllocator>& a3);
    /// 0x088928f2
    UNK_RET Update(float a1);
    /// 0x088929b0
    UNK_RET SetFSMPathAction(float a1, bool a2);
    /// 0x08892a80
    UNK_RET OnPlay();
    /// 0x08892aca
    UNK_RET SetupAnimations();
    /// 0x08892b2e
    UNK_RET ReloadAssets(CvLeaderHeadTextureManager* a1);
    /// 0x08892c76
    UNK_RET AddCompressedTexture(unsigned int a1, FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>* a2, unsigned int a3, FireGrafix::TextureData* a4, unsigned int a5, FireGrafix::FImageCoder::CompressionHeader* a6);
    /// 0x08892ec2
    UNK_RET IsPoolTextureInUse(unsigned int a1);
};

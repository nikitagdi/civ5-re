/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FGrFGXMaterialInfo.h"
#include "FirePlace/FireWorks/FIFile.h"
#include "FirePlace/FireWorks/FString.h"

class CvLeaderHeadFallbackTextureInfo;
class CvLeaderHeadScene;
class DBCMaterialEntry;
class _IO_FILE;
namespace FireGrafix {
class FGXSurface;
}

/// size ?
class CvLeaderHeadTextureManager {
public:
    class TextureInfo;
    class ColorKeyInfo;
    /// size ?
    class TextureInfo {
    public:
        /// 0x0889ca30
        TextureInfo();
        /// 0x088992a8
        UNK_RET RemoveSceneReference(CvLeaderHeadScene* a1);
        /// 0x0889a0e8
        UNK_RET HasSceneReference();
        /// 0x0889c648
        UNK_RET AddSceneReference(CvLeaderHeadScene* a1);
        /// 0x0889ca6c
        UNK_RET HasSceneReference(CvLeaderHeadScene* a1);
    };
    /// 0x0889910c
    CvLeaderHeadTextureManager();
    /// 0x088991c6
    ~CvLeaderHeadTextureManager();
    /// 0x0889921e
    UNK_RET EnableSM41(bool a1);
    /// 0x0889922e
    UNK_RET EnableGPUDecompress(bool a1);
    /// 0x0889923a
    UNK_RET UnloadScene(CvLeaderHeadScene* a1);
    /// 0x088992de
    UNK_RET DestroyUnreferencedTextures();
    /// 0x088993f0
    UNK_RET BindTexturesToSurface(FireGrafix::FGXSurface* a1, DBCMaterialEntry const* a2, CvLeaderHeadScene* a3, char const* a4);
    /// 0x08899638
    UNK_RET BindEditableTexturesToSurface(FireGrafix::FGXSurface* a1, unsigned int a2, FGrFGXMaterialInfo::TextureBindingInfo const* a3, DBCMaterialEntry const* a4, CvLeaderHeadScene* a5);
    /// 0x08899908
    UNK_RET BindEnvironmentTexturesToSurface(FireGrafix::FGXSurface* a1, char const* a2, CvLeaderHeadScene* a3, char const* a4);
    /// 0x08899f16
    UNK_RET DestroyTextures();
    /// 0x0889a028
    UNK_RET DestroyColorKeyTextures();
    /// 0x0889a0f8
    UNK_RET ShutdownGame();
    /// 0x0889a154
    UNK_RET Shutdown();
    /// 0x0889a190
    UNK_RET ReloadChangedTextures();
    /// 0x0889a274
    UNK_RET IsCubemapArrayOutOfDate(char const* a1, FFileTimestamp const* a2) const;
    /// 0x0889a3da
    UNK_RET ReadCubemapArrayFiles(char const* a1, FFileTimestamp* a2) const;
    /// 0x0889a3de
    UNK_RET ReloadColorKeys();
    /// 0x0889a43c
    UNK_RET LoadColorKey(char const* a1, CvLeaderHeadTextureManager::ColorKeyInfo& a2);
    /// 0x0889a690
    UNK_RET GetColorKey(char const* a1);
    /// 0x0889a872
    UNK_RET GetIdentityColorKey();
    /// 0x0889a94e
    UNK_RET GetFallbackTexture(CvLeaderHeadScene* a1, char const* a2, CvLeaderHeadFallbackTextureInfo& a3);
    /// 0x0889ab7a
    UNK_RET FindPoolTexture(CvLeaderHeadScene* a1, unsigned int a2, unsigned int a3, unsigned int a4, bool a5);
    /// 0x0889ad54
    UNK_RET RestoreFallbackTexture(CvLeaderHeadFallbackTextureInfo const& a1);
    /// 0x0889af02
    UNK_RET DumpTextureStats(_IO_FILE* a1);
    /// 0x0889b630
    UNK_RET IsArtistAssignedTexture(char const* a1);
    /// 0x0889b67a
    UNK_RET BindTexture(FireGrafix::FGXSurface* a1, CvLeaderHeadScene* a2, char const* a3, char const* a4);
    /// 0x0889b736
    UNK_RET BindCubeArray(FireGrafix::FGXSurface* a1, char const* a2, char const* a3);
    /// 0x0889b738
    UNK_RET LoadTexture(CvLeaderHeadScene* a1, char const* a2);
    /// 0x0889c52a
    UNK_RET GetTextureBindingInfo(char const* a1, unsigned int& a2, unsigned int& a3, unsigned int& a4);
    /// 0x0889c644
    UNK_RET LoadCubeArray(char const* a1);
    /// 0x0889c674
    UNK_RET IsTextureTypeReducible(char const* a1);
    /// 0x0889c6c0
    UNK_RET GetCubemapArrayNames(char const* a1, FStringA* a2) const;
    /// 0x0889c7f8
    UNK_RET ReadTextureFile(char const* a1, FFileTimestamp& a2, unsigned int& a3) const;
};

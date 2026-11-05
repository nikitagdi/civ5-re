/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/CvLeaderHeadLightRig.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FString.h"
#include <d3d9.h>

class CVLHR_MeshBuilder;
class CvLeaderHeadLightRig;
class CvLeaderHeadModelInstance;
class CvLeaderHeadScene;
class CvLeaderHeadTextureInfo;
class FCamera;
class FGXPlane;
class FGXVector2;
class FGXVector3;
class FGXVector4;
class SkinBlurParams;
namespace FireGrafix {
class FGXModelNode;
class FGXShaderPackageInstanceView;
class LeaderLightsConsts;
enum FGX_SPIV_GENERIC : int;
}

/// size ?
class CvLeaderHeadRenderer {
public:
    class CvLeaderDecompressionJob;
    /// size ?
    /// vtable 0x0a2fd8b8
    class CvLeaderDecompressionJob : public FJob {
    public:
        /// 0x0888e402
        virtual void Execute(unsigned int a1);
        /// 0x0888434a
        UNK_RET FinishTexture();
    };
    enum DebugMode : int;
    class DrawType;
    class FurNodeInfo;
    enum ShadowMode : int;
    class ShapeMesh;
    enum SkinMode : int;
    class TransparencyDraw;
    /// 0x0887ff08
    CvLeaderHeadRenderer();
    /// 0x0888016a
    UNK_RET Initialize(bool a1, bool a2, CvLeaderHeadRenderer::ShadowMode a3, unsigned int a4, CvLeaderHeadRenderer::SkinMode a5, unsigned int a6, bool a7, bool a8, bool a9);
    /// 0x08880fa4
    UNK_RET CreatePostQuadVB();
    /// 0x0888110a
    UNK_RET CreateQuadIB();
    /// 0x088811a6
    UNK_RET CreateCommandStream(bool a1);
    /// 0x08881260
    UNK_RET CreateEffectSPIs();
    /// 0x08881298
    UNK_RET InitializeShaderMethodTable(unsigned char (*a1)[2][25], bool a2);
    /// 0x08881fe8
    UNK_RET SetSkinMode(CvLeaderHeadRenderer::SkinMode a1, unsigned int a2);
    /// 0x08882214
    UNK_RET ConfigureShadows(CvLeaderHeadRenderer::ShadowMode a1, unsigned int a2);
    /// 0x088822ea
    UNK_RET ConfigureRenderingPipeline(bool a1, bool a2, bool a3, bool a4, unsigned int a5);
    /// 0x08882540
    UNK_RET CanUseMSAA();
    /// 0x0888256c
    UNK_RET CreateSkinBlurQuadVBAndSurfaces(unsigned int a1);
    /// 0x08882866
    UNK_RET CreateShapeMeshes();
    /// 0x088828da
    UNK_RET Shutdown();
    /// 0x08882d6c
    UNK_RET ReleaseShapeMeshes();
    /// 0x08882da2
    UNK_RET FlushSorterState();
    /// 0x08882e98
    UNK_RET SetupSorting(CvLeaderHeadScene* a1);
    /// 0x08882ecc
    UNK_RET RenderScene(FCamera const& a1, CvLeaderHeadScene const* a2, unsigned int a3, bool a4);
    /// 0x0888442e
    UNK_RET BackgroundLoadNextTexture(CvLeaderHeadTextureInfo const* a1, CvLeaderHeadTextureInfo const* a2, unsigned int a3, unsigned int a4);
    /// 0x088845f2
    UNK_RET RenderFallbackImage(unsigned int a1, unsigned int a2);
    /// 0x088848a8
    UNK_RET ChangeDisplayResolution(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08884b7c
    UNK_RET PreProcessTransparency(CvLeaderHeadScene const* a1, FCamera const& a2);
    /// 0x08884efa
    UNK_RET UpdateAllGlobalConstants(CvLeaderHeadScene const* a1, FCamera const& a2);
    /// 0x08885556
    UNK_RET RenderFullPipelineMSAA(unsigned int a1, CvLeaderHeadScene const* a2, FCamera const& a3, bool a4, bool a5, bool a6, bool a7);
    /// 0x088857c6
    UNK_RET RenderFullPipelineSingle(unsigned int a1, CvLeaderHeadScene const* a2, FCamera const& a3, bool a4, bool a5, bool a6, bool a7);
    /// 0x08885970
    UNK_RET IssueDrawCalls(CvLeaderHeadScene const* a1, unsigned int a2, CvLeaderHeadRenderer::DrawType a3, unsigned int a4, bool a5, unsigned int a6);
    /// 0x08885b34
    UNK_RET VisualizeLights(CvLeaderHeadScene const* a1, FCamera const& a2, unsigned int a3, unsigned int a4, bool a5);
    /// 0x088863ee
    UNK_RET GammaCorrect(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x0888653c
    UNK_RET RenderPRStencilPass(unsigned int a1, unsigned int a2, CvLeaderHeadScene const* a3);
    /// 0x08886660
    UNK_RET DrawPlanarReflectionGeometry(CvLeaderHeadModelInstance const* a1, unsigned int a2, unsigned int a3);
    /// 0x08886784
    UNK_RET RenderPostDepth(CvLeaderHeadScene const* a1, unsigned int a2, unsigned int a3);
    /// 0x08886852
    UNK_RET GammaAndAddBloom(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x088869b6
    UNK_RET RenderCompressionBenchmark(CvLeaderHeadScene const* a1, unsigned int a2);
    /// 0x08886c28
    UNK_RET CreateBloomQuadVB();
    /// 0x08886d8e
    UNK_RET CreateRandomRotationTexture(unsigned int a1) const;
    /// 0x08886f8c
    UNK_RET SetDebuggingMode(CvLeaderHeadRenderer::DebugMode a1);
    /// 0x08886f9c
    UNK_RET GetRenderStateText(FStringA& a1, bool a2);
    /// 0x0888722c
    UNK_RET SetViewport(float a1, float a2, float a3, float a4);
    /// 0x088872c6
    UNK_RET CreateViewerCommandStream();
    /// 0x08887334
    UNK_RET OnResolutionOptionsChanged(bool a1);
    /// 0x088873cc
    UNK_RET SetSkinViewportScale(float a1);
    /// 0x08887486
    UNK_RET RenderLitScene(unsigned int a1, unsigned int a2, CvLeaderHeadScene const* a3, FCamera const& a4, bool a5);
    /// 0x08887fea
    UNK_RET RenderDistortion(CvLeaderHeadScene const* a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x088880b8
    UNK_RET RenderDOFBlurWidth(CvLeaderHeadScene const* a1, unsigned int a2, unsigned int a3);
    /// 0x08888186
    UNK_RET DoDOFBlurMSAA(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x088882ce
    UNK_RET DoBloomFilter(CvLeaderHeadScene const* a1, unsigned int a2);
    /// 0x08888a08
    UNK_RET DoDOFBlurSingle(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08888b64
    UNK_RET DoDOFBlurSingleAndGamma(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08888cc4
    UNK_RET RenderSubsurfaceBasePasses(CvLeaderHeadScene const* a1);
    /// 0x08888dbe
    UNK_RET RenderPlanarReflections(unsigned int a1, unsigned int a2, FCamera const& a3, CvLeaderHeadScene const* a4);
    /// 0x088896dc
    UNK_RET FinalizeSubsurface(unsigned int a1, unsigned int a2, CvLeaderHeadScene const* a3);
    /// 0x08889784
    UNK_RET GetViewQuad(FGXVector3* a1, D3DXMATRIX const& a2);
    /// 0x08889882
    UNK_RET RenderShadowedPointLight(unsigned int a1, unsigned int a2, D3DXMATRIX const& a3, FGXVector3 const* a4, CvLeaderHeadScene const* a5, CvLeaderHeadLightRig::LightList const& a6, unsigned int a7, unsigned int a8, unsigned int a9, float a10, float a11);
    /// 0x08889f36
    UNK_RET RenderShadowedDirectionalLight(unsigned int a1, unsigned int a2, D3DXMATRIX const& a3, FGXVector3 const* a4, CvLeaderHeadScene const* a5, CvLeaderHeadLightRig::LightList const& a6, unsigned int a7, unsigned int a8, unsigned int a9, float a10, float a11);
    /// 0x0888a54a
    UNK_RET RenderShadowedSpotLight(unsigned int a1, unsigned int a2, D3DXMATRIX const& a3, FGXVector3 const* a4, CvLeaderHeadScene const* a5, CvLeaderHeadLightRig::LightList const& a6, unsigned int a7, unsigned int a8, unsigned int a9, float a10, float a11);
    /// 0x0888abea
    UNK_RET PutLightsIntoConstantStore(FireGrafix::LeaderLightsConsts* a1, CvLeaderHeadLightRig const* a2, CvLeaderHeadRenderer::ShadowMode a3);
    /// 0x0888b2b8
    UNK_RET SetupPlanarReflectionCamera(FCamera const& a1, FGXPlane const& a2);
    /// 0x0888b502
    UNK_RET UpdateTransforms(FireGrafix::FGXShaderPackageInstanceView const* a1, FireGrafix::FGXModelNode const* a2, FGXVector4 const* a3);
    /// 0x0888b680
    UNK_RET UpdateFurConstants(FireGrafix::FGXShaderPackageInstanceView const* a1, FireGrafix::FGXModelNode const* a2, FGXVector4 const* a3, float a4);
    /// 0x0888b7b0
    UNK_RET IssueNodeDrawCalls(FireGrafix::FGXModelNode const* a1, FireGrafix::FGX_SPIV_GENERIC const* a2, FGXVector4 const* a3, CvLeaderHeadRenderer::DrawType a4, unsigned int const* a5, unsigned int a6, bool a7, unsigned int a8);
    /// 0x0888bc5c
    UNK_RET RenderCubeShadowMap(FGXVector4 const& a1, CvLeaderHeadScene const* a2, float a3, float a4);
    /// 0x0888c468
    UNK_RET RenderSubsurfaceShadowPointLight(CvLeaderHeadScene const* a1, CvLeaderHeadLightRig::LightList const& a2, unsigned int a3, FGXVector4 const& a4, float a5);
    /// 0x0888c6d2
    UNK_RET RenderLightStencil(unsigned int a1, unsigned int a2, CvLeaderHeadRenderer::ShapeMesh const& a3, D3DXMATRIX const& a4);
    /// 0x0888c936
    UNK_RET RenderLightStencilFast(unsigned int a1, unsigned int a2, CvLeaderHeadRenderer::ShapeMesh const& a3, D3DXMATRIX const& a4);
    /// 0x0888ca12
    UNK_RET ClearAlpha(unsigned int a1, unsigned int a2);
    /// 0x0888cb3a
    UNK_RET GetDirectionalLightMatrix(FGXVector4 const& a1, FGXVector4 const& a2, FGXVector4 const& a3, FGXVector4 const& a4, D3DXMATRIX& a5);
    /// 0x0888ce2c
    UNK_RET Render2DShadowMap(CvLeaderHeadScene const* a1, D3DXMATRIX const& a2);
    /// 0x0888d0a0
    UNK_RET RenderSubsurfaceShadowDirectLight(CvLeaderHeadScene const* a1, CvLeaderHeadLightRig::LightList const& a2, unsigned int a3, float a4);
    /// 0x0888d29c
    UNK_RET GetSpotLightMatrix(FGXVector4 const& a1, FGXVector4 const& a2, FGXVector4 const& a3, FGXVector4 const& a4, float a5, D3DXMATRIX& a6);
    /// 0x0888d62c
    UNK_RET RenderSubsurfaceShadowSpotLight(CvLeaderHeadScene const* a1, CvLeaderHeadLightRig::LightList const& a2, unsigned int a3, float a4);
    /// 0x0888d846
    UNK_RET RenderSkinBlurMap(CvLeaderHeadScene const* a1);
    /// 0x0888d962
    UNK_RET DoSkinGaussianBlurs(unsigned int a1, unsigned int a2, CvLeaderHeadScene const* a3, SkinBlurParams const* a4, unsigned int a5);
    /// 0x0888da20
    UNK_RET SkinBlurPass(unsigned int a1, unsigned int a2, CvLeaderHeadScene const* a3, float a4, float const* a5, unsigned int a6, bool a7);
    /// 0x0888de22
    UNK_RET CreateAnisoSamplePattern(unsigned int a1, FGXVector2* a2);
    /// 0x0888df82
    UNK_RET AppendFur(FFastVector<CvLeaderHeadRenderer::FurNodeInfo, true, 46u, 0u, BaseVector<CvLeaderHeadRenderer::FurNodeInfo, true>::FDefaultFastVectorAllocator> const& a1, FFastVector<CvLeaderHeadRenderer::TransparencyDraw, true, 46u, 0u, BaseVector<CvLeaderHeadRenderer::TransparencyDraw, true>::FDefaultFastVectorAllocator>& a2);
    /// 0x0888dff6
    UNK_RET RenderLightShape(CvLeaderHeadRenderer::ShapeMesh const& a1, D3DXMATRIX const& a2, FGXVector4 const& a3);
    /// 0x0888e080
    UNK_RET GetDecompressionViewport(unsigned int a1, unsigned int a2);
    /// 0x0888e158
    UNK_RET StopDecompression();
    /// 0x0888e1b8
    UNK_RET DecompressTextures(CvLeaderHeadScene* a1);
};

namespace FGXMeshUtils {
/// 0x0888e9ec CreateBoxMesh<CVLHR_MeshBuilder>
template <class T0>
void CreateBoxMesh(T0& a1, float a2, float a3, float a4);
/// 0x0888ec92 CreateSphereMesh<CVLHR_MeshBuilder>
template <class T0>
void CreateSphereMesh(T0& a1, float a2, unsigned int a3, unsigned int a4);
}

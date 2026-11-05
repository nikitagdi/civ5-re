/// origin guess
#pragma once
#include <d3d9.h>
#include <forkparticles.h>

class FGXVector2;
class FGXVector3;
class FGXVector4;
class e_frkParticleShape;

/// size ?
class FForkPRenderBuffer {
public:
    /// 0x08caf6a0
    FForkPRenderBuffer();
    /// 0x08caf7e2
    ~FForkPRenderBuffer();
    /// 0x08caf80c
    UNK_RET Release();
    /// 0x08caf88a
    UNK_RET Init(unsigned int a1);
    /// 0x08cafa42
    UNK_RET InitForkAlphaStates();
    /// 0x08cafd0e
    UNK_RET CreateVBAndIB();
    /// 0x08cafd96
    UNK_RET UnlockVBAndIB();
    /// 0x08cafe22
    UNK_RET ReleaseVBAndIB();
    /// 0x08cafe8a
    UNK_RET BeginBuffer();
    /// 0x08caff0c
    UNK_RET ResetBuffer();
    /// 0x08caff80
    UNK_RET LockVBAndIB();
    /// 0x08cb0014
    UNK_RET BeginBatch(unsigned int a1);
    /// 0x08cb0078
    UNK_RET EndBuffer();
    /// 0x08cb00ba
    UNK_RET EndBatch(unsigned int a1);
    /// 0x08cb010c
    UNK_RET CanSubmit(_t_frkpemitter* a1) const;
    /// 0x08cb014c
    UNK_RET BeginSubmit(_t_frkpemitter* a1, unsigned int a2);
    /// 0x08cb02de
    UNK_RET SetViewIndex(unsigned int a1);
    /// 0x08cb036e
    UNK_RET SetAlphaState(unsigned int a1);
    /// 0x08cb0462
    UNK_RET SetShapeMode(e_frkParticleShape a1);
    /// 0x08cb04fa
    UNK_RET SetFacing(_t_frkpemitter* a1, D3DXMATRIX const& a2);
    /// 0x08cb0be0
    UNK_RET GetViewMatrixAt(unsigned int a1) const;
    /// 0x08cb0bf0
    UNK_RET SetShaderDataIndex(unsigned int a1);
    /// 0x08cb0c86
    UNK_RET EndSubmit();
    /// 0x08cb0c88
    UNK_RET SubmitDiamond(FGXVector4 const* a1, float a2, float a3, FGXVector2 const* a4, float a5, FGXVector4 const* a6, FGXVector4 const* a7);
    /// 0x08cb0fc4
    UNK_RET SubmitRectangle(FGXVector4 const* a1, float a2, float a3, FGXVector2 const* a4, float a5, FGXVector4 const* a6, FGXVector4 const* a7);
    /// 0x08cb1472
    UNK_RET SubmitTriangle(FGXVector4 const* a1, float a2, float a3, FGXVector2 const* a4, float a5, FGXVector4 const* a6, FGXVector4 const* a7);
    /// 0x08cb18d0
    UNK_RET SubmitString(FGXVector4 const* a1, float a2, float a3, FGXVector2 const* a4, float a5, FGXVector4 const* a6, FGXVector4 const* a7);
    /// 0x08cb1c3a
    UNK_RET GetSubmitFuncForShape(e_frkParticleShape a1);
    /// 0x08cb1c8c
    UNK_RET AcceptingShape(e_frkParticleShape a1) const;
    /// 0x08cb1cf4
    UNK_RET ViewHasBatches(unsigned int a1) const;
    /// 0x08cb1d0c
    UNK_RET SetTexture(unsigned int a1);
    /// 0x08cb1da2
    UNK_RET SetQuadAxis(FGXVector3 const& a1, FGXVector3 const& a2, FGXVector3 const& a3);
    /// 0x08cb1df6
    UNK_RET GetCurrentViewMatrix() const;
    /// 0x08cb1e08
    UNK_RET SetViewMatrixAt(unsigned int a1, D3DXMATRIX const& a2);
};

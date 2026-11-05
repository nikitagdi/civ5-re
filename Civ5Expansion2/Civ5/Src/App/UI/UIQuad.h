/// origin guess
#pragma once

class FGXVector2;
class FGXVector3;
class UIRenderBuffer;
class UITextureInfo;
enum UIAABB : int;
namespace FireGrafix {
class FGXSurface;
}

/// size ?
/// vtable 0x0a2ff980
class UIQuad {
public:
    /// 0x089d807c
    UIQuad();
    /// 0x089d811e
    ~UIQuad();
    /// 0x089d82c0
    virtual UNK_RET Initialize();
    /// 0x089d84fe
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089d8840
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d8bca
    virtual UNK_RET PerspectiveRender(unsigned int a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x089d8f26
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089d921e
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d9548
    virtual UNK_RET PerspectiveRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x089d988a
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x089d9890
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x089d8182
    UNK_RET CreateSurface(FireGrafix::FGXSurface** a1);
    /// 0x089d82d4
    UNK_RET NormalizeTexture();
    /// 0x089d83a4
    UNK_RET SetTextureHandle(unsigned int a1);
    /// 0x089d8406
    UNK_RET ReleaseAndUnloadTexture();
    /// 0x089d8486
    UNK_RET SetTexture(UITextureInfo* a1, UITextureInfo* a2);
    /// 0x0c083024
    static UNK_DATA s_Cache[0x1c];
};

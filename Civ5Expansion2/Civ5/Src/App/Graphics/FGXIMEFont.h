/// origin guess
#pragma once

namespace FireGrafix {
class CCommandStream;
}

/// size ?
class FGXIMEFont {
public:
    /// 0x0873d068
    FGXIMEFont();
    /// 0x0873d0dc
    ~FGXIMEFont();
    /// 0x0873d106
    UNK_RET Release();
    /// 0x0873d262
    UNK_RET OnDeviceReset();
    /// 0x0873d276
    UNK_RET CreateFontA(wchar_t* a1, long a2, long a3);
    /// 0x0873d278
    UNK_RET UploadGlyphs();
    /// 0x0873d27a
    UNK_RET UploadGlyph(FireGrafix::CCommandStream* a1, wchar_t a2);
    /// 0x0873d27c
    UNK_RET GetGlyphIndex(wchar_t a1);
    /// 0x0873d280
    UNK_RET PrepareText(wchar_t* a1, unsigned int a2);
    /// 0x0873d282
    UNK_RET PrepareTextUtf8(char* a1);
    /// 0x0873d2f4
    UNK_RET HasGlyph(unsigned int a1);
    /// 0x0873d2fa
    UNK_RET GetGlyphFromIntValue(unsigned int a1);
    /// 0x0873d308
    UNK_RET GetGlyphIndexOnly(wchar_t a1);
    /// 0x0873d30c
    UNK_RET GetFontTexture(unsigned int a1);
};

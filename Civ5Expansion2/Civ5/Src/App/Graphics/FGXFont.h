/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FXmlElement;

/// size ?
class FGXFont {
public:
    /// 0x088bb776
    FGXFont();
    /// 0x088bb84a
    ~FGXFont();
    /// 0x088bb984
    UNK_RET Release();
    /// 0x088bb9e8
    UNK_RET LoadFont(FStringA const& a1, int a2);
    /// 0x088bbd84
    UNK_RET LoadFontFromXML(FStringA const& a1);
    /// 0x088bc57e
    UNK_RET AddTextureFromXMLElement(FXmlElement const& a1);
    /// 0x088bcbb4
    UNK_RET AddStyleFromXMLElement(FXmlElement const& a1);
    /// 0x088bd080
    UNK_RET AddGlyphFromXMLElement(FXmlElement const& a1, int a2, int a3, int a4);
    /// 0x088bd3cc
    UNK_RET TestGlyphFromCharValue(unsigned char a1) const;
    /// 0x088bd422
    UNK_RET GetGlyphFromCharValue(unsigned char a1) const;
    /// 0x088bd488
    UNK_RET GetGlyphFromIntValue(unsigned int a1) const;
    /// 0x088bd4ec
    UNK_RET HasGlyph(unsigned char a1);
    /// 0x088bd542
    UNK_RET HasGlyph(unsigned int a1);
    /// 0x088bd596
    UNK_RET GetKerningData() const;
    /// 0x088bd59c
    UNK_RET GetFontSize() const;
    /// 0x088bd5a8
    UNK_RET GetLeading() const;
    /// 0x088bd602
    UNK_RET GetAscent() const;
    /// 0x088bd60e
    UNK_RET GetTextWidth(char const* a1, unsigned int a2) const;
    /// 0x088bd686
    UNK_RET GetStyleIndex(FStringA const& a1) const;
    /// 0x088bd6e4
    UNK_RET GetStyle(unsigned int a1) const;
    /// 0x088bd6f4
    UNK_RET GetFontShader();
    /// 0x088bd700
    UNK_RET GetIndexTexture(unsigned int a1) const;
    /// 0x088bd720
    UNK_RET GetBlockTexture(unsigned int a1) const;
    /// 0x088bd740
    UNK_RET GetAllowColor(unsigned int a1) const;
    /// 0x088bd76a
    UNK_RET GetBlockSize(unsigned int a1) const;
    /// 0x088bd798
    UNK_RET GetIndexScale(unsigned int a1) const;
    /// 0x088bd804
    UNK_RET GetIndexTextureSize(unsigned int a1) const;
    /// 0x088bd8bc
    UNK_RET GetBlockTextureSize(unsigned int a1) const;
};

/// origin guess
#pragma once
#include "Civ5/Src/App/IStrategicViewFont.h"
#include "FirePlace/FireWorks/FString.h"

class FXmlElement;

/// size ?
/// vtable 0x0a301280
class StrategicViewFont : public IStrategicViewFont {
public:
    /// 0x08c978ae
    StrategicViewFont();
    /// 0x08c9795a
    /// deleting 0x08c97932
    virtual ~StrategicViewFont();
    /// 0x08c9915a
    virtual UNK_RET GetGlyphFromIntValue(unsigned int a1) const;
    /// 0x08c991b2
    virtual UNK_RET HasGlyph(unsigned char a1);
    /// 0x08c99202
    virtual UNK_RET HasGlyph(unsigned int a1);
    /// 0x08c99250
    virtual UNK_RET GetFontSize() const;
    /// 0x08c992be
    virtual UNK_RET GetAscent() const;
    /// 0x08c99258
    virtual UNK_RET GetLeading() const;
    /// 0x08c992c6
    virtual UNK_RET GetTextWidth(char const* a1, unsigned int a2) const;
    /// 0x08c9933e
    virtual UNK_RET GetStyleIndex(FStringA const& a1) const;
    /// 0x08c99396
    virtual UNK_RET GetStyle(unsigned int a1) const;
    /// 0x08c993a4
    virtual UNK_RET GetIndexTexture(unsigned int a1) const;
    /// 0x08c993be
    virtual UNK_RET GetBlockTexture(unsigned int a1) const;
    /// 0x08c993d8
    virtual UNK_RET GetAllowColor(unsigned int a1) const;
    /// 0x08c993fc
    virtual UNK_RET GetBlockSize(unsigned int a1) const;
    /// 0x08c99424
    virtual UNK_RET GetIndexScale(unsigned int a1) const;
    /// 0x08c9948a
    virtual UNK_RET GetIndexTextureSize(unsigned int a1) const;
    /// 0x08c99538
    virtual UNK_RET GetBlockTextureSize(unsigned int a1) const;
    /// 0x08c979fa
    UNK_RET Release();
    /// 0x08c97a6a
    UNK_RET LoadFontFromXML(FStringA const& a1);
    /// 0x08c98252
    UNK_RET AddTextureFromXMLElement(FXmlElement const& a1);
    /// 0x08c98910
    UNK_RET AddStyleFromXMLElement(FXmlElement const& a1);
    /// 0x08c98dc0
    UNK_RET AddGlyphFromXMLElement(FXmlElement const& a1, int a2, int a3, int a4);
    /// 0x08c99100
    UNK_RET GetGlyphFromCharValue(unsigned char a1) const;
};

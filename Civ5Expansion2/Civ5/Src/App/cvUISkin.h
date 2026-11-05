/// origin guess
#pragma once
#include "Civ5/Src/App/cvContentPackage.h"
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>

class ColorSet;
class FGXVector4;
class FXmlElement;
class cvContentPackage;
class cvContentTypeAttribute;
class cvContentTypeAttributeList;
class cvUISkin;
class cvUISkinIndexList;

/// size ?
class cvUISkin {
public:
    enum SECTION_TYPE : int;
    /// 0x087619da
    cvUISkin(cvContentPackage* a1, unsigned int a2);
    /// 0x08761e92
    UNK_RET IsActive(cvUISkin::SECTION_TYPE a1) const;
    /// 0x08761ec0
    UNK_RET HasSection(cvUISkin::SECTION_TYPE a1) const;
    /// 0x08761eee
    UNK_RET Activate(cvUISkin::SECTION_TYPE a1);
    /// 0x08762858
    UNK_RET GetAttributeStr(char const* a1) const;
    /// 0x08762890
    UNK_RET Activate();
    /// 0x087628bc
    UNK_RET Deactivate(cvUISkin::SECTION_TYPE a1);
    /// 0x08762906
    UNK_RET Deactivate();
    /// 0x08762970
    UNK_RET IsActive() const;
    /// 0x0876298c
    UNK_RET SetActive(cvUISkin::SECTION_TYPE a1, bool a2);
    /// 0x08762a28
    UNK_RET GetContentID(_GUID* a1) const;
    /// 0x08762a58
    UNK_RET SetAttributes(cvContentTypeAttributeList const& a1);
    /// 0x08762aa8
    UNK_RET GetAttributeInt(char const* a1) const;
    /// 0x08762ae8
    UNK_RET IsAttributeEqual(char const* a1, char const* a2) const;
    /// 0x08762b3a
    UNK_RET IsAttributeEqual(char const* a1, int a2) const;
    /// 0x08762b88
    UNK_RET IsAttributeEqual(cvContentTypeAttribute const& a1) const;
    /// 0x08762bde
    UNK_RET IsAttributeEqual(cvContentTypeAttributeList const& a1) const;
    /// 0x08762c2e
    UNK_RET GetStyleElement(FXmlElement& a1, FStringA const& a2, cvUISkin::SECTION_TYPE a3);
    /// 0x08762cb8
    UNK_RET GetStyleElement(FXmlElement& a1, char const* a2, cvUISkin::SECTION_TYPE a3);
    /// 0x08762d0e
    UNK_RET GetStyleElement(FXmlElement& a1, char const* a2, unsigned int a3, cvUISkin::SECTION_TYPE a4);
    /// 0x08762da6
    UNK_RET GetColorSetByName(char const* a1, ColorSet& a2, cvUISkin::SECTION_TYPE a3);
    /// 0x08762e16
    UNK_RET GetColorSetByHash(unsigned int a1, ColorSet& a2, cvUISkin::SECTION_TYPE a3);
    /// 0x08762ebc
    UNK_RET GetColorByName(char const* a1, FGXVector4& a2, cvUISkin::SECTION_TYPE a3);
    /// 0x08762f2c
    UNK_RET GetColorByHash(unsigned int a1, FGXVector4& a2, cvUISkin::SECTION_TYPE a3);
};

namespace cvUISkinManager {
/// 0x08762fac
UNK_RET SetPlatform(char const* a1);
/// 0x0876301c
UNK_RET AddSkin(cvContentPackage* a1);
/// 0x0876317c
UNK_RET AddSkin(cvUISkin* a1);
/// 0x087631d2
UNK_RET ActivateSkin(_GUID const& a1, cvContentTypeAttributeList const& a2, cvUISkin::SECTION_TYPE a3);
/// 0x087632c6
UNK_RET RebuildColors();
/// 0x08763332
UNK_RET ActivateSkin(cvContentTypeAttributeList const& a1, cvUISkin::SECTION_TYPE a2);
/// 0x087633d4
UNK_RET FindSkin(cvContentTypeAttributeList const& a1, int a2);
/// 0x0876346e
UNK_RET FindSkins(cvContentTypeAttributeList const& a1, cvUISkinIndexList& a2, bool a3);
/// 0x08763542
UNK_RET ActivateSkinIndex(int a1);
/// 0x087635a4
UNK_RET GetHighestPrioritySkinSet(cvUISkin::SECTION_TYPE a1, int a2);
/// 0x087636f8
UNK_RET ActivateSkins(cvUISkin::SECTION_TYPE a1);
/// 0x087639d2
UNK_RET DeactivateSkins(cvUISkin::SECTION_TYPE a1, cvUISkinIndexList const* a2, bool a3);
/// 0x08763ac0
UNK_RET DeactivateSkins();
/// 0x08763b1a
UNK_RET DeactivateSkins(cvContentTypeAttributeList const& a1);
/// 0x08763bb4
UNK_RET DeactivateSkin(cvContentPackage* a1);
/// 0x08763c26
UNK_RET RemoveSkin(cvContentPackage* a1);
/// 0x08763cee
UNK_RET AddSkins();
/// 0x08763d5a
UNK_RET ClearSkins();
/// 0x08763e56
UNK_RET GetSkinCount();
/// 0x08763e72
UNK_RET GetActiveSkin(int a1);
/// 0x08763eba
UNK_RET GetActiveSkinCount();
/// 0x08763f00
UNK_RET GetStyleElement(FXmlElement& a1, FStringA const& a2, cvUISkin::SECTION_TYPE a3);
/// 0x08763f6a
UNK_RET GetStyleElement(FXmlElement& a1, char const* a2, cvUISkin::SECTION_TYPE a3);
/// 0x08763fd4
UNK_RET GetStyleElement(FXmlElement& a1, char const* a2, unsigned int a3, cvUISkin::SECTION_TYPE a4);
/// 0x08764046
UNK_RET GetColorSetByName(char const* a1, ColorSet& a2, cvUISkin::SECTION_TYPE a3);
/// 0x087640ac
UNK_RET GetColorSetByHash(unsigned int a1, ColorSet& a2, cvUISkin::SECTION_TYPE a3);
/// 0x08764178
UNK_RET GetColorByName(char const* a1, FGXVector4& a2, cvUISkin::SECTION_TYPE a3);
/// 0x087641de
UNK_RET OnContentChange(cvContentPackage* a1, cvContentPackage::CONTENT_TYPES a2, bool a3);
}

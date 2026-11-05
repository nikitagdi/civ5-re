/// origin guess
#pragma once
#include "Civ5/Src/App/cvUISkin.h"
#include "FirePlace/FireWorks/FString.h"

class ColorSet;
class FGXVector4;

/// size ?
class cvUISkinColors {
public:
    /// 0x087607c0
    ~cvUISkinColors();
    /// 0x08760838
    UNK_RET Clear();
    /// 0x0876093a
    UNK_RET Load(char const* a1);
    /// 0x08760f9e
    UNK_RET ColorFromXML(FStringA const& a1, FGXVector4& a2);
    /// 0x0876169c
    UNK_RET RebuildColors();
    /// 0x087617ca
    UNK_RET GetColorSetByName(char const* a1, ColorSet& a2);
    /// 0x08761874
    UNK_RET GetColorSetByHash(unsigned int a1, ColorSet& a2);
    /// 0x087618f2
    UNK_RET GetColorByName(char const* a1, FGXVector4& a2);
    /// 0x08761958
    UNK_RET GetColorByHash(unsigned int a1, FGXVector4& a2);
};

namespace cvUISkinManager {
/// 0x0876135a
UNK_RET ExtractColor(FStringA const& a1, FGXVector4& a2, unsigned int* a3, unsigned char* a4);
/// 0x0876174a
UNK_RET GetColorByHash(unsigned int a1, FGXVector4& a2, cvUISkin::SECTION_TYPE a3);
}

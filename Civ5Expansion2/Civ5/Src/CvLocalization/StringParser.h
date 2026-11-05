/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"
#include <Windows.h>
#include <memory>
#include <vector>

namespace Localization {
/// size ?
class StringParser {
public:
    /// 0x08c60f36
    StringParser(char const* a1, unsigned int a2);
    /// 0x08c60f60
    UNK_RET Parse(std::vector<Localization::StringParameter, std::allocator<Localization::StringParameter> >& a1);
    /// 0x08c6106e
    UNK_RET ParseParameter(Localization::StringParameter& a1, char const* a2, unsigned int a3, unsigned int& a4);
    /// 0x08c614e0
    UNK_RET IncrementOffset(unsigned int& a1, unsigned int a2, unsigned int a3);
    /// 0x08c61576
    UNK_RET IncrementOffsetAndSkipWhitespace(char const* a1, unsigned int a2, unsigned int a3, unsigned int& a4);
    /// 0x08c6166a
    UNK_RET ParseNumericIdentifier(char const* a1, unsigned int a2, int& a3, unsigned int& a4);
    /// 0x08c61742
    UNK_RET ParseIdentifier(char const* a1, unsigned int a2, unsigned int& a3, unsigned int& a4, unsigned int& a5);
    /// 0x08c6179e
    UNK_RET SkipWhitespace(char const* a1, unsigned int a2, unsigned int& a3);
    /// 0x08c6186a
    UNK_RET ParseFormSpecifier(Localization::StringParameter& a1, char const* a2, unsigned int a3, unsigned int& a4);
    /// 0x08c61ca0
    UNK_RET ParsePluralOrGenderForm(Localization::StringParameter& a1, char const* a2, unsigned int a3, unsigned int& a4);
    /// 0x08c61e12
    UNK_RET ParseInjectArgs(Localization::StringParameter& a1, char const* a2, unsigned int a3, unsigned int& a4);
    /// 0x08c621f0
    UNK_RET ParseTransform(Localization::StringParameter& a1, char const* a2, unsigned int a3, unsigned int& a4);
    /// 0x08c62360
    UNK_RET ParseLiteral(char const* a1, unsigned int a2, Localization::ParameterArgument& a3, unsigned int& a4);
    /// 0x08c62480
    UNK_RET ParseNumeric(char const* a1, unsigned int a2, Localization::ParameterArgument& a3, unsigned int& a4);
    /// 0x08c625fa
    UNK_RET ParseGenderForm(Localization::StringParameter& a1, char const* a2, unsigned int a3, unsigned int& a4);
    /// 0x08c6293a
    UNK_RET ParsePluralForm(Localization::StringParameter& a1, char const* a2, unsigned int a3, unsigned int& a4);
    /// 0x08c62c7a
    UNK_RET ParseGenderAndTraits(Localization::SharedString const& a1, Localization::Gender& a2, std::vector<Localization::SharedString, std::allocator<Localization::SharedString> >& a3);
};
}

namespace Localization {
/// 0x08c63ad6
UNK_RET ConvertUTCDatetoLocalTZDate(_SYSTEMTIME& a1, wchar_t const* a2);
/// 0x08c63cad
UNK_RET ConvertUTCTimetoLocalTZTime(_SYSTEMTIME& a1, wchar_t const* a2);
/// 0x08c63e84
UNK_RET ConvertUTCDateTimetoLocalTZDateTime(_SYSTEMTIME& a1, wchar_t const* a2, wchar_t const* a3);
}

namespace Localization { namespace TextEncoding {
/// 0x08c64360
UNK_RET ConvertUTF8toUTF16(char const* a1);
/// 0x08c64480
UNK_RET ConvertUTF16toUTF8(wchar_t const* a1);
}}

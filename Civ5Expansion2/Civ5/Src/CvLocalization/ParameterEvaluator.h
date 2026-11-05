/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"
#include <memory>
#include <string>
#include <vector>

namespace Localization {
/// size ?
class ParameterEvaluator {
public:
    /// 0x08c67018
    ParameterEvaluator(Localization::StringParameter const& a1, Localization::SingleFormString const& a2, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a3);
    /// 0x08c678b2
    UNK_RET Evaluate(Localization::SingleFormString& a1);
    /// 0x08c67d2a
    UNK_RET GetInjectedArguments(std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> >& a1);
    /// 0x08c67ffc
    UNK_RET EvaluateString(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > a2, Localization::SingleFormString& a3);
    /// 0x08c682aa
    UNK_RET EvaluateNumber(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > a2, Localization::SingleFormString& a3);
    /// 0x08c682e8
    UNK_RET GetExplicitFormSpecifier(unsigned int& a1);
    /// 0x08c68382
    UNK_RET GetPluralFormSpecifier(unsigned int& a1, Localization::Language const* a2);
    /// 0x08c68526
    UNK_RET GetGenderFormSpecifier(Localization::Gender& a1, std::vector<Localization::SharedString, std::allocator<Localization::SharedString> >& a2);
    /// 0x08c686d8
    UNK_RET Transform(Localization::SingleFormString& a1, Localization::ParameterArgument const& a2, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a3);
    /// 0x08c68afa
    UNK_RET DefaultTransformation(Localization::SingleFormString& a1, Localization::ParameterArgument const& a2, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a3);
    /// 0x0d266250
    static UNK_DATA ms_styletransforms[0x14];
};
}

namespace Localization {
/// 0x08c674cd
UNK_RET FormatCurrency(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c67520
UNK_RET FormatNumber(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c67573
UNK_RET FormatPercent(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c675c6
UNK_RET FormatRoman(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c67619
UNK_RET FormatSpellout(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c6766c
UNK_RET FormatOrdinal(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c676bf
UNK_RET FormatUppercase(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c67712
UNK_RET FormatLowercase(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c67765
UNK_RET FormatTitlecase(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c677b8
UNK_RET FormatTextkey(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c6780b
UNK_RET ChoiceFormatGender(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
/// 0x08c6785e
UNK_RET ChoiceFormatPlurality(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, std::string const& a3, std::string& a4);
}

namespace Modding {
/// 0x0d266924
extern UNK_DATA g_allocCount[0x4];
/// 0x0d266930
extern UNK_DATA g_allocCountTemp[0x4];
/// 0x0d266928
extern UNK_DATA g_curAlloc[0x4];
/// 0x0d266934
extern UNK_DATA g_curAllocTemp[0x4];
/// 0x0d26692c
extern UNK_DATA g_maxAlloc[0x4];
/// 0x0d266938
extern UNK_DATA g_maxAllocTemp[0x4];
}

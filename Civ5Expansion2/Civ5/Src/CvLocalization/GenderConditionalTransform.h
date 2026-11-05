/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/ConditionalTransform.h"
#include "Civ5/Src/CvLocalization/CvLocalization.h"
#include <icu.h>
#include <memory>
#include <vector>

namespace Localization {
/// size ?
/// vtable 0x0a300db0
class GenderConditionalTransform : public Localization::ConditionalTransform {
public:
    /// 0x08c64a98
    GenderConditionalTransform(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, char const* a3, unsigned int a4);
    /// absent
    virtual ~GenderConditionalTransform();
    /// 0x08c64ae8
    virtual UNK_RET IsMatch(char const* a1, unsigned int a2);
};
}

namespace Localization {
/// 0x08c65430
UNK_RET GetPluralityRule(unsigned int a1);
}

namespace Localization { namespace ICU {
/// 0x08c64b60
UNK_RET LogStatus(UErrorCode a1);
}}

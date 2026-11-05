/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/ConditionalTransform.h"
#include "Civ5/Src/CvLocalization/CvLocalization.h"
#include <memory>
#include <vector>

namespace Localization {
/// size ?
/// vtable 0x0a300c90
class PluralityConditionalTransform : public Localization::ConditionalTransform {
public:
    /// 0x08c5cb64
    PluralityConditionalTransform(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, char const* a3, unsigned int a4);
    /// absent
    virtual ~PluralityConditionalTransform();
    /// 0x08c5cc9e
    virtual UNK_RET IsMatch(char const* a1, unsigned int a2);
};
}

/// 0x08c60e84
UNK_RET GetStringType(char const* a1, unsigned int a2);

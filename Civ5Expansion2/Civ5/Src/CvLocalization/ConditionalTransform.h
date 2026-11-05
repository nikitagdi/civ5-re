/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"
#include <memory>
#include <string>
#include <vector>

namespace Localization {
/// size ?
/// vtable 0x0a300d90
class ConditionalTransform {
public:
    /// 0x08c647d4
    ConditionalTransform(Localization::ParameterArgument const& a1, std::vector<Localization::ParameterArgument, std::allocator<Localization::ParameterArgument> > const& a2, char const* a3, unsigned int a4);
    /// 0x08c64826
    /// deleting 0x08c64812
    virtual ~ConditionalTransform();
    /// pure
    virtual UNK_RET IsMatch(char const* a1, unsigned int a2) = 0;
    /// 0x08c64828
    UNK_RET Transform(std::string& a1);
};
}

/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300fac
class PluralRule14 : public Localization::IPluralityRule {
public:
    /// 0x08c6588c
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c65892
    virtual unsigned int GetPluralForm(int a1) const;
};
}

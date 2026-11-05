/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300e2c
class PluralRule2 : public Localization::IPluralityRule {
public:
    /// 0x08c655ca
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c655d0
    virtual unsigned int GetPluralForm(int a1) const;
};
}

/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300e4c
class PluralRule3 : public Localization::IPluralityRule {
public:
    /// 0x08c655de
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c655e4
    virtual unsigned int GetPluralForm(int a1) const;
};
}

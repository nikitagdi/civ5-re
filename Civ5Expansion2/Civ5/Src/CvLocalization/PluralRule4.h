/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300e6c
class PluralRule4 : public Localization::IPluralityRule {
public:
    /// 0x08c655fc
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c65602
    virtual unsigned int GetPluralForm(int a1) const;
};
}

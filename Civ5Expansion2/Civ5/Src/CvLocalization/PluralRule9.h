/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300f0c
class PluralRule9 : public Localization::IPluralityRule {
public:
    /// 0x08c65736
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c6573c
    virtual unsigned int GetPluralForm(int a1) const;
};
}

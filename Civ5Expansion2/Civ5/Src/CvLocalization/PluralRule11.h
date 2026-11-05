/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300f4c
class PluralRule11 : public Localization::IPluralityRule {
public:
    /// 0x08c657a4
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c657aa
    virtual unsigned int GetPluralForm(int a1) const;
};
}

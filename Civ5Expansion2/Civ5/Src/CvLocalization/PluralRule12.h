/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300f6c
class PluralRule12 : public Localization::IPluralityRule {
public:
    /// 0x08c657ec
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c657f2
    virtual unsigned int GetPluralForm(int a1) const;
};
}

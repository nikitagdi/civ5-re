/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300f2c
class PluralRule10 : public Localization::IPluralityRule {
public:
    /// 0x08c6575a
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c65760
    virtual unsigned int GetPluralForm(int a1) const;
};
}

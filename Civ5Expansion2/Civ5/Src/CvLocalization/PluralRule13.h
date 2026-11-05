/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300f8c
class PluralRule13 : public Localization::IPluralityRule {
public:
    /// 0x08c65828
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c6582e
    virtual unsigned int GetPluralForm(int a1) const;
};
}

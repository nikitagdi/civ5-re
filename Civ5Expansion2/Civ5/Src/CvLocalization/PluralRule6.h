/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300eac
class PluralRule6 : public Localization::IPluralityRule {
public:
    /// 0x08c65668
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c6566e
    virtual unsigned int GetPluralForm(int a1) const;
};
}

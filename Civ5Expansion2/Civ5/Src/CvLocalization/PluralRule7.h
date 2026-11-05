/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300ecc
class PluralRule7 : public Localization::IPluralityRule {
public:
    /// 0x08c656a8
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c656ae
    virtual unsigned int GetPluralForm(int a1) const;
};
}

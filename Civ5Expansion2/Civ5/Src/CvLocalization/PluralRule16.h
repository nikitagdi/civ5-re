/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300fec
class PluralRule16 : public Localization::IPluralityRule {
public:
    /// 0x08c65916
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c6591c
    virtual unsigned int GetPluralForm(int a1) const;
};
}

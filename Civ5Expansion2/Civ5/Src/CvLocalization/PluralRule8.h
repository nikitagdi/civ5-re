/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300eec
class PluralRule8 : public Localization::IPluralityRule {
public:
    /// 0x08c656e4
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c656ea
    virtual unsigned int GetPluralForm(int a1) const;
};
}

/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300e10
class PluralRule1 : public Localization::IPluralityRule {
public:
    /// 0x08c655be
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c655c4
    virtual unsigned int GetPluralForm(int a1) const;
};
}

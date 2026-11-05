/// origin guess
#pragma once
#include "Civ5/Src/CvLocalization/CvLocalization.h"

namespace Localization {
/// size ?
/// vtable 0x0a300fcc
class PluralRule15 : public Localization::IPluralityRule {
public:
    /// 0x08c658d8
    virtual unsigned int GetMaxPluralForms() const;
    /// 0x08c658de
    virtual unsigned int GetPluralForm(int a1) const;
};
}

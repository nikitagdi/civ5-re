/// origin guess
#pragma once

template <class T>
class cvUnitVisSystemAccessor;

/// size 0x4
struct cvGeneralRefHandleBase {
    unsigned int field_0;  ///< +0x0
};

/// size 0x8
template <class T, class TId, class TDataDef, template <class> class TAccessor>
class cvGeneralRefHandle : public cvGeneralRefHandleBase {
public:
    TId m_uiIndex;  ///< +0x4
};

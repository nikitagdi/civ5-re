/// origin binary
#pragma once
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FUncopyable.h"

class CvNexusListener;
class CvNexusViewManager;
template <class T0, eMPoolType T1, int T2> class FNewInstanceBuilder;

/// size ?
template <class T0, class T1>
class FGlobalFacility : private FUncopyable {
public:
    static UNK_DATA sm_pkInstance[0x4];
};
/// 0x0a3d35a4 FGlobalFacility<CvNexusListener, FNewInstanceBuilder<CvNexusListener, (eMPoolType)26, 0> >
/// 0x0c078ac0 FGlobalFacility<CvNexusViewManager, FNewInstanceBuilder<CvNexusViewManager, (eMPoolType)26, 0> >
template <class T0, class T1>
UNK_DATA FGlobalFacility<T0, T1>::sm_pkInstance[0x4];

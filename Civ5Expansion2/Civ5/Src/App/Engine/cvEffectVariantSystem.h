/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "Civ5/Src/App/Graphics/cvUnitLibrary.h"
#include "Civ5/Src/App/cvCityCombatSim.h"

class FGXVector3;
class cvUnitSimDataDef;
class cvUnitSimSystem;
template <class T0> class cvUnitSimSystemAccessor;

/// size ?
class cvEffectVariantSystem {
public:
    /// 0x087e87b0
    cvEffectVariantSystem();
    /// 0x087e87b2
    ~cvEffectVariantSystem();
    /// 0x087e87b4
    UNK_RET Startup();
    /// 0x087e87b6
    UNK_RET Shutdown();
    /// 0x087e87b8
    UNK_RET GenerateVariantHandle(char const* a1, char const** a2, char const** a3, int a4, unsigned int a5);
    /// 0x087e8a14
    UNK_RET GenerateVariantHandle(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>& a1, char const* a2, unsigned int a3);
    /// 0x087e8d2e
    UNK_RET GetTerrainTagAtCenterPosition(FGXVector3& a1);
    /// 0x087e9094
    UNK_RET GetUnitMemberMaterialTag(cvUnitLibrary::cvUnitLibraryUnitMemberEntry const* a1, bool a2);
    /// 0x087e90d8
    UNK_RET GetUnitMemberImpactWeaponTypeTag(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>& a1, int a2, bool a3);
    /// 0x087e9186
    UNK_RET GetUnitWeaponTypeTag(cvGeneralRefHandle<cvUnitSimSystem, unsigned int, cvUnitSimDataDef, cvUnitSimSystemAccessor>& a1, int a2, bool a3);
    /// 0x087e9234
    UNK_RET GetUnitWeaponTypeTag(cvCityCombatSim::HandleRef& a1, int a2, bool a3);
    /// 0x087e92ce
    UNK_RET GenerateVariantHandle(cvCityCombatSim::HandleRef& a1, char const* a2, unsigned int a3);
    /// 0x087e9456
    UNK_RET GetUnitMemberImpactWeaponTypeTag(cvCityCombatSim::HandleRef& a1, int a2, bool a3);
    /// 0x087e94f0
    UNK_RET GenerateDefaultVariantHandle(char const* a1, unsigned int a2);
    /// 0x087e9550
    UNK_RET ValidateMacros(char const* a1, unsigned int* a2);
};

/// 0x087e89a2
UNK_RET IsSoundTag(char const* a1);

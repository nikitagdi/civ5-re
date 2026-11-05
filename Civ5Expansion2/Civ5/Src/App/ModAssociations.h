/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"
#include <list>
#include <memory>
#include <vector>

class FVersionInfo;

/// size ?
class ModAssociations {
public:
    class DlcInfo;
    class ModInfo;
    class CheckRequirementsStatus;
    /// size ?
    class DlcInfo {
    public:
        /// 0x08c7307c
        DlcInfo(char const* a1, int a2);
        /// 0x08c730c6
        DlcInfo(ModAssociations::DlcInfo const& a1);
        /// 0x08c73102
        UNK_RET GetDlcId() const;
        /// 0x08c73108
        UNK_RET operator=(ModAssociations::DlcInfo const& a1);
        /// 0x08c7315a
        UNK_RET operator==(ModAssociations::DlcInfo const& a1) const;
        /// 0x08c731a0
        UNK_RET GetVersion() const;
    };
    /// size ?
    class ModInfo {
    public:
        /// 0x08c731a8
        ModInfo(char const* a1, int a2);
        /// 0x08c731f2
        ModInfo(ModAssociations::ModInfo const& a1);
        /// 0x08c7322e
        UNK_RET GetModId() const;
        /// 0x08c73234
        UNK_RET operator=(ModAssociations::ModInfo const& a1);
        /// 0x08c73286
        UNK_RET operator==(ModAssociations::ModInfo const& a1) const;
        /// 0x08c732cc
        UNK_RET GetVersion() const;
    };
    /// 0x08c732d4
    ModAssociations(Database::Connection& a1, FVersionInfo a2, std::list<ModAssociations::DlcInfo, std::allocator<ModAssociations::DlcInfo> >& a3, std::list<ModAssociations::DlcInfo, std::allocator<ModAssociations::DlcInfo> >& a4);
    /// 0x08c735ee
    UNK_RET GetActivationOrder(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1);
    /// 0x08c739ba
    UNK_RET GetReferencedMods(ModAssociations::ModInfo const& a1, std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a2) const;
    /// 0x08c73d3e
    UNK_RET GetDLCToEnable(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1);
    /// 0x08c743a6
    UNK_RET CheckRequirements(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1);
    /// 0x08c74422
    UNK_RET ContainsDuplicateMods(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1, ModAssociations::CheckRequirementsStatus& a2) const;
    /// 0x08c7448e
    UNK_RET ContainsAllDependencies(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1, ModAssociations::CheckRequirementsStatus& a2) const;
    /// 0x08c748fe
    UNK_RET ContainsBlockers(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1, ModAssociations::CheckRequirementsStatus& a2) const;
    /// 0x08c74d86
    UNK_RET HasExclusivityConflicts(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1, ModAssociations::CheckRequirementsStatus& a2) const;
    /// 0x08c74fd2
    UNK_RET GetRequiredMods(ModAssociations::ModInfo const& a1, std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a2) const;
    /// 0x08c75226
    UNK_RET HasFiraxisScenarioConflicts(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1, ModAssociations::CheckRequirementsStatus& a2) const;
    /// 0x08c752d8
    UNK_RET HasDLLConflicts(std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> > const& a1, ModAssociations::CheckRequirementsStatus& a2) const;
};

/// 0x08c7540b VectorIndexOf<ModAssociations::ModInfo>
template <class T0>
unsigned int VectorIndexOf(std::vector<T0, std::allocator<T0> > const& a1, T0 const& a2);
/// 0x08c7557d ListContains<std::list<ModAssociations::ModInfo, std::allocator<ModAssociations::ModInfo> >, ModAssociations::ModInfo>
template <class T0, class T1>
bool ListContains(T0 const& a1, T1 const& a2);

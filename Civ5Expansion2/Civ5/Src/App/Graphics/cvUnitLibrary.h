/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
class FGXGrannyCompositeContainerTemplate;
class FStringHashPalette;
class FXmlDocument;
class FXmlTraverser;
class cvTypeMap;
namespace FireGrafix {
enum FGX_SPIV_GENERIC : int;
}

/// size ?
class cvUnitLibrary {
public:
    /// 0x08925944
    cvUnitLibrary();

    /// 0x089259e2
    ~cvUnitLibrary();
    class cvUnitLibraryUnitEntry;
    class cvUnitLibraryUnitMemberEntry;
    /// size ?
    class cvUnitLibraryUnitEntry {
    public:
        /// 0x0892c2b8
        cvUnitLibraryUnitEntry();
        /// 0x0892c3fa
        ~cvUnitLibraryUnitEntry();
        /// 0x0892765a
        UNK_RET ReloadEntry(cvUnitLibrary& a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a2);
        /// 0x0892b0de
        UNK_RET LoadEntry(FXmlTraverser const& a1, cvUnitLibrary& a2, cvTypeMap const& a3, float a4, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a5);
        /// 0x0892b926
        UNK_RET GetType() const;
        /// 0x0892b92e
        UNK_RET LoadEntry(Database::Results& a1, Database::Results& a2, Database::Results& a3, cvUnitLibrary& a4, cvTypeMap const& a5, float a6, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a7);
        /// 0x0892c23e
        UNK_RET GetUnitMemberType(unsigned int a1) const;
        /// 0x0892c76e
        UNK_RET GetUnitMemberCount() const;
        /// 0x0892c776
        UNK_RET GetContainerTemplate() const;
        /// 0x0892c780
        UNK_RET CalcUnitMemberCount(FGXGrannyCompositeContainerTemplate const& a1) const;
    };
    /// size 0x530
    class cvUnitLibraryUnitMemberEntry {
    public:
        class CombatConstants;
        /// size 0x370
        class CombatConstants {
        public:
            enum WEAPON_TYPES : int;
            class WeaponInfo;
            /// size 0x40
            class WeaponInfo {
            public:
                unsigned int m_uiUsageMask;            ///< +0x0
                unsigned int m_uiID;                   ///< +0x4
                float m_fVisKillStrengthMin;           ///< +0x8
                float m_fVisKillStrengthMax;           ///< +0xc
                float m_fProjectileSpeed;              ///< +0x10
                float m_fProjectileTurnRateMin;        ///< +0x14
                float m_fProjectileTurnRateMax;        ///< +0x18
                unsigned int m_hHitEffect;             ///< +0x1c
                float m_fHitEffectScale;               ///< +0x20
                float m_fHitRadius;                    ///< +0x24
                float m_fMissTargetSlopRadius;         ///< +0x28
                float m_fProjectileChildEffectScale;   ///< +0x2c
                float m_fAreaDamageDelay;              ///< +0x30
                bool m_bContinuousFire;                ///< +0x34
                bool m_bWaitForEffectCompletion;       ///< +0x35
                bool m_bTargetGround;                  ///< +0x36
                bool m_bIsDropped;                     ///< +0x37
                unsigned int m_uiWeaponTypeTag;        ///< +0x38
                unsigned int m_uiWeaponTypeSoundOverrideTag; ///< +0x3c
            };
            float m_fMoveRadius;                      ///< +0x0
            float m_fShortMoveRadius;                  ///< +0x4
            float m_fChargeRadius;                     ///< +0x8
            float m_fAttackRadius;                     ///< +0xc
            float m_fRangedAttackRadius;               ///< +0x10
            float m_fMoveRate;                         ///< +0x14
            float m_fShortMoveRate;                    ///< +0x18
            float m_fTurnRateMin;                      ///< +0x1c
            float m_fTurnRateMax;                      ///< +0x20
            float m_fTurnFacingRateMin;                ///< +0x24
            float m_fTurnFacingRateMax;                ///< +0x28
            float m_fRollRateMin;                      ///< +0x2c
            float m_fRollRateMax;                      ///< +0x30
            float m_fPitchRateMin;                     ///< +0x34
            float m_fPitchRateMax;                     ///< +0x38
            float m_fLOSRadiusScale;                   ///< +0x3c
            float m_fTargetRadius;                     ///< +0x40
            float m_fTargetHeight;                     ///< +0x44
            bool m_bHasShortRangedAttack;              ///< +0x48
            bool m_bHasLongRangedAttack;               ///< +0x49
            bool m_bHasLeftRightAttack;                ///< +0x4a
            bool m_bHasStationaryMelee;                ///< +0x4b
            bool m_bHasStationaryRangedAttack;         ///< +0x4c
            bool m_bHasIndependentWeaponFacing;        ///< +0x4d
            bool m_bHasCollisionAttack;                ///< +0x4e
            bool m_bHasRefaceAfterCombat;              ///< +0x4f
            bool m_bHasOpponentTracking;               ///< +0x50
            bool m_bOnlyTurnInMovementActions;         ///< +0x51
            bool m_bLastToDie;                         ///< +0x52
            char unk_53[1];                            ///< +0x53
            int m_iReformBeforeCombat;                 ///< +0x54
            unsigned int m_uiRushAttackFormation;      ///< +0x58
            float m_fAttackAltitude;                   ///< +0x5c
            float m_fAltitudeDecelerationDistance;     ///< +0x60
            unsigned int m_uiEnabledActions;           ///< +0x64
            float m_fDeathRotationAmount;              ///< +0x68
            WeaponInfo m_weapons[6][2];                ///< +0x6c
            char unk_36c[4];                           ///< +0x36c

            /// 0x0892e110
            UNK_RET SupportsActionCode(int a1) const;
            /// 0x0892e152
            UNK_RET GetWeaponByID(cvUnitLibrary::cvUnitLibraryUnitMemberEntry::CombatConstants::WEAPON_TYPES a1, unsigned int a2) const;
            /// 0x0892e17c
            UNK_RET GetWeaponByUsage(unsigned int a1) const;
        };
        /// 0x0892c79e
        cvUnitLibraryUnitMemberEntry();
        /// 0x0892c816
        ~cvUnitLibraryUnitMemberEntry();
        /// 0x089279b4
        UNK_RET LoadEntry(FXmlTraverser const& a1, float a2, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>* a3, std::map<unsigned int, cvUnitLibrary::cvUnitLibraryUnitMemberEntry const*, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, cvUnitLibrary::cvUnitLibraryUnitMemberEntry const*> > > const* a4, FStringHashPalette& a5);
        /// 0x08929796
        UNK_RET LoadEntry(Database::Results& a1, Database::Results& a2, Database::Results& a3, float a4, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>* a5, FStringHashPalette& a6);
        /// 0x0892c5c8
        UNK_RET BindAsset(FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>& a1);
        /// 0x0892c83c
        UNK_RET LoadWeaponEntry(FXmlTraverser const& a1, int a2, int a3, FStringHashPalette& a4);
        /// 0x0892d480
        UNK_RET LoadWeaponEntry(Database::Results& a1, int a2, int a3, FStringHashPalette& a4);
        char m_szType[0x80];                           ///< +0x0
        char m_szGranny[0x80];                         ///< +0x80
        char m_szMaterialTypeTag[0x20];                ///< +0x100
        char m_szMaterialTypeSoundOverrideTag[0x20];   ///< +0x120
        float m_fScale;                                ///< +0x140
        float m_fZOffset;                              ///< +0x144
        int m_eDomain;                                 ///< +0x148
        unsigned int m_uiFlags;                        ///< +0x14c
        CombatConstants m_combat;                      ///< +0x150
        char unk_4c0[0x70];                            ///< +0x4c0
    };
    /// 0x08925a60
    UNK_RET Release();
    /// 0x08925aa8
    UNK_RET Init(std::vector<std::string, std::allocator<std::string> > const& a1, char const* a2, float a3, float a4, bool a5);
    /// 0x08925b44
    UNK_RET InitShaderPackageInstances();
    /// 0x08925bfc
    UNK_RET LoadLibraryA(std::vector<std::string, std::allocator<std::string> > const& a1, float a2, float a3);
    /// 0x08925c98
    UNK_RET InitActionValuesMap(char const* a1);
    /// 0x08925f24
    UNK_RET Init(Database::Connection& a1, char const* a2, float a3, float a4, bool a5);
    /// 0x08926054
    UNK_RET InitUnitMemberEntries(Database::Connection& a1, float a2);
    /// 0x08926416
    UNK_RET InitUnitEntries(Database::Connection& a1, float a2);
    /// 0x089267d2
    UNK_RET InitUnitMemberEntries(std::list<FXmlDocument, std::allocator<FXmlDocument> > const& a1, float a2);
    /// 0x089271a8
    UNK_RET InitUnitEntries(std::list<FXmlDocument, std::allocator<FXmlDocument> > const& a1, float a2);
    /// 0x089274f0
    UNK_RET DeallocateUnitEntries();
    /// 0x0892754e
    UNK_RET DeallocateUnitMemberEntries();
    /// 0x089275ae
    UNK_RET ReleaseShaderPackageInstances();
    /// 0x089275d2
    UNK_RET ReloadUnitType(unsigned int a1);
    /// 0x08927614
    UNK_RET GetUnitEntry(unsigned int a1);
    /// 0x08927894
    UNK_RET GetUnitLibrary(unsigned int a1) const;
    /// 0x089278aa
    UNK_RET GetUnitMemberLibrary(unsigned int a1) const;
    /// 0x089278c0
    UNK_RET GetUnitLibrary(unsigned int a1);
    /// 0x089278d6
    UNK_RET GetUnitLibraryOffset(unsigned int a1) const;
    /// 0x089278f6
    UNK_RET GetUnitMemberLibrary(unsigned int a1);
    /// 0x0892790c
    UNK_RET GetUnitMemberLibraryOffset(unsigned int a1) const;
    /// 0x0892792c
    UNK_RET GetUnitLibraryID(unsigned int a1, unsigned int* a2, unsigned int* a3) const;
    /// 0x0892796e
    UNK_RET GetUnitEntry(unsigned int a1) const;
    /// 0x0892971c
    UNK_RET AllocateUnitMemberEntries(unsigned int a1);
    /// 0x0892b068
    UNK_RET AllocateUnitEntries(unsigned int a1);
    /// 0x0892c0ee
    UNK_RET GetUnitMemberLibraryID(unsigned int a1, unsigned int* a2, unsigned int* a3) const;
    /// 0x0892c132
    UNK_RET GetUnitDamageStateRange(unsigned int a1) const;
    /// 0x0892c16c
    UNK_RET GetUnitMemberEntry(unsigned int a1, unsigned int a2) const;
    /// 0x0892c27e
    UNK_RET GetHashedString(unsigned int a1) const;
};

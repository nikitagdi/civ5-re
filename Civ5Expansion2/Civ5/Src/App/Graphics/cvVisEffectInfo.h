/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"
#include "FirePlace/FireWorks/EventSystem/FastDelegate.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include <d3d9.h>
#include <lua.h>

class FXmlTraverser;
class cvStrategicViewTracker;
class cvTypeMap;
namespace cvStrategicViewAssetMap {
class AssetEntry;
}

/// size 0xbc
class cvVisEffectInfo {
public:
    unsigned char unk_0[0x60];        ///< +0x0
    D3DXMATRIX m_mTransform;          ///< +0x60
    float m_fScale;                   ///< +0xa0
    float m_fBaseDuration;            ///< +0xa4
    float m_fBaseDurationScale;       ///< +0xa8
    // CRC32 of the first ModelEffects Name; 0xffffffff when absent
    unsigned int m_uModelEffectHash;  ///< +0xac
    unsigned int m_uSoundTag;         ///< +0xb0
    float m_fMovementSpeed;           ///< +0xb4
    unsigned short m_usAnimation;     ///< +0xb8
    unsigned char m_ucFowMask;        ///< +0xba
    unsigned char unk_bb[0x1];        ///< +0xbb

    /// 0x08a59da4
    cvVisEffectInfo();
    /// 0x08a59e1c
    ~cvVisEffectInfo();
    /// 0x08a59e44
    UNK_RET Load(FXmlTraverser const& a1);
    /// 0x08a5a74c
    UNK_RET ChangeBaseDuration(float a1);
    /// 0x08a5a772
    UNK_RET GetTransformFromXml(FXmlTraverser const& a1, D3DXMATRIX& a2, float& a3);
    /// 0x08a5aa44
    UNK_RET Reload(FXmlTraverser const& a1);
    /// 0x08a5aad4
    UNK_RET FindChild(unsigned int a1) const;
};

/// 0x08a5bbc0
UNK_RET StrategicViewTrackerEventCount();
/// 0x08a5bbd3
UNK_RET AddStrategicViewTrackerEvent(void (*a1)(cvStrategicViewTracker*), void (*a2)(cvStrategicViewTracker*));
/// 0x08a5bc10
/// lua ?
int LuaThunk_UnitMoving(lua_State* a1);
/// 0x08a5bca4
UNK_RET UnitMoving(int a1, int a2);
/// 0x08a5bdf7
int LuaToString_UnitMoving(lua_State* a1);

namespace cvStrategicViewAssetMap {
/// 0x08a5af9c
UNK_RET Init(char const* a1, cvTypeMap const& a2, fastdelegate::FastDelegate1<char const*, unsigned int> const& a3, FFastVector<cvStrategicViewAssetMap::AssetEntry, true, 0u, 0u, BaseVector<cvStrategicViewAssetMap::AssetEntry, true>::FDefaultFastVectorAllocator>* a4);
/// 0x08a5b418
UNK_RET Init(Database::Results& a1, cvTypeMap const& a2, fastdelegate::FastDelegate1<char const*, unsigned int> const& a3, FFastVector<cvStrategicViewAssetMap::AssetEntry, true, 0u, 0u, BaseVector<cvStrategicViewAssetMap::AssetEntry, true>::FDefaultFastVectorAllocator>* a4);
/// 0x08a5b552
UNK_RET InitAndBuildTypeMap(char const* a1, cvTypeMap& a2, fastdelegate::FastDelegate1<char const*, unsigned int> const& a3);
/// 0x08a5b860
UNK_RET InitAndBuildTypeMap(Database::Results& a1, cvTypeMap& a2, fastdelegate::FastDelegate1<char const*, unsigned int> const& a3);
/// 0x08a5b94e
UNK_RET Shutdown();
}

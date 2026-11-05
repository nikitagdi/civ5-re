/// origin binary
#pragma once
#include <vector>
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/cvSystemBase.h"

/// size 0x8
struct Vec2 {
    float x;  ///< +0x0
    float y;  ///< +0x4
};

namespace FireWorks {

class cvUnitVis;

#define DECLARE_GETTER(type, name) type Get##name() const;
#define DECLARE_FIELD(type, name) type name;

// Gamecore events reach it through EventSystem, not direct calls.
/// size 0x3c
/// vtable 0x0a1b2c40
/// lua class UnitVisSystem xml
class cvUnitVisSystem : public cvSystemBase, public IEventListener {
public:
    /// 0x0834a000
    /// base 0x0834a080
    cvUnitVisSystem();

    /// 0x0834a100
    /// deleting 0x0834a180
    /// thunk 0x0834a1a0
    virtual ~cvUnitVisSystem();

    /// 0x0834a1c0
    /// thunk 0x0834a1e0
    virtual void OnEvent(int e);

    /// 0x0834a200
    Vec2 GetPos() const;

    /// 0x0834a240 fastcc
    static int Count(int a1, int a2);

    /// 0x0834a280
    DECLARE_GETTER(int, Flags)

    /// 0x0834a400
    UNK_RET Fun_0834a400(int a1);

    /// inline
    int GetCount() const { return m_iCount; }

    /// inline
    int GetTwice() const;

    /// 0x0a300000
    static int s_iInstances;

private:
    FFastVector<cvUnitVis*> m_aVis;  ///< +0x8
    int m_iCount;                    ///< +0x10
    DECLARE_FIELD(short, m_sKind)    ///< +0x14
    unsigned m_bFlagA : 1;           ///< +0x16
    unsigned m_bFlagB : 3;           ///< +0x16
    /// size 0x4
    union {
        int m_iRaw;    ///< +0x18
        float m_fRaw;  ///< +0x18
    };
    std::vector<int> m_aIds;         ///< +0x1c
    char unk_28[0x14];               ///< +0x28
};

}  // namespace FireWorks

#include "Civ5/Src/App/Graphics/cvUnitVisSystem.inl"

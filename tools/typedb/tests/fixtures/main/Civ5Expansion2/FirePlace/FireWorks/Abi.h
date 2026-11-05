/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"

/// size 0x8
struct PodPair {
    int a;  ///< +0x0
    int b;  ///< +0x4
};

/// size 0x8
struct CopyPair {
    /// inline
    CopyPair(const CopyPair& o) : a(o.a), b(o.b) {}
    int a;  ///< +0x0
    int b;  ///< +0x4
};

/// 0x08140000
Vec2 MakeVec(float x, float y);

/// 0x08140040
void TakePairs(PodPair p, CopyPair c);

/// inline
static int StaticHelper(int x) { return x; }

namespace {
/// inline
int AnonHelper(int x) { return x + 1; }
}

/// inline
const int kConst = 5;

/// 0x0a100000
extern UNK_DATA g_0a100000[0x10];

/// 0x08140080
UNK_RET Fun_08140080(UNK_PARAMS);

/// 0x081400c0
int Printf(const char* fmt, ...);

extern "C" {
/// 0x08140100
UNK_RET FSBAAllocate(UNK_PARAMS);
}

/// lua enum UnitState
enum UnitState {
    US_IDLE,
    US_MOVING = 4,
    US_DEAD
};

/// lua ?
typedef int (*UnitCallback)(int a1, void* a2);

/// 0x0a100100
extern UnitCallback g_pfnCallback;

#if !(defined(linux) && defined(unix) && defined(i386))
#error gnu++11 must define linux, unix and i386
#endif

// A non-POD base lends its tail padding to the derived class: dsize matters.
/// size 0xc
struct TailA {
    /// inline
    TailA() {}
    int a;   ///< +0x0
    int b;   ///< +0x4
    char c;  ///< +0x8
};

/// size 0xc
struct TailB : TailA {
    char d;  ///< +0x9
};

/// size 0x4
/// vtable 0x0a000500
class Sealed {
    /// 0x08150000
    /// deleting 0x08150040
    virtual ~Sealed();
};

namespace Gfx {
struct Tex;

/// 0x08150080
void Load(int a1);

/// 0x081500c0
void Load(float a1, Tex* a2);
}

/// origin binary
#pragma once
#include "FirePlace/FireWorks/FUncopyable.h"

class FBlockCurveLibrary;
class FResourceMgr;
class LuaContext;
namespace FireGrafix {
class TextureData;
}

/// size ?
template <class T0>
class FSingleton : private FUncopyable {
public:
    static UNK_DATA ms_Singleton[0x4];
};
/// 0x0d276a3c FSingleton<FBlockCurveLibrary>
/// 0x0a340e38 FSingleton<FResourceMgr>
template <class T0>
UNK_DATA FSingleton<T0>::ms_Singleton[0x4];

// State of the ColorKeySystem functions: a 32x32x32 key texture for every system and type, indexed [system][type].
// Terrain cell surfaces take the handles of systems 0, 1 and 2 by the cell's terrain class.
/// size 0xdc
class ColorKeyData {
public:
    FireGrafix::TextureData* m_apKeyTextures[5][5];  ///< +0x0
    unsigned int m_auKeyHandles[5][5];               ///< +0x64
    bool m_bSavePending;                             ///< +0xc8
    char unk_c9[0x3];                                ///< +0xc9
    int m_iSystem;                                   ///< +0xcc
    int m_iType;                                     ///< +0xd0
    LuaContext* m_pLuaContext;                       ///< +0xd4
    unsigned char m_ucFlags;                         ///< +0xd8
    char unk_d9[0x3];                                ///< +0xd9
};

extern "C" {
    /// 0x0a341188
    extern ColorKeyData g_ColorKeySystem;
    /// 0x0a340e3c
    extern UNK_DATA g_bTEST[0x1];
}

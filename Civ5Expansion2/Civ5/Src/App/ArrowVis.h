/// origin guess
#pragma once
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireWorks/FFastVector.h"

namespace FireGrafix {
template <int T0> class VStruct;
}

// One arc arrow: a B-spline from m_vStart to m_vEnd drawn as a growing tube with an arrowhead.
/// size 0x30
class ArrowVis {
public:
    // Vertex format 5; the constructor zeroes position and normal only.
    /// size 0x28
    class ArrowVertex {
    public:
        FGXVector3 m_vPosition;   ///< +0x0
        // Zero on outline vertices.
        FGXVector3 m_vNormal;     ///< +0xc
        // Packed 0xAABBGGRR.
        unsigned int m_uColor;    ///< +0x18
        // 0xffffffff on body and head vertices, 0 on outline vertices.
        unsigned int m_uBodyFlag; ///< +0x1c
        unsigned char unk_20[0x8]; ///< +0x20
    };

    /// 0x0884d0a8
    ArrowVis();
    /// 0x0884d104
    UNK_RET CreateBuffers();
    /// 0x0884d172
    UNK_RET ReleaseBuffers();
    // Rebuilds the shaft, head-base collar, head and outline mesh up to m_fProgress and uploads it.
    /// 0x0884d1ce
    UNK_RET UpdateArrowBuffers(FFastVector<FireGrafix::VStruct<5>, true, 62u, 0u, BaseVector<FireGrafix::VStruct<5>, true>::FDefaultFastVectorAllocator>& aVertices, FFastVector<unsigned int, true, 62u, 0u, BaseVector<unsigned int, true>::FDefaultFastVectorAllocator>& aIndices);
    /// 0x08851158
    UNK_RET Update(float fDeltaTime);
    // Tests the point against the six view clip planes of the given camera.
    /// 0x088511bc
    static bool IsOnScreen(FGXVector3 const& a1, int a2);

    FGXVector3 m_vStart;         ///< +0x0
    FGXVector3 m_vEnd;           ///< +0xc
    bool m_bDirty;               ///< +0x18
    bool m_bAnimating;           ///< +0x19
    bool m_bHidden;              ///< +0x1a
    unsigned char unk_1b[0x1];   ///< +0x1b
    float m_fProgress;           ///< +0x1c
    float m_fGrowRate;           ///< +0x20
    unsigned int m_hSurface;     ///< +0x24
    unsigned int m_hVB;          ///< +0x28
    unsigned int m_hIB;          ///< +0x2c
};

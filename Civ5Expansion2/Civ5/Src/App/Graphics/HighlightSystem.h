/// origin guess
#pragma once
#include "Civ5/Src/App/HighlightLibrary.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FString.h"
#include <d3d9.h>

class ClearHexHighlightStyleData;
class FGXVector4;
class HexEdge;
class HexHighlightData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
}
namespace GameSpline {
class ControlPoint;
class Segment;
}

namespace FireGrafix {
class DrawSimpleOverlay;
}

/// size 0x90
class HighlightSystem {
public:
    class SplineHighlightHexGrid_BranchFunc;
    /// size 0xc
    class HighlightArray {
    public:
        void* m_pData;             ///< +0x0
        unsigned int m_nCount;     ///< +0x4
        unsigned int m_nCapacity;  ///< +0x8
    };
    /// size 0xac
    class HexCellMap {
    public:
        char unk_0[0x94];          ///< +0x0
        int m_iWidth;              ///< +0x94
        int m_iHeight;             ///< +0x98
        bool m_bWrapX;             ///< +0x9c
        char unk_9d[0x3];          ///< +0x9d
        unsigned char* m_pCells;   ///< +0xa0
        unsigned int m_nCells;     ///< +0xa4
        unsigned int m_nCellsCapacity; ///< +0xa8
    };
    /// size 0x30
    class FilledHexStyle {
    public:
        char* m_pszName;           ///< +0x0
        char unk_4[0x4];           ///< +0x4
        FStringA m_sUnused;        ///< +0x8
        char unk_c[0x4];           ///< +0xc
        float m_fScale;            ///< +0x10
        float m_afColor[4];        ///< +0x14
        HighlightArray m_kHexes;   ///< +0x24
    };
    /// size 0xc8
    class HexStyle {
    public:
        char* m_pszName;           ///< +0x0
        char unk_4[0x4];           ///< +0x4
        FStringA m_sTexture;       ///< +0x8
        bool m_bBuilt;             ///< +0xc
        char unk_d[0x3];           ///< +0xd
        unsigned int m_hVB;        ///< +0x10
        unsigned int m_hIB;        ///< +0x14
        unsigned int m_hSurface;   ///< +0x18
        HexCellMap m_kCells;       ///< +0x1c
    };
    /// size 0x4d0
    class ModelStyle {
    public:
        char* m_pszName;           ///< +0x0
        char unk_4[0x8];           ///< +0x4
        bool m_bBuilt;             ///< +0xc
        char unk_d[0x3];           ///< +0xd
        FStringA m_sModel;         ///< +0x10
        char unk_14[0x404];        ///< +0x14
        HexCellMap m_kCells;       ///< +0x418
        char unk_4c4[0xc];         ///< +0x4c4
    };
    /// size ?
    class SplineHighlightHexGrid_BranchFunc {
    public:
        /// 0x08a4f778
        UNK_RET operator()(HexEdge const& a1, HexEdge const& a2, HexEdge const& a3);
    };
    /// size 0x15c
    class SplineStyle {
    public:
        char* m_pszName;           ///< +0x0
        float m_fWidth;            ///< +0x4
        FStringA m_sTexture;       ///< +0x8
        bool m_bBuilt;             ///< +0xc
        char unk_d[0x3];           ///< +0xd
        HighlightArray m_kVBs;     ///< +0x10
        char unk_1c[0x14];         ///< +0x1c
        HighlightArray m_kSurfaces; ///< +0x30
        char unk_3c[0x14];         ///< +0x3c
        HighlightArray m_kBounds;  ///< +0x50
        char unk_5c[0x44];         ///< +0x5c
        HexCellMap m_kCells;       ///< +0xa0
        float m_afColor[4];        ///< +0x14c
    };
    /// size 0x20
    class StaticTextureStyle {
    public:
        char* m_pszName;           ///< +0x0
        char unk_4[0x4];           ///< +0x4
        FStringA m_sTexture;       ///< +0x8
        char unk_c[0x4];           ///< +0xc
        unsigned int m_hSurface;   ///< +0x10
        HighlightArray m_kHexes;   ///< +0x14
    };
    /// 0x08a4f946
    HighlightSystem();
    /// 0x08a4f98c
    ~HighlightSystem();
    /// 0x08a4fa0c
    UNK_RET Startup();
    /// 0x08a4fa6c
    UNK_RET BuildSharedBuffers();
    /// 0x08a4ffe2
    UNK_RET LoadFromXML();
    /// 0x08a50082
    UNK_RET SubscribeToEvents();
    /// 0x08a500fe
    UNK_RET Shutdown();
    /// 0x08a50450
    UNK_RET UnSubscribeToEvents();
    /// 0x08a504cc
    UNK_RET DestroySharedBuffers();
    /// 0x08a50584
    UNK_RET StartupGame();
    /// 0x08a506f6
    UNK_RET ShutdownGame();
    /// 0x08a5086c
    UNK_RET Render(FireGrafix::CCommandStream* a1, int a2);
    /// 0x08a5098e
    UNK_RET RenderHexContourStyle(FireGrafix::CCommandStream* a1, int a2, HighlightSystem::HexStyle& a3);
    /// 0x08a50ec0
    UNK_RET RenderSplineBorderStyle(FireGrafix::CCommandStream* a1, int a2, HighlightSystem::SplineStyle& a3);
    /// 0x08a511da
    UNK_RET RenderHexModelStyle(FireGrafix::CCommandStream* a1, int a2, HighlightSystem::ModelStyle& a3);
    /// 0x08a51616
    UNK_RET RenderStaticTextureStyle(FireGrafix::CCommandStream* a1, int a2, HighlightSystem::StaticTextureStyle& a3);
    /// 0x08a519f4
    UNK_RET RenderFilledHexStyle(FireGrafix::CCommandStream* a1, int a2, HighlightSystem::FilledHexStyle& a3);
    /// 0x08a51e42
    UNK_RET BuildSplineBorderStyle(HighlightSystem::SplineStyle& a1);
    /// 0x08a52766
    UNK_RET BuildHexModelStyle(HighlightSystem::ModelStyle& a1);
    /// 0x08a5292a
    UNK_RET Update(float a1);
    /// 0x08a52a30
    UNK_RET OnHexHighlight(EventTemplate<Event_Int2Type<114>, HexHighlightData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a52e7e
    UNK_RET OnClearHexHighlights(EventTemplate<Event_Int2Type<115>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a53126
    UNK_RET OnClearHexHighlightStyle(EventTemplate<Event_Int2Type<116>, ClearHexHighlightStyleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a5342a
    UNK_RET AddHexContourStyle(HighlightLibrary::Entry const& a1);
    /// 0x08a53a12
    UNK_RET AddSplineBorderStyle(HighlightLibrary::Entry const& a1);
    /// 0x08a53ba8
    UNK_RET AddHexModelStyle(HighlightLibrary::Entry const& a1);
    /// 0x08a53e28
    UNK_RET AddStaticTextureStyle(HighlightLibrary::Entry const& a1);
    /// 0x08a53ec8
    UNK_RET AddFilledHexStyle(HighlightLibrary::Entry const& a1);
    /// 0x08a53f6a
    UNK_RET BuildStaticTextureStyle(HighlightSystem::StaticTextureStyle& a1, char const* a2) const;
    /// 0x08a54008
    UNK_RET LoadHexModelIntoStyleRenderList(HighlightSystem::ModelStyle& a1, D3DXMATRIX const& a2);
    /// 0x08a5410a
    UNK_RET TraverseBorder(HighlightSystem::SplineStyle& a1, HexEdge const& a2, FFastVector<HexEdge, true, 42u, 0u, BaseVector<HexEdge, true>::FDefaultFastVectorAllocator>& a3, FFastVector<HexEdge, true, 42u, 0u, BaseVector<HexEdge, true>::FDefaultFastVectorAllocator>& a4) const;
    /// 0x08a54382
    UNK_RET BuildCPsFromEdges(HighlightSystem::SplineStyle& a1, FFastVector<GameSpline::ControlPoint, true, 42u, 0u, BaseVector<GameSpline::ControlPoint, true>::FDefaultFastVectorAllocator>& a2, FFastVector<HexEdge, true, 42u, 0u, BaseVector<HexEdge, true>::FDefaultFastVectorAllocator> const& a3) const;
    /// 0x08a5469c
    UNK_RET TessellateSplineBorderStyle(HighlightSystem::SplineStyle& a1, FFastVector<GameSpline::ControlPoint, true, 42u, 0u, BaseVector<GameSpline::ControlPoint, true>::FDefaultFastVectorAllocator>& a2);
    /// 0x08a54b94
    UNK_RET GenerateHexSegment(HexEdge a1, GameSpline::Segment* a2) const;
    /// 0x08a54ea8
    UNK_RET ExtractColor(FStringA const& a1, FGXVector4& a2) const;

    int unk_0;                                  ///< +0x0
    FireGrafix::DrawSimpleOverlay* m_pOverlay;  ///< +0x4
    bool m_bGameStarted;                        ///< +0x8
    char unk_9[0x3];                            ///< +0x9
    HighlightArray m_kHexStyles;                ///< +0xc
    HighlightArray m_kSplineStyles;             ///< +0x18
    HighlightArray m_kModelStyles;              ///< +0x24
    HighlightArray m_kStaticTextureStyles;      ///< +0x30
    HighlightArray m_kFilledHexStyles;          ///< +0x3c
    unsigned int m_hQuadIB;                     ///< +0x48
    unsigned int m_hStaticTextureVB;            ///< +0x4c
    unsigned int m_hStaticTextureIB;            ///< +0x50
    unsigned int m_hHexFanVB;                   ///< +0x54
    unsigned int m_hHexFanIB;                   ///< +0x58
    unsigned int m_hHexFanSurface;              ///< +0x5c
    HighlightArray m_kSplineVertices;           ///< +0x60
    HighlightArray m_kControlPoints;            ///< +0x6c
    HighlightArray m_kVisitedEdges;             ///< +0x78
    HighlightArray m_kBorderEdges;              ///< +0x84
};

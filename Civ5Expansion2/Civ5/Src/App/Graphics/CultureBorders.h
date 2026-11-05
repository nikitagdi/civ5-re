/// origin guess
#pragma once
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FString.h"

class HexCultureChangedData;
class HexEdge;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
struct TerrainEventDelegate;
namespace FireGrafix {
class CCommandStream;
class DrawSimpleOverlay;
}
namespace GameSpline {
class Segment;
}

/// size ?
class CultureBorders {
public:
    class CivIDShouldBranch;
    /// size ?
    class CivIDShouldBranch {
    public:
        /// 0x08a4a106
        UNK_RET operator()(HexEdge const& a1, HexEdge const& a2, HexEdge const& a3);
    };
    class SplineDescription;
    /// 0x08a47ffc
    CultureBorders();
    /// 0x08a48306
    ~CultureBorders();
    /// 0x08a483ac
    UNK_RET Startup();
    /// 0x08a4855c
    UNK_RET LoadFromXML();
    /// 0x08a48588
    UNK_RET OnHexCultureChanged(GameCoreEventTemplate<Event_Int2Type<29>, HexCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a48648
    UNK_RET OnUpdateAllHexCulture(GameCoreEventTemplate<Event_Int2Type<30>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a48658
    UNK_RET Shutdown();
    /// 0x08a48758
    UNK_RET ClearRenderables();
    /// 0x08a4881c
    UNK_RET StartupGame();
    /// 0x08a48876
    UNK_RET ClearCivIDs();
    /// 0x08a488e8
    UNK_RET Update(float a1);
    /// 0x08a4895a
    UNK_RET UpdateSplines();
    /// 0x08a4899a
    UNK_RET ShutdownGame();
    /// 0x08a48a32
    UNK_RET LoadXML();
    /// 0x08a48df6
    UNK_RET Render(FireGrafix::CCommandStream* a1, int a2);
    /// 0x08a48fe0
    UNK_RET GenerateControlPoints();
    /// 0x08a4945e
    UNK_RET SmoothControlPoints();
    /// 0x08a49846
    UNK_RET ExpandControlPoints();
    /// 0x08a49b92
    UNK_RET RandomizeControlPoints();
    /// 0x08a49ff8
    UNK_RET GenerateSplineMeshes();
    /// 0x08a4a272
    UNK_RET TraverseBorder(unsigned char a1, HexEdge const& a2, FFastVector<HexEdge, true, 42u, 0u, BaseVector<HexEdge, true>::FDefaultFastVectorAllocator>& a3, FFastVector<HexEdge, true, 42u, 0u, BaseVector<HexEdge, true>::FDefaultFastVectorAllocator>& a4) const;
    /// 0x08a4a4f2
    UNK_RET AddControlPointsFromBorderList(unsigned char a1, FFastVector<HexEdge, true, 42u, 0u, BaseVector<HexEdge, true>::FDefaultFastVectorAllocator> const& a2);
    /// 0x08a4a8ca
    UNK_RET GenerateHexSegment(HexEdge a1, GameSpline::Segment* a2) const;
    /// 0x08a4ac9e
    UNK_RET GetBorderColors(unsigned long& a1, unsigned long& a2, unsigned long& a3, unsigned long& a4, unsigned char a5) const;
    /// 0x08a4b080
    UNK_RET GenerateMinorCivMesh(CultureBorders::SplineDescription const& a1);
    /// 0x08a4b742
    UNK_RET GenerateMajorCivMesh(CultureBorders::SplineDescription const& a1);
    /// 0x08a4be8e
    UNK_RET BuildBufferAndPendingSurfaces(int* a1);
    /// 0x08a4c1f2
    UNK_RET GetPhysicalSplineLength(CultureBorders::SplineDescription const& a1) const;
    /// 0x08a4c36e
    UNK_RET CalculateDashWidth(CultureBorders::SplineDescription const& a1) const;
    /// 0x08a4c3e8
    UNK_RET GetAvailableBuffer();
};

// One corner of the hex ring mesh; HexOverlay keeps the 12 template corners of a hex, 1024 hexes are written to the vertex buffer.
/// size 0x28
struct HexOverlayVertex {
    float x;               ///< +0x0
    float y;               ///< +0x4
    float z;               ///< +0x8
    char unk_c[0xc];       ///< +0xc
    unsigned int uColor;   ///< +0x18
    char unk_1c[0x4];      ///< +0x1c
    float u;               ///< +0x20
    float v;               ///< +0x24
};

// Hex grid outline drawn over the terrain; passed as this at OverlaySystem+0x4 and at TerrainEditorOverlaySystem+0x30.
/// size 0x46c
class HexOverlay {
public:
    /// 0x0c08c628
    static HexOverlayVertex s_aVertexTemplate[12];
    /// 0x08a4d5ac
    void Startup();
    /// 0x08a4d78e
    void LoadXML();
    /// 0x08a4da76
    void OnHexGridOn(EventTemplate<Event_Int2Type<112>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x08a4da82
    void OnHexGridOff(EventTemplate<Event_Int2Type<113>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x08a4da8e
    void StartupGame();
    /// 0x08a4df64
    void Shutdown();
    /// 0x08a4e026
    void Update(float fDeltaTime);
    /// 0x08a4e06c
    void ExtractColor(FStringA const& kText, FGXVector4& vColor);
    /// 0x08a4e3f2
    void Render(FireGrafix::CCommandStream* pStream, int iCamera);

    char unk_0[0x4];                               ///< +0x0
    FireGrafix::DrawSimpleOverlay* m_pShader;       ///< +0x4
    unsigned int m_ahVertexBuffer[3];               ///< +0x8
    unsigned int m_ahIndexBuffer[3];                ///< +0x14
    int m_iVisible;                                 ///< +0x20
    FGXVector4 m_vColor;                            ///< +0x24
    float m_fWidth;                                 ///< +0x34
    char m_kFileMonitor[0x434];                     ///< +0x38
};

// Fills HexOverlay::s_aVertexTemplate; the width arrives in XMM0 (LLVM fastcc float register), not on the stack.
/// 0x08a4daab
void Fun_08a4daab(float fWidth);
// Event (un)subscribe helpers: the delegate holds the owner and its method pointer.
/// 0x08a4f3e6
UNK_RET Fun_08a4f3e6(TerrainEventDelegate* a1);
/// 0x08a4f054
UNK_RET Fun_08a4f054(TerrainEventDelegate* a1);
/// 0x08a4ee28
UNK_RET Fun_08a4ee28(TerrainEventDelegate* a1);
/// 0x08a4ebfc
UNK_RET Fun_08a4ebfc(TerrainEventDelegate* a1);

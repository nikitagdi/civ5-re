/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FUnrolledList_Core.h"

class FGXVector2;
class HEXVec_i;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class PlotPacketData;
class PlotPolygon;
class RoadVertex;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class EventPacketData;
template <class T0, class T1> class FUnrolledList_Tail_Pointer;
template <class T0, unsigned int T1, unsigned int T2> class NodeArray;
template <int T0> class Event_Int2Type;
namespace FGXMathRect {
class Rect;
}

// One placed landmark piece: matrix at +0x80 is composed with the plot translation.
/// size 0xc4
struct cvLandmarkSim_Piece {
    char unk_00[0x80];              ///< +0x0
    float m_afWorldMatrix[16];      ///< +0x80
    int m_iVisible;                 ///< +0xc0
};

/// size 0xc84
class cvLandmarkSim_Init {
public:
    unsigned char m_ucFlags;        ///< +0x0
    char unk_01[0x3];               ///< +0x1
    cvLandmarkSim_Piece m_aPieces[16];  ///< +0x4
    float m_fWorldX;                ///< +0xc44
    float m_fWorldY;                ///< +0xc48
    int m_iVariant;                 ///< +0xc4c
    unsigned int m_uiPieceCount;    ///< +0xc50
    unsigned char m_ucPieceFlag;    ///< +0xc54
    char unk_c55[0x3];              ///< +0xc55
    unsigned int m_uiLibraryEntry;  ///< +0xc58
    char unk_c5c[0x10];             ///< +0xc5c
    int m_iLandmarkType;            ///< +0xc6c
    unsigned short m_usC70;         ///< +0xc70
    unsigned short m_usC72;         ///< +0xc72
    int m_iVisState;                ///< +0xc74
    int m_iC78;                     ///< +0xc78
    int m_iC7C;                     ///< +0xc7c
    int m_iC80;                     ///< +0xc80
};

/// size 0xc90
class cvLandmarkSim {
public:
    cvLandmarkSim_Init m_Init;      ///< +0x0
    unsigned char m_bDirty;         ///< +0xc84
    char unk_c85[0x3];              ///< +0xc85
    unsigned int m_hVis;            ///< +0xc88
    bool m_bVisCreated;             ///< +0xc8c
    char unk_c8d[0x3];              ///< +0xc8d

    /// 0x08a21264
    cvLandmarkSim();
    /// 0x08a2129e
    ~cvLandmarkSim();
    /// 0x08a212a0
    UNK_RET DestroyData();
    /// 0x08a212e4
    UNK_RET UpdateData(cvLandmarkSim_Init const& a1);
    /// 0x08a21414
    UNK_RET UpdateState();
};

/// 0x08a2497a SendEventPackets<EventTemplate<Event_Int2Type<3>, EventPacketData<FGXVector2, PlotPacketData>, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>, PlotPacketData, FUnrolledList_Core<RoadVertex, NodeArray<RoadVertex, 20u, 9u>, FFastAllocator<NodeArray<RoadVertex, 20u, 9u>, true, 47u, 0u, BaseVector<NodeArray<RoadVertex, 20u, 9u>, true>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<RoadVertex, 20u, 9u>, FFastAllocator<NodeArray<RoadVertex, 20u, 9u>, true, 47u, 0u, BaseVector<NodeArray<RoadVertex, 20u, 9u>, true>::FDefaultFastVectorAllocator> > >::const_iterator>
template <class T0, class T1, class T2>
void SendEventPackets(T1 a1, T2 a2, unsigned int a3);

namespace cvPlotPolygonManager {
/// 0x08a21574
UNK_RET UpdateRange(FGXMathRect::Rect const& a1, HEXVec_i a2);
/// 0x08a2169e
UNK_RET ExtractPlotPolygonFromRoadNetwork(unsigned int a1);
/// 0x08a21f6c
UNK_RET ResolveValidPlotSet(FFastAllocator<PlotPolygon, false, 47u, 0u, BaseVector<PlotPolygon, false>::FDefaultFastVectorAllocator>& a1, HEXVec_i a2);
/// 0x08a226a2
UNK_RET ProcessPlots(FFastAllocator<PlotPolygon, false, 47u, 0u, BaseVector<PlotPolygon, false>::FDefaultFastVectorAllocator>& a1, HEXVec_i a2);
/// 0x08a22c2c
UNK_RET RemoveHexPlots(HEXVec_i a1);
/// 0x08a22cb8
UNK_RET ResendPlotsForHex(HEXVec_i a1);
}

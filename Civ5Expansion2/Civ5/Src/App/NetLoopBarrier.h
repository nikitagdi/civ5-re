/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faea0
class NetLoopBarrier : public FImmediateNetMessage {
public:
    /// 0x0870b85e
    NetLoopBarrier();
    /// 0x0870b8a0
    NetLoopBarrier(PlayerTypes a1);
    /// 0x0870ba22
    /// deleting 0x0870ba0e
    virtual ~NetLoopBarrier();
    /// 0x0870ba24
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087040d0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870ba92
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870bace
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x086f5110
    UNK_RET reset();
    /// 0x08703ef0
    UNK_RET barrierReached();
    /// 0x0870b1de
    UNK_RET receivedRandomNumberGeneratorSyncCheck();
    /// 0x0a3d3ad8
    static UNK_DATA s_count[0x4];
    /// 0x0a3d3adc
    static UNK_DATA s_syncChecks[0x4];
};

namespace Civ5NetMessage {
/// 0x086f5195
UNK_RET Uninit();
/// 0x086f51d3
UNK_RET GetTurnSliceMaxMessages();
/// 0x086f51e8
UNK_RET SetTurnSliceMaxMessages(int a1);
/// 0x086f520c
UNK_RET GetTurnSlicingActive();
/// 0x0a3d3aec
extern UNK_DATA s_hotJoiner[0x1];
/// 0x0a3d3aeb
extern UNK_DATA s_hotJoining[0x1];
/// 0x0a3d3aed
extern UNK_DATA s_hotJoiningSendReady[0x1];
/// 0x0a3d3aee
extern UNK_DATA s_inBarrier[0x1];
/// 0x0a3d3af4
extern UNK_DATA s_lastPlayerInfoUpdate[0x4];
/// 0x0a3d3af0
extern UNK_DATA s_loopCompleteMessages[0x4];
/// 0x0a3d3aef
extern UNK_DATA s_resyncing[0x1];
/// 0x0a3d3ae8
extern UNK_DATA s_sentTurnSlice[0x1];
/// 0x0a3d3ae9
extern UNK_DATA s_slicingActive[0x1];
/// 0x0a3d3ae0
extern UNK_DATA s_turnSlice[0x4];
/// 0x0a3d3ae4
extern UNK_DATA s_turnSlicesReceived[0x4];
/// 0x0a3d3afc
extern UNK_DATA s_uiTurnSliceLastSendTime[0x4];
/// 0x0a3d3aea
extern UNK_DATA s_usingTurnCompleteQueue[0x1];
/// 0x0a3d3af8
extern UNK_DATA s_wantForceResync[0x1];
}

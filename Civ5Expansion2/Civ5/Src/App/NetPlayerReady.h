/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faec0
class NetPlayerReady : public FImmediateNetMessage {
public:
    /// 0x0870bb0a
    NetPlayerReady();
    /// 0x0870bb4c
    NetPlayerReady(PlayerTypes a1);
    /// 0x0870bba4
    /// deleting 0x0870bb90
    virtual ~NetPlayerReady();
    /// 0x0870bba6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870bc8c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870bc14
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870bc50
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x086f50c2
    UNK_RET reset();
    /// 0x0870c082
    UNK_RET playersAreReady();
    /// 0x0870c0c2
    UNK_RET playersLeftToLoad();
    /// 0x0a3d3b00
    static UNK_DATA s_count[0x4];
};

namespace Civ5NetMessage {
/// 0x0a3d3b04
extern UNK_DATA s_localTurnSliceTime[0x338];
}

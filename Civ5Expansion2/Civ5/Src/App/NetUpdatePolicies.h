/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fab00
class NetUpdatePolicies : public FNetMessage {
public:
    /// 0x0870362a
    NetUpdatePolicies();
    /// 0x08703676
    NetUpdatePolicies(PlayerTypes a1, bool a2, int a3, bool a4);
    /// 0x087036e2
    /// deleting 0x087036ce
    virtual ~NetUpdatePolicies();
    /// 0x087036e4
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f67b8
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087038c6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08703930
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};

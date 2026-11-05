/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fab60
class NetAdvancedStartAction : public FNetMessage {
public:
    /// 0x08704dc4
    NetAdvancedStartAction(AdvancedStartActionTypes a1, PlayerTypes a2, int a3, int a4, int a5, bool a6);
    /// 0x08704e3e
    /// deleting 0x08704e2a
    virtual ~NetAdvancedStartAction();
    /// 0x08704e40
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08704f84
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08704e74
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08704efc
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};

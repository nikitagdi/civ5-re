/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fae80
class NetRandomNumberGeneratorSyncCheck : public FImmediateNetMessage {
public:
    /// 0x0870ab8e
    NetRandomNumberGeneratorSyncCheck();
    /// 0x0870ac40
    NetRandomNumberGeneratorSyncCheck(PlayerTypes a1, ICvRandom1* a2);
    /// 0x0870ad82
    /// deleting 0x0870ad5a
    virtual ~NetRandomNumberGeneratorSyncCheck();
    /// 0x0870aff2
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870b0da
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870ae12
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870af02
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};

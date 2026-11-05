/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fad80
class NetArchaeologyChoice : public FNetMessage {
public:
    /// 0x087075a4
    NetArchaeologyChoice();
    /// 0x087075f0
    NetArchaeologyChoice(PlayerTypes a1, int a2, ArchaeologyChoiceType a3);
    /// 0x0871aa9e
    virtual ~NetArchaeologyChoice();
    /// 0x08707640
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870767e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08707798
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087077fa
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};

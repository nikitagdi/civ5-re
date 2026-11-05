/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include <memory>
#include <string>

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304060
class NetRegister : public FImmediateNetMessage {
public:
    /// 0x08d52784
    NetRegister();
    /// 0x08d55b7a
    /// deleting 0x08d55daa
    virtual ~NetRegister();
    /// 0x08d52720
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d5283e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d52aba
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d52af6
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08d527fe
    UNK_RET SetVersionString(std::string const& a1);
    /// 0x08d5282a
    UNK_RET GetVersionString();
};

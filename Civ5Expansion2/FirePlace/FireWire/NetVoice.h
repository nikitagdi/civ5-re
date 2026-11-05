/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3040e0
class NetVoice : public FForgettableNetMessage {
public:
    /// 0x08d538be
    NetVoice(unsigned short a1, unsigned short* a2, unsigned short* a3, unsigned char** a4, unsigned short a5, double a6);
    /// 0x08d539d2
    /// deleting 0x08d539aa
    virtual ~NetVoice();
    /// 0x08d53cae
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d53d00
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d53ac8
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d53bac
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08d53a24
    UNK_RET GetNumSources() const;
    /// 0x08d53a2e
    UNK_RET GetChunk(int a1) const;
    /// 0x08d53a4e
    UNK_RET GetSources() const;
    /// 0x08d53a56
    UNK_RET GetSource(int a1) const;
    /// 0x08d53a78
    UNK_RET GetSizes() const;
    /// 0x08d53a82
    UNK_RET GetChunkSize(int a1) const;
    /// 0x08d53aa6
    UNK_RET GetChunks() const;
    /// 0x08d53ab0
    UNK_RET GetFrame() const;
    /// 0x08d53abc
    UNK_RET GetVolume() const;
};

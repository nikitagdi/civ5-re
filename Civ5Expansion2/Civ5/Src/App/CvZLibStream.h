/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"

/// size ?
/// vtable 0x0a2fdf30
class CvZLibStream : public FDataStream {
public:
    /// 0x08997b3c
    CvZLibStream(FDataStream& a1, bool a2, bool a3);
    /// 0x08997c6a
    /// deleting 0x08997c42
    virtual ~CvZLibStream();
    /// 0x08997cdc
    virtual bool AtEnd() const;
    /// 0x08997cda
    virtual void FastFwd();
    /// 0x08998198
    virtual void Flush();
    /// 0x08998306
    virtual unsigned int GetEOF() const;
    /// 0x08997d16
    virtual unsigned int ReadIt(unsigned int a1, void* a2);
    /// 0x08997cd8
    virtual void Rewind();
    /// 0x08997cd6
    virtual void Skip(unsigned int a1);
    /// 0x08997fcc
    virtual unsigned int WriteIt(unsigned int a1, void const* a2);
    /// 0x08997d04
    UNK_RET AtBlockEnd() const;
    /// 0x0899830c
    UNK_RET GetMD5();
};

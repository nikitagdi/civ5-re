/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include <memory>
#include <vector>

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3040c0
class NetRequestHostMigration : public FImmediateNetMessage {
public:
    /// 0x08d53622
    NetRequestHostMigration(int a1);
    /// 0x08d5367a
    /// deleting 0x08d53666
    virtual ~NetRequestHostMigration();
    /// 0x08d53850
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d53886
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d5367c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d536b8
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08d536f4
    UNK_RET DebugPrintHostMigrationPlayers(std::vector<int, std::allocator<int> > const& a1);
};

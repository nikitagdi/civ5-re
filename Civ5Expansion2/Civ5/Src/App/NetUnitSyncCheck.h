/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include "FirePlace/FireWorks/FMemoryStream.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fae20
class NetUnitSyncCheck : public FImmediateNetMessage {
public:
    /// 0x087089e0
    NetUnitSyncCheck();
    /// 0x08708a48
    NetUnitSyncCheck(PlayerTypes a1, int a2, FMemoryStream& a3, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > const& a4);
    /// 0x0871ab6a
    /// deleting 0x0871aba4
    virtual ~NetUnitSyncCheck();
    /// 0x08708c08
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08708c62
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08708aea
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08708b74
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};

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
/// vtable 0x0a2fae00
class NetCitySyncCheck : public FImmediateNetMessage {
public:
    /// 0x08707fb2
    NetCitySyncCheck();
    /// 0x0870801a
    NetCitySyncCheck(PlayerTypes a1, int a2, FMemoryStream& a3, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > const& a4);
    /// 0x0871aaee
    /// deleting 0x0871ab28
    virtual ~NetCitySyncCheck();
    /// 0x087081da
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08708234
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087080bc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08708146
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};

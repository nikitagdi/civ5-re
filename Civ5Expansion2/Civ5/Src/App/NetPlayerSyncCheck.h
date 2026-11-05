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
/// vtable 0x0a2fae60
class NetPlayerSyncCheck : public FImmediateNetMessage {
public:
    /// 0x0870a70a
    NetPlayerSyncCheck();
    /// 0x0870a76c
    NetPlayerSyncCheck(PlayerTypes a1, FMemoryStream& a2, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > const& a3);
    /// 0x0871ac62
    /// deleting 0x0871ac9c
    virtual ~NetPlayerSyncCheck();
    /// 0x0870a906
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870a95a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870a806
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870a882
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};

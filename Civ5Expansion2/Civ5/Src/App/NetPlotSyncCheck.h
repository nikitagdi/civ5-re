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
/// vtable 0x0a2fae40
class NetPlotSyncCheck : public FImmediateNetMessage {
public:
    /// 0x08709ca8
    NetPlotSyncCheck();
    /// 0x08709d10
    NetPlotSyncCheck(PlayerTypes a1, short a2, short a3, FMemoryStream& a4, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > const& a5);
    /// 0x0871abe6
    /// deleting 0x0871ac20
    virtual ~NetPlotSyncCheck();
    /// 0x08709ef8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08709f5c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08709dbc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08709e56
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};

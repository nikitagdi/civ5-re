/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvReplayHeader.h"
#include "Civ5/Src/App/CvSavedGameHeader.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include <Windows.h>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb460
class NetTurnTimerSync : public FImmediateNetMessage {
public:
    /// 0x08716f46
    NetTurnTimerSync();
    /// 0x08716f8e
    NetTurnTimerSync(float a1, float a2);
    /// 0x0871b06a
    virtual ~NetTurnTimerSync();
    /// 0x0871715c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    // Forwards both floats to ICvGame2::TurnTimerSync; no expiry check, no end-turn send.
    /// 0x08717076
    virtual void Execute(FNetSessionIFace* a1) const;
    /// 0x08716fde
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0871702a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);

    char unk_4[0x2c];       ///< +0x4
    float m_fCurTurnTime;   ///< +0x30
    float m_fTurnStartTime; ///< +0x34
};

/// 0x089f0d6e operator<< <CvReplayHeader::DlcInfo>
/// 0x089f0dd3 operator<< <CvReplayHeader::ModInfo>
/// 0x08997054 operator<< <CvSavedGameHeader::DlcInfo>
/// 0x089970b9 operator<< <CvSavedGameHeader::ModInfo>
/// 0x08719f9b operator<< <int>
/// 0x08719c8e operator<< <std::pair<std::string, std::string> >
/// 0x08719ada operator<< <unsigned char>
template <class T0>
FDataStream& operator<<(FDataStream& a1, std::vector<T0, std::allocator<T0> > const& a2);
/// 0x08719bbc operator<< <_GUID>
template <class T0>
FDataStream& operator<<(FDataStream& a1, std::list<T0, std::allocator<T0> > const& a2);

namespace FSerialization {
/// 0x08719a1c toString<MissionTypes>
template <class T0>
std::string toString(T0 const& a1);
}

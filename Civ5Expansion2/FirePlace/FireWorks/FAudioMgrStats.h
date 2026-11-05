/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FKnob;
class FSound3D;

/// size ?
class FAudioMgrStats {
public:
    /// 0x08d27634
    UNK_RET GetNumResidentBytesUsed() const;
    /// 0x08d276aa
    UNK_RET DumpAllStats(FStringA& a1) const;
    /// 0x08d284fc
    UNK_RET DumpVolumeStates(FStringA& a1) const;
    /// 0x08d2868c
    UNK_RET PrintVolumeAndChildren(FKnob* a1, FStringA& a2, int a3, FStringA& a4) const;
    /// 0x08d289c4
    UNK_RET PrintVolumeGraphic(int a1, FStringA& a2) const;
    /// 0x08d28afc
    UNK_RET Dump2DStats(FStringA& a1) const;
    /// 0x08d290ce
    UNK_RET Get2DStatLine(unsigned int a1, FStringA& a2) const;
    /// 0x08d29498
    UNK_RET Dump3DStats(FStringA& a1) const;
    /// 0x08d29980
    UNK_RET Get3DStatLine(unsigned int a1, FStringA& a2) const;
    /// 0x08d29c18
    UNK_RET Get3ChannelVolumeLine(unsigned int a1, FStringA& a2) const;
    /// 0x08d29e9c
    UNK_RET DumpStreamStats(FStringA& a1) const;
    /// 0x08d2a79e
    UNK_RET DumpDynaResStats(FStringA& a1) const;
    /// 0x08d2b332
    UNK_RET DumpResidentAudioStats(FStringA& a1) const;
    /// 0x08d2bda6
    UNK_RET Dump51Stats(FSound3D* a1, FStringA& a2) const;
};

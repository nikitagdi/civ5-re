/// origin guess
#pragma once
#include "FirePlace/FireWorks/FBaseSoundScriptData.h"

class FXmlTraverser;

/// size ?
/// vtable 0x0a303870
class F2DSoundScriptData : public FBaseSoundScriptData {
public:
    /// 0x08d38442
    F2DSoundScriptData();
    /// 0x08d384c8
    /// deleting 0x08d384b4
    virtual ~F2DSoundScriptData();
    /// 0x08d3947c
    virtual UNK_RET Clear();
    /// 0x08d392fa
    virtual UNK_RET IsValid();
    /// 0x08d38cee
    virtual UNK_RET LoadFromXml(FXmlTraverser& a1);
};

/// 0x08d3960c
UNK_RET GetAudioRandom();

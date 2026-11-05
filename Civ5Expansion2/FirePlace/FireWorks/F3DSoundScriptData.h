/// origin guess
#pragma once
#include "FirePlace/FireWorks/FBaseSoundScriptData.h"

class FXmlTraverser;

/// size ?
/// vtable 0x0a303850
class F3DSoundScriptData : public FBaseSoundScriptData {
public:
    /// 0x08d383a4
    F3DSoundScriptData();
    /// 0x08d384b2
    /// deleting 0x08d3849e
    virtual ~F3DSoundScriptData();
    /// 0x08d39526
    virtual UNK_RET Clear();
    /// 0x08d39350
    virtual UNK_RET IsValid();
    /// 0x08d38dba
    virtual UNK_RET LoadFromXml(FXmlTraverser& a1);
};

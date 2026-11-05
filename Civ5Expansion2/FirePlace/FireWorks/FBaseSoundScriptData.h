/// origin guess
#pragma once

class FXmlTraverser;

/// size ?
/// vtable 0x0a303890
class FBaseSoundScriptData {
public:
    /// 0x08d3841c
    FBaseSoundScriptData();
    /// 0x08d384de
    virtual UNK_RET LoadFromXml(FXmlTraverser& a1);
    /// 0x08d3914e
    virtual UNK_RET IsValid();
    /// 0x08d38440
    /// deleting 0x08d384ca
    virtual ~FBaseSoundScriptData();
    /// 0x08d393ee
    virtual UNK_RET Clear();
    /// 0x08d395ec
    UNK_RET GetLastErrorMsg() const;
};

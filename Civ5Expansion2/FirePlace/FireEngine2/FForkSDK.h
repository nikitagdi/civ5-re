/// origin guess
#pragma once

/// size ?
class FForkSDK {
public:
    /// 0x08cb6374
    FForkSDK();
    /// 0x08cb63a2
    ~FForkSDK();
    /// 0x08cb63a4
    UNK_RET OpenSDK(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08cb650c
    UNK_RET CloseSDK();
    /// 0x0d267898
    static UNK_DATA ms_csForkSDK[0x18];
};

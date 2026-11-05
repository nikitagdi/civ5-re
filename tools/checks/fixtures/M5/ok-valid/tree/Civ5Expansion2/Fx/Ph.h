/// origin guess
#pragma once

/// size 0x10
/// vtable 0x0a1b2f00
class Class_0a1b2f00 {
public:
    /// 0x08350010
    virtual void Alpha();
    /// 0x08350020
    virtual void Beta();
    /// 0x08350030
    virtual void vf_02(int a1, float a2);
    /// 0x08350040
    UNK_RET Fun_08350040(int a1);
    /// 0x08350050
    void Setup(UNK_PARAMS);
    int field_4;     ///< +0x4
    char unk_8[8];   ///< +0x8
};

/// 0x0b000100
extern int g_0b000100;

/// 0x0a920000
extern UNK_DATA g_0a920000[0x20];

extern "C" {
/// 0x0834a2c0
UNK_RET FSBAAllocate(UNK_PARAMS);
}

/// 0x08350060
void Take(int a1, int a2);

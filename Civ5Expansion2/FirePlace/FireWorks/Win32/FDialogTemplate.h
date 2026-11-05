/// origin binary
#pragma once

/// size ?
class FDialogTemplate {
public:
    /// 0x08dd2094
    FDialogTemplate(char const* a1, unsigned long a2, int a3, int a4, int a5, int a6, char const* a7, unsigned short a8);
    /// 0x08dd2278
    ~FDialogTemplate();
    /// 0x08dd2176
    UNK_RET AppendData(void* a1, int a2);
    /// 0x08dd21ba
    UNK_RET AppendString(char const* a1);
    /// 0x08dd227a
    UNK_RET AddComponent(char const* a1, char const* a2, unsigned long a3, unsigned long a4, int a5, int a6, int a7, int a8, unsigned short a9);
    /// 0x08dd2322
    UNK_RET AddButton(char const* a1, unsigned long a2, unsigned long a3, int a4, int a5, int a6, int a7, unsigned short a8);
    /// 0x08dd23a8
    UNK_RET AddStandardComponent(unsigned short a1, char const* a2, unsigned long a3, unsigned long a4, int a5, int a6, int a7, int a8, unsigned short a9);
    /// 0x08dd2478
    UNK_RET AddEditBox(char const* a1, unsigned long a2, unsigned long a3, int a4, int a5, int a6, int a7, unsigned short a8);
    /// 0x08dd24fe
    UNK_RET AddStatic(char const* a1, unsigned long a2, unsigned long a3, int a4, int a5, int a6, int a7, unsigned short a8);
    /// 0x08dd2584
    UNK_RET AddListBox(char const* a1, unsigned long a2, unsigned long a3, int a4, int a5, int a6, int a7, unsigned short a8);
    /// 0x08dd260a
    UNK_RET AddScrollBar(char const* a1, unsigned long a2, unsigned long a3, int a4, int a5, int a6, int a7, unsigned short a8);
    /// 0x08dd2690
    UNK_RET AddComboBox(char const* a1, unsigned long a2, unsigned long a3, int a4, int a5, int a6, int a7, unsigned short a8);
    /// 0x08dd2716
    UNK_RET GetDialogTemplate() const;
    /// 0x08dd271e
    UNK_RET AlignData(int a1);
    /// 0x08dd273e
    UNK_RET EnsureSpace(int a1);
};

/// 0x08dd28e4
UNK_RET GetCpuSpeed();
/// 0x08dd2b39
UNK_RET HasCPUID();
/// 0x08dd2b3c
UNK_RET ExCPUID(unsigned long a1, unsigned long* a2);
/// 0x08dd2b3d
UNK_RET CpuSignature(unsigned long a1);
/// 0x08dd2b3e
UNK_RET CpuFeatureInfo(unsigned long a1, unsigned long a2);
/// 0x08dd2b3f
UNK_RET CpuCacheInfo(unsigned long* a1);
/// 0x08dd2b40
UNK_RET CpuExFeatureInfo(unsigned long a1, unsigned long a2);
/// 0x08dd2b41
UNK_RET CpuMiscInfo(int a1);
/// 0x08dd2b42
UNK_RET GetCpuInfo();
/// 0x08dd2b43
UNK_RET FGetCPUBrandString(char* a1);
/// 0x08dd2b5c
UNK_RET DisplaySystemVersion();
/// 0x08dd2c8e
UNK_RET GetSystemRAM();
/// 0x08dd2cb3
UNK_RET GetSysClockRate();
/// 0x08dd2cf6
UNK_RET GetMmClockRate();
/// 0x08dd2d39
UNK_RET DisplaySystemInformation();
/// 0x08dd2f1f
UNK_RET FGetComputerNameA(char* a1, unsigned long& a2);
/// 0x08dd2f33
UNK_RET FGetShortOSVersion();
/// 0x08dd3042
UNK_RET Is32BitWindows();

namespace FAccessibilityFeatures {
/// 0x08dd2740
UNK_RET Disable();
/// 0x08dd287a
UNK_RET Restore();
}

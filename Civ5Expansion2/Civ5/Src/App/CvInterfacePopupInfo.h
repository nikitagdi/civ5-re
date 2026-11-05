/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvStructs.h"

class CvInterfacePopupInfo;

/// size ?
class CvInterfacePopupInfo {
public:
    /// 0x086a2f14
    CvInterfacePopupInfo();
    /// 0x086a2f4e
    CvInterfacePopupInfo(CvPopupInfo const& a1);
    /// 0x086a3068
    CvInterfacePopupInfo(int a1, int a2, int const* a3, int a4, bool const* a5);
    /// 0x086a3174
    CvInterfacePopupInfo(CvInterfacePopupInfo const& a1);
    /// 0x086a31f4
    UNK_RET operator=(CvInterfacePopupInfo const& a1);
};

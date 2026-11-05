/// origin binary
#pragma once
#include <Windows.h>
#include <jsoncpp.h>

/// size ?
class FVersionInfo {
public:
    /// 0x08dce160
    UNK_RET Retrieve(HINSTANCE__* a1);
    /// 0x08dce2d2
    UNK_RET Retrieve(char const* a1);
    /// 0x08dce62a
    UNK_RET Retrieve(wchar_t const* a1);
};

/// 0x08dce62b
UNK_RET GetJSonValue(Json::Value const& a1, char const* a2);

namespace FDebugHelperNS {
/// 0x08dd0a58
UNK_RET GetExceptionDescription(unsigned long a1);
/// 0x08dd0b79
UNK_RET AppendStackTrace(_tagSTACKFRAME64& a1);
/// 0x08dd0ef6
UNK_RET FillExceptionText();
/// 0x08dd1105
UNK_RET ExceptionDlgProc(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
/// 0x08dd1479
UNK_RET PrintExceptionInfo();
/// 0x08dd147a
UNK_RET DisplayExceptionDialog();
}

/// origin binary
#pragma once
#include "FirePlace/FireWorks/Win32/FWinAppBase.h"
#include <Windows.h>

/// size ?
/// vtable 0x0a2f9e90
class FWinMainApp : public FWinAppBase {
public:
    enum EScreenMode : int;
    char unk_4[0xa8];    ///< +0x4
    /// 0x086ed9b4
    /// deleting 0x086ed92a
    virtual ~FWinMainApp();
    /// 0x086ee608
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x086ee652
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x08dd0638
    virtual UNK_RET InitWindow(HINSTANCE__* a1, char const* a2, unsigned int a3, unsigned int a4, FWinMainApp::EScreenMode a5, HICON__* a6, HICON__* a7, bool a8);
    /// 0x08dd006c
    UNK_RET Exit(int a1);
    /// 0x08dd00d0
    UNK_RET ToggleFullScreen(bool a1);
    /// 0x08dd02a4
    UNK_RET GetAdjustedWindowSize(tagPOINT& a1, unsigned int a2, unsigned int a3, tagPOINT* a4);
    /// 0x08dd0370
    UNK_RET Create(HINSTANCE__* a1, char const* a2, char const* a3, unsigned int a4, unsigned int a5, FWinMainApp::EScreenMode a6, HICON__* a7, HICON__* a8, bool a9);
    /// 0x08dd0422
    UNK_RET Run();
    /// 0x08dd08ae
    UNK_RET WinProc(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x08dd0910
    UNK_RET LowLevelKeyboardProc(int a1, unsigned int a2, long a3);
    /// 0x08dd0962
    UNK_RET ProcessMessage(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
};

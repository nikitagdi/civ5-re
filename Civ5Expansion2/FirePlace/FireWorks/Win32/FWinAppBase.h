/// origin binary
#pragma once
#include <Windows.h>

/// size ?
/// vtable 0x0a2f9e50
class FWinAppBase {
public:
    /// 0x086eda32
    /// deleting 0x086ed6dc
    virtual ~FWinAppBase();
    /// pure
    virtual UNK_RET vf_02(UNK_PARAMS) = 0;
    /// 0x086ee5ea
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x086ee5f0
    virtual UNK_RET OnIdle();
    /// 0x086ee5f6
    virtual UNK_RET OnMessage(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x086ee5fa
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x086ee5fc
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x086ee5fe
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x086ee600
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x086ee602
    virtual UNK_RET vf_10(UNK_PARAMS);
    /// 0x086ee604
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x086ee606
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x08dce69c
    UNK_RET CreateShortcut(wchar_t const* a1, wchar_t const* a2);
    /// 0x08dce6a0
    UNK_RET IsMediaCenter();
    /// 0x08dce6c4
    UNK_RET RegisterFileType(wchar_t const* a1, wchar_t const* a2, int a3);
    /// 0x08dcef7c
    UNK_RET CheckForRegisteredFileLoad();
    /// 0x08dcf0e6
    UNK_RET GetBrowser();
    /// 0x08dcf3e0
    UNK_RET GetCommonFolder(bool a1);
    /// 0x08dcf55e
    UNK_RET SetDefaultRootFolder(wchar_t const* a1);
    /// 0x08dcf736
    UNK_RET GetCommonDataFolder(bool a1);
    /// 0x08dcf8c6
    UNK_RET GetUserFolder(bool a1);
    /// 0x08dcfa12
    UNK_RET GetUserDataFolder(bool a1);
    /// 0x08dcfb5e
    UNK_RET GetExecutableFilename(char* a1);
    /// 0x08dcfba6
    UNK_RET LogEvent(char const* a1);
    /// 0x08dcfba8
    UNK_RET CreateHandle(char const* a1, char const* a2, char const* a3);
    /// 0x08dcfc5c
    UNK_RET CheckParentalControls();
    /// 0x08dcfc62
    UNK_RET CreateInstance(HWND__* a1);
    /// 0x08dcfc9e
    UNK_RET HandleMessage(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x08dcfdb6
    UNK_RET Activate(bool a1);
    /// 0x08dcfe12
    UNK_RET HandleQuitMessage();
    /// 0x08dcffb0
    UNK_RET SetDefaultWorkingFolder(bool a1);
    /// 0x100a21cc
    static UNK_DATA ms_bCheckedForMediaCenter[0x1];
    /// 0x100a21cd
    static UNK_DATA ms_bCheckedForRegisteredFileLoad[0x1];
    /// 0x100a21ce
    static UNK_DATA ms_bLaunchedFromMediaCenter[0x1];
    /// 0x100a21ec
    static UNK_DATA ms_pkApplication[0x4];
    /// 0x100a21d0
    static UNK_DATA ms_strBrowser[0x4];
    /// 0x100a21d8
    static UNK_DATA ms_strCommonDataFolder[0x4];
    /// 0x100a21d4
    static UNK_DATA ms_strCommonFolder[0x4];
    /// 0x100a21dc
    static UNK_DATA ms_strDefaultRootFolder[0x4];
    /// 0x100a21e0
    static UNK_DATA ms_strRegisteredFileToLoad[0x4];
    /// 0x100a21e8
    static UNK_DATA ms_strUserDataFolder[0x4];
    /// 0x100a21e4
    static UNK_DATA ms_strUserFolder[0x4];
};

namespace FDebugHelperNS {
/// 0x100a2318
extern UNK_DATA g_ExceptionText[0x10000];
/// 0x100a2314
extern UNK_DATA g_pExceptionInfo[0x4];
}

namespace FDebugHelperNS { namespace StaticExceptionHelper {
/// 0x100b2318
extern UNK_DATA previousExceptionFilter[0x4];
}}

namespace FWaitCursor {
/// 0x100a21f0
extern UNK_DATA ms_hDefaultWaitCursor[0x4];
}

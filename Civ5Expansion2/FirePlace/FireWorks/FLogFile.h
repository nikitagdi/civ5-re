/// origin binary
#pragma once
#include "FirePlace/FireWorks/FILogFile.h"

/// size ?
/// vtable 0x0a305980
class FLogFile : public FILogFile {
public:
    /// 0x08d8def6
    /// deleting 0x08d8e040
    virtual ~FLogFile();
    /// 0x08d8cbb0
    virtual void Archive(wchar_t const* a1);
    /// 0x08d8cb3a
    virtual void Close();
    /// 0x08d8d166
    virtual void DebugMsg(char const* a1, ...);
    /// 0x08d8d26e
    virtual void ErrorMsg(char const* a1, ...);
    /// 0x08d8deee
    virtual LPCWSTR GetFileName() const;
    /// 0x08d8dee6
    virtual uint GetFlags() const;
    /// 0x08d8d0fc
    virtual void Msg(char const* a1, ...);
    /// 0x08d8ca3a
    virtual void Open();
    /// 0x08d8d2f2
    virtual void SetBufSize(int a1);
    /// 0x08d8deda
    virtual void SetFlags(unsigned int a1);
    /// 0x08d8d1ea
    virtual void WarningMsg(char const* a1, ...);
    /// 0x08d8cc48
    UNK_RET OutputMsg() const;
    /// 0x08d8d072
    UNK_RET MsgA(char const* a1);
};

/// 0x08d8e260
UNK_RET FXmlAllocator(unsigned int a1);
/// 0x08d8e274
UNK_RET FXmlFree(void* a1);
extern "C" {
    /// 0x08d8d31e
    UNK_RET GetLogFileMgr(UNK_PARAMS);
    /// 0x08d8d332
    UNK_RET PeekLogFileMgr(UNK_PARAMS);
}

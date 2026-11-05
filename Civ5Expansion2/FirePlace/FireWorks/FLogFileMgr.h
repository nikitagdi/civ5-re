/// origin guess
#pragma once
#include "FirePlace/FireWorks/FILogFile.h"

/// size ?
/// vtable 0x0a305930
class FLogFileMgr : public FILogFileMgr {
public:
    /// 0x08d8db3c
    /// deleting 0x08d8dad4
    virtual ~FLogFileMgr();
    /// 0x08d8d89a
    virtual bool DeleteLog(wchar_t const* a1);
    /// 0x08d8d9de
    virtual bool DeleteLog(FILogFile* a1);
    /// 0x08d8d392
    virtual void DisableLogging();
    /// 0x08d8d348
    virtual void EnableLogging();
    /// 0x08d8d3dc
    virtual FILogFile * GetLog(char const* a1, unsigned int a2, char const* a3);
    /// 0x08d8d6b6
    virtual FILogFile * GetLog(wchar_t const* a1, unsigned int a2, char const* a3);
    /// 0x08d8dec4
    virtual bool IsLoggingEnabled();
    /// 0x08d8c9b8
    UNK_RET Uninit();
    /// 0x08d8caea
    UNK_RET GetInstance();
    /// 0x08d8d666
    UNK_RET FindLogFile(wchar_t const* a1);
    /// 0x0a312ed4
    static UNK_DATA ms_bEnabled[0x1];
    /// 0x0ff8b82c
    static UNK_DATA ms_strLogFolder[0x4];
};

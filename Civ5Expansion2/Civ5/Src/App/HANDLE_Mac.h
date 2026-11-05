/// origin guess
#pragma once

/// size ?
/// vtable 0x0a2f47b0
class HANDLE_Mac {
public:
    /// 0x0856f9ee
    virtual UNK_RET WaitForSingleObject(unsigned long a1);
    /// 0x0856f9f2
    virtual UNK_RET WaitForSingleObjectEx(unsigned long a1);
    /// 0x0856f9f6
    virtual UNK_RET Signal();
    /// 0x0856f99c
    virtual UNK_RET CloseHandle();
    /// 0x0856f9e0
    /// deleting 0x0856f9cc
    virtual ~HANDLE_Mac();
};

/// 0x0865b435
UNK_RET szStringF(char const* a1, ...);

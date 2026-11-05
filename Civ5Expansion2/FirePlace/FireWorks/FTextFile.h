/// origin binary
#pragma once
#include "FirePlace/FireWorks/FFile.h"
#include "FirePlace/FireWorks/FIFile.h"
#include "FirePlace/FireWorks/FMemoryStream.h"
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a2fc800
class FTextFile : public FFile {
public:
    enum EEncodingType : int;
    // Opens pszFile through the Open virtual when it is not empty; the binary has one hidden out-of-line copy at 0x08791c7a.
    /// inline
    FTextFile(char const* pszFile, FIFile::OpenMode eMode, unsigned long ulFlags, unsigned int a4);
    /// 0x08791bb0
    /// deleting 0x08791b88
    virtual ~FTextFile();
    /// 0x08791e7e
    virtual FileErr Close();
    /// 0x08791ff4
    virtual uint GetLength() const;
    /// 0x08792066
    virtual bool IsOpen() const;
    /// 0x08daa6c8
    virtual FileErr Open(char const* a1, FIFile::OpenMode a2, unsigned long a3, unsigned int a4);
    /// 0x08daa7ec
    virtual FileErr Open(wchar_t const* a1, FIFile::OpenMode a2, unsigned long a3, unsigned int a4);
    /// 0x08daafd8
    virtual uint Read(void* a1, unsigned int a2);
    /// 0x08daaafc
    virtual uint Seek(long a1, FIFile::SeekMode a2);
    /// 0x08dab4b4
    virtual uint Write(void const* a1, unsigned int a2);
    /// 0x08daa88a
    virtual UNK_RET OpenFromStream(FMemoryStream* a1, FIFile::OpenMode a2, unsigned long a3);
    /// 0x08daa4d0
    UNK_RET DetermineEncoding();
    /// 0x08daa62c
    UNK_RET SetEncoding(FTextFile::EEncodingType a1);
    /// 0x08daa766
    UNK_RET Init(bool a1, FTextFile::EEncodingType a2);
    /// 0x08daa8f2
    UNK_RET FillBuffer();
    /// 0x08daaa24
    UNK_RET GoToLine(unsigned int a1);
    /// 0x08daac0e
    UNK_RET Seek(char const* a1, int a2);
    // Nonzero when a line was read.
    /// 0x08daad54
    int ReadString(FStringA& a1);
    /// 0x08daae26
    UNK_RET ReadStringUnicode(FStringW& a1);
    /// 0x08daae94
    UNK_RET ReadStringAnsi(FStringA& a1);
    /// 0x08daaef2
    UNK_RET ReadString(FStringW& a1);
    /// 0x08dab08a
    UNK_RET WriteString(FStringA const& a1);
    /// 0x08dab15c
    UNK_RET WriteStringUnicode(FStringW const& a1);
    /// 0x08dab1ca
    UNK_RET WriteStringAnsi(FStringA const& a1);
    /// 0x08dab228
    UNK_RET WriteString(FStringW const& a1);
    /// 0x08dab30c
    UNK_RET WriteString(char const* a1, ...);
    /// 0x08dab3da
    UNK_RET WriteF(char const* a1, ...);
    /// 0x08dab566
    UNK_RET GetChar(char& a1);
    /// 0x08dab712
    UNK_RET GetCharUnicode(wchar_t& a1);
    /// 0x08dab78c
    UNK_RET GetCharAnsi(char& a1);
    /// 0x08dab7f2
    UNK_RET GetChar(wchar_t& a1);
    /// 0x08dab8f4
    UNK_RET UnGetChar(char a1);
    /// 0x08dab9ea
    UNK_RET UnGetCharUnicode(wchar_t a1);
    /// 0x08daba50
    UNK_RET UnGetCharAnsi(char a1);
    /// 0x08dabab6
    UNK_RET UnGetChar(wchar_t a1);
    /// 0x08dabc4c
    UNK_RET PutChar(char a1);
    /// 0x08dabd50
    UNK_RET PutCharUnicode(wchar_t a1);
    /// 0x08dabdbc
    UNK_RET PutCharAnsi(char a1);
    /// 0x08dabe1c
    UNK_RET PutChar(wchar_t a1);
};

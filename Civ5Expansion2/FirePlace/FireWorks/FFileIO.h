/// origin binary
#pragma once
#include "FirePlace/FireWorks/FIFile.h"

class FFileIO;
class FIOBufferAsync;

/// size ?
/// vtable 0x0a2f8e50
class FFileIO : public FIFile {
public:
    class Allocator;
    /// size ?
    /// vtable 0x0a305b58
    class Allocator : public FIFile::Allocator {
    public:
        /// 0x08da0124
        virtual byte * Allocate(unsigned int a1);
        /// 0x08da01a2
        virtual void Delete(unsigned char* a1);
        /// 0x0ff8fc50
        static UNK_DATA m_GlobalBuffer[0x100000];
        /// 0x0a312ee4
        static UNK_DATA m_gpGlobalBuffer[0x4];
    };
    class FOverlappedContext;
    /// 0x08da0214
    FFileIO(char const* a1, FIFile::OpenMode a2, unsigned long a3, unsigned int a4);
    /// 0x08da02e8
    FFileIO(wchar_t const* a1, FIFile::OpenMode a2, unsigned long a3, unsigned int a4);
    /// 0x086690e0
    /// deleting 0x086690b8
    virtual ~FFileIO();
    /// 0x08da03bc
    virtual FileErr Close();
    /// 0x08da11be
    virtual uint Flush();
    /// 0x08669f76
    virtual const wchar_t * GetFileName();
    /// 0x08669f8a
    virtual uint GetLength() const;
    /// 0x0866a00e
    virtual uint GetPosition();
    /// 0x08669f7e
    virtual byte * GetViewCaret() const;
    /// 0x08669f82
    virtual byte * GetViewEnd() const;
    /// 0x08669f86
    virtual byte * GetViewPtr() const;
    /// 0x0866a04c
    virtual bool IsEOF();
    /// 0x08669fa6
    virtual bool IsMapped() const;
    /// 0x08669f96
    virtual bool IsOpen() const;
    /// 0x08da044a
    virtual FileErr Open(char const* a1, FIFile::OpenMode a2, unsigned long a3, unsigned int a4);
    /// 0x08da09da
    virtual FileErr Open(wchar_t const* a1, FIFile::OpenMode a2, unsigned long a3, unsigned int a4);
    /// 0x08da0de8
    virtual uint Read(void* a1, unsigned int a2);
    /// 0x0866a040
    virtual void Rewind();
    /// 0x08da109c
    virtual uint Seek(long a1, FIFile::SeekMode a2);
    /// 0x08669faa
    virtual uint SeekToBegin();
    /// 0x08669fdc
    virtual uint SeekToEnd();
    /// 0x08da1392
    virtual void SetLength(unsigned int a1);
    /// 0x08da0f36
    virtual uint Write(void const* a1, unsigned int a2);
    /// 0x08da041a
    UNK_RET RemoveBuffer();
    /// 0x08da08d4
    UNK_RET DiskOpen(unsigned long a1, bool a2, bool a3);
    /// 0x08da0e46
    UNK_RET BufferRead(void* a1, unsigned int a2);
    /// 0x08da0e7c
    UNK_RET DiskRead(void* a1, unsigned int a2);
    /// 0x08da0f94
    UNK_RET BufferWrite(void const* a1, unsigned int a2);
    /// 0x08da0fca
    UNK_RET DiskWrite(void const* a1, unsigned int a2);
    /// 0x08da110a
    UNK_RET BufferSeek(long a1, FIFile::SeekMode a2);
    /// 0x08da1140
    UNK_RET DiskSeek(long a1, FIFile::SeekMode a2);
    /// 0x08da1192
    UNK_RET StreamBuffer(FFileIO& a1);
    /// 0x08da11f4
    UNK_RET GetLastWriteTime() const;
    /// 0x08da1242
    UNK_RET GetLastWriteTime(char const* a1);
    /// 0x08da1314
    UNK_RET StartAsyncRead(void* a1, unsigned int a2, FFileIO::FOverlappedContext& a3);
    /// 0x08da131c
    UNK_RET FinishAsyncRead(FFileIO::FOverlappedContext& a1);
    /// 0x08da1320
    UNK_RET AsyncCompleteNotification(bool a1, unsigned int a2, FIOBufferAsync* a3);
    /// 0x08da1408
    UNK_RET Exist(char const* a1);
    /// 0x08da15f6
    UNK_RET Exist(wchar_t const* a1);
    /// 0x08da1702
    UNK_RET GetLength(char const* a1);
    /// 0x08da1810
    UNK_RET GetLength(wchar_t const* a1);
    /// 0x08da192e
    UNK_RET IsNaked(char const* a1);
    /// 0x08da197a
    UNK_RET IsNaked(wchar_t const* a1);
    /// 0x08da19c6
    UNK_RET InitFileSystem();
    /// 0x08da19e6
    UNK_RET UninitFileSystem();
    /// 0x08da19e8
    UNK_RET Delete(char const* a1);
    /// 0x08da1c22
    UNK_RET Delete(wchar_t const* a1);
    /// 0x08da1d7e
    UNK_RET DeleteAll(char const* a1);
    /// 0x08da2270
    UNK_RET DeleteAll(wchar_t const* a1);
    /// 0x08da2716
    UNK_RET Rename(char const* a1, char const* a2);
    /// 0x08da2904
    UNK_RET Rename(wchar_t const* a1, wchar_t const* a2);
    /// 0x08da2b22
    UNK_RET Copy(char const* a1, char const* a2, bool a3);
    /// 0x08da2d1e
    UNK_RET Copy(wchar_t const* a1, wchar_t const* a2, bool a3);
    /// 0x08da2f4c
    UNK_RET ReadAll(char const* a1, unsigned long a2, FFileIO::Allocator& a3);
    /// 0x08da303c
    UNK_RET ReadAll(wchar_t const* a1, unsigned long a2, FFileIO::Allocator& a3);
    /// 0x08da312c
    UNK_RET ExternalFileSystemErrorNotification(char const* a1);
    /// 0x08da316e
    UNK_RET DownloadFile(char const* a1, char const* a2);
    /// 0x08da34a2
    UNK_RET DownloadFile(wchar_t const* a1, wchar_t const* a2);
    /// 0x0ff8fc40
    static UNK_DATA ms_pkErrorHandler[0x4];
};

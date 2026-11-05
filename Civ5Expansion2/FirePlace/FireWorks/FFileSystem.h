/// origin binary

#ifndef _FFILESYSTEM_H_
#define _FFILESYSTEM_H_

#include "FirePlace/FireWorks/FIFile.h"

// Implementation class for the file system.  This should only be accessed directly on the application side, DLLs should use FIFileSystem::GetInstance()
/// size 0x4
/// vtable 0x0a305b80
class FFileSystem : public FIFileSystem
{
public:
	/// 0x08da65e2
	static FFileSystem&	GetInstance();

	/// 0x08da661c
	virtual FIFile*		Create( const  char*  szFileName = NULL, FIFile::OpenMode eOpenMode = FIFile::modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE );
	/// 0x08da66e0
	virtual FIFile*		Create( const wchar* wszFileName       , FIFile::OpenMode eOpenMode = FIFile::modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE );
	
	/// 0x08da67a4
	virtual FileErr		Delete( const  char*  szFileName );
	/// 0x08da67c6
	virtual FileErr		Delete( const wchar* wszFileName );

	/// 0x08da67e8
	virtual FileErr		ReadAll( const  char*  szFileName, byte** pucBufferOut, size_t *puiBufferSizeOut, dword dwFlags = FFILE_FLAG_DEFAULT);
	/// 0x08da697c
	virtual FileErr		ReadAll( const  wchar*  wszFileName, byte** pucBufferOut, size_t *puiBufferSizeOut, dword dwFlags = FFILE_FLAG_DEFAULT);

	/// 0x08da6b10
	virtual FileErr		ReadAll( const  char*  szFileName, byte** pucBufferOut, size_t *puiBufferSizeOut, dword dwFlags, FIFile::Allocator& alloc);
	/// 0x08da6cbe
	virtual FileErr		ReadAll( const  wchar*  wszFileName, byte** pucBufferOut, size_t *puiBufferSizeOut, dword dwFlags, FIFile::Allocator& alloc);

	/// 0x08da6e6c
	virtual bool		Exist( const char* pszFileName );
	/// 0x08da6e90
	virtual bool		Exist( const wchar* pwszFileName );

	/// 0x08da6eb4
	virtual uint		GetLength( const  char*  szFileName );
	/// 0x08da6ed6
	virtual uint		GetLength( const wchar* wszFileName );

	/// 0x08da6ef8
	virtual FFileTimestamp GetLastWriteTime( const char* pFileName );

	/// 0x08da6f24
	virtual	FFileErrorHandler* GetErrorHandler();

	/// 0x08da6f3a
	virtual bool		IsFileCatalogEnabled();
};

#endif // _FFILESYSTEM_H_

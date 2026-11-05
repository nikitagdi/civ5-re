/// origin binary
#ifndef _FIFILE_H_
#define _FIFILE_H_
#pragma once

#include "FirePlace/FireWorks/FFileDefines.h"

#ifdef WIN32

//====================================================================================================
// \brief Simple utility class for comparing Win32 file times
//====================================================================================================
/// size 0x8
class FFileTimestamp
{
public:

	enum
	{
		LESS = -1,
		EQUAL = 0,
		GREATER = 1
	};

	/// inline
	inline FFileTimestamp() {};
	/// inline
	inline FFileTimestamp( const FILETIME& t ) : m_time(t) {};

	/// inline
	inline bool operator<( const FFileTimestamp& a ) const  { return Compare( a ) == LESS;    };
	/// inline
	inline bool operator<=( const FFileTimestamp& a ) const { return Compare( a ) != GREATER; };
	/// inline
	inline bool operator>( const FFileTimestamp& a ) const  { return Compare( a ) == GREATER; };
	/// inline
	inline bool operator>=( const FFileTimestamp& a ) const { return Compare( a ) != LESS;    };
	/// inline
	inline bool operator==( const FFileTimestamp& a ) const { return Compare( a ) == EQUAL;   };
	/// inline
	inline bool operator!=( const FFileTimestamp& a ) const { return Compare( a ) != EQUAL;   };
	/// inline
	inline long Compare( const FFileTimestamp& a ) const { return CompareFileTime( &m_time, &a.m_time ); };

private:
	FILETIME m_time; ///< +0x0
};

#else

typedef unsigned int FFileTimestamp; // stopgap solution for non-windows platforms...

#endif

class FFileErrorHandler;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  CLASS:  FIFile
//
//  DESC:   Interface class for basic file functions
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size 0x4
/// vtable ?
class FIFile
{
public:
	enum SeekMode { begin=0, current=1, end=2 };
	enum OpenMode { modeRead=1, modeWrite=2, modeEdit=3 };

	/// 0x08da86f0
	/// deleting 0x08da86dc
	virtual ~FIFile() = 0;

	/// pure
	virtual FileErr Open( const  char*  szFileName, OpenMode eOpenMode = modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE ) = 0;
	/// pure
	virtual FileErr Open( const wchar* wszFileName, OpenMode eOpenMode = modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE ) = 0;
	/// pure
	virtual FileErr Close( void ) = 0;

	/// pure
	virtual const wchar_t* GetFileName() = 0;

	/// pure
	virtual byte* GetViewCaret( void ) const = 0;
	/// pure
	virtual byte* GetViewEnd( void ) const = 0;
	/// pure
	virtual byte* GetViewPtr( void ) const = 0;

	/// pure
	virtual uint GetLength( void ) const = 0;
	/// pure
	virtual void SetLength( uint size ) = 0;

	/// pure
	virtual bool IsOpen( void ) const = 0;
	/// pure
	virtual bool IsMapped( void ) const = 0;

	/// pure
	virtual uint Seek( long lOffset, SeekMode eOrigin ) = 0;

	/// pure
	virtual uint SeekToBegin() = 0;
	/// pure
	virtual uint SeekToEnd() = 0;
	/// pure
	virtual uint GetPosition() = 0;
	/// pure
	virtual void Rewind( void ) = 0;

	/// pure
	virtual bool IsEOF( void ) = 0;

	/// pure
	virtual uint Read(        void* pData, uint size ) = 0;
	/// pure
	virtual uint Write( const void* pData, uint size ) = 0;

	//--
	//  Class read/write templates, e.g. myFile.Read( varFloat );
	//    
	/// inline
	template <class T> uint Read(        T& kData )
	{
		return ( Read( &kData, sizeof( T ) ) );
	}

	/// inline
	template <class T> uint Write( const T& kData )
	{
		return ( Write( &kData, sizeof( T ) ) );
	}

	/// pure
	virtual	uint Flush( void ) = 0;

	/// size 0x4
	/// vtable ?
	class Allocator
	{
	public:

		/// pure
		virtual byte* Allocate(const uint uiNumBytes) = 0;
		/// pure
		virtual void  Delete(byte* pBuffer)           = 0;
	};

protected:

	/// inline
	FIFile() { };

	// Copy constructor protection
	/// absent
	FIFile( const FIFile& obj );

	// Assignment operator protection
	/// absent
	FIFile& operator=( const FIFile& obj );
};

/// 0x08da86f0
inline FIFile::~FIFile() { }

/*	An encapsulation of an FIFile class that allows for auto creation and destruction.
	Use this where you used to use the FFile class.  This allows the file system implementation code
	to be hidden which is helpful when using with a DLL. 
	Use the -> or * operator to dereference the internal FIFile object. */
/// size 0x4
class FFileInstance
{
	FIFile* m_pkFile; ///< +0x0
public:
	/// 0x08da86f2
	~FFileInstance();
	/// 0x08da871a
	FFileInstance( const  char*  szFileName = NULL, FIFile::OpenMode eOpenMode = FIFile::modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE );
	/// 0x08da8768
	FFileInstance( const wchar* wszFileName       , FIFile::OpenMode eOpenMode = FIFile::modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE );

	// Access the FIFile object
	/// inline
	FIFile& operator*() { return *m_pkFile; }
	/// inline
	FIFile* operator->() { return m_pkFile; }
};

/*	Interface class for file system operations, including the creation of a file access object */
/// size 0x4
/// vtable ?
class FIFileSystem
{
public:
	// Create a file access object.
	/// pure
	virtual FIFile*		Create( const  char*  szFileName = NULL, FIFile::OpenMode eOpenMode = FIFile::modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE ) = 0;
	/// pure
	virtual FIFile*		Create( const wchar* wszFileName       , FIFile::OpenMode eOpenMode = FIFile::modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE ) = 0;
	
	// Delete the file at the supplied path
	/// pure
	virtual FileErr		Delete( const  char*  szFileName ) = 0;
	/// pure
	virtual FileErr		Delete( const wchar* wszFileName ) = 0;

	// Read the entire file into a buffer.  It is the caller's responsibility to delete the returned buffer
	/// pure
	virtual FileErr		ReadAll( const  char*  szFileName, byte** pucBufferOut, size_t *puiBufferSizeOut, dword dwFlags = FFILE_FLAG_DEFAULT) = 0;
	/// pure
	virtual FileErr		ReadAll( const  wchar*  wszFileName, byte** pucBufferOut, size_t *puiBufferSizeOut, dword dwFlags = FFILE_FLAG_DEFAULT) = 0;

	// Read the entire file into a buffer (that was retrieved from the allocator).  It is the caller's responsibility
	// to delete the returned buffer using the allocator
	/// pure
	virtual FileErr		ReadAll( const  char*  szFileName, byte** pucBufferOut, size_t *puiBufferSizeOut, dword dwFlags, FIFile::Allocator& alloc) = 0;
	/// pure
	virtual FileErr		ReadAll( const  wchar*  wszFileName, byte** pucBufferOut, size_t *puiBufferSizeOut, dword dwFlags, FIFile::Allocator& alloc) = 0;

	// File existence
	/// pure
	virtual bool		Exist( const char* pszFileName ) = 0;
	/// pure
	virtual bool		Exist( const wchar* pwszFileName ) = 0;

	// File size
	/// pure
	virtual uint		GetLength( const  char*  szFileName ) = 0;
	/// pure
	virtual uint		GetLength( const wchar* wszFileName ) = 0;

	// Timestamp query
	/// pure
	virtual FFileTimestamp GetLastWriteTime( const char* pFileName ) = 0;

	// Returns true if the primary file catalog system is enabled.  If so, you can pass in "naked" file names 
	// to the Open/Create methods of the file system and they will be found in either PAK files or in a pre-scanned directory hierarchy.
	/// pure
	virtual bool		IsFileCatalogEnabled() = 0;

	// Get the active error handler.
	/// pure
	virtual	FFileErrorHandler* GetErrorHandler() = 0;

	/// 0x08da87ea
	static FIFileSystem& GetInstance();
	/// 0x08da87b6
	static void			SetInstance(FIFileSystem* pkInstance);

protected:
	/// 0x1008ff20
	static FIFileSystem* ms_pInstance;
};

#if defined(FXS_IS_DLL)
#define FFILESYSTEM FIFileSystem::GetInstance()
#else
// If not in a DLL, access the implementation of the file system.  Not strictly necessary but
// this will allow any old code to not crash if there haven't been any calls to initalize FFileSystem.
// Remove at a later data and always call FIFileSystem::GetInstance()
#include "FirePlace/FireWorks/FFileSystem.h"
#define FFILESYSTEM FFileSystem::GetInstance()
#endif

#endif // _FIFILE_H_

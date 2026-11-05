/// origin binary
#ifndef _FILOGFILE_H_
#define _FILOGFILE_H_

#include "FirePlace/FireWorks/FFireTypes.h"

//
// log file object
//
/// size 0x4
/// vtable ?
class FILogFile
{
public:
	enum EFlags
	{
		kDontTimeStamp		= 0x1,
		kDontFlushOnWrite	= 0x2,
		kWriteToConsole		= 0x4
	};

	/// pure
	virtual void Msg(const TCHAR* format, ... ) = 0;
	/// pure
	virtual void DebugMsg(const TCHAR* format, ... ) = 0;
	/// pure
	virtual void WarningMsg(const TCHAR* format, ... ) = 0;
	/// pure
	virtual void ErrorMsg(const TCHAR* format, ... ) = 0;
	/// pure
	virtual void SetFlags(uint uiFlags) = 0;
	/// pure
	virtual uint GetFlags() const = 0;
 	/// pure
 	virtual void SetBufSize(int iSize) = 0;
	/// pure
	virtual LPCWSTR GetFileName() const = 0;
	/// pure
	virtual void Close( void ) = 0;
	/// pure
	virtual void Open( void ) = 0;
	/// pure
	virtual void Archive(LPCWSTR wszArchiveName) = 0;

protected:
	/// inline
	FILogFile() {};
	/// 0x08d8940a
	/// deleting 0x08d893f6
	virtual ~FILogFile() = 0;
};

/// 0x08d8940a
inline FILogFile::~FILogFile() {}

//
// log file manager
//
/// size 0x4
/// vtable ?
class FILogFileMgr 
{
public:
	/// 0x08d893f4
	/// deleting 0x08d893e0
	virtual ~FILogFileMgr() = 0;
	/// 0x08d8940c
	static FILogFileMgr& GetInstance();
	/// 0x08d89422
	static FILogFileMgr* PeekInstance();

	/// pure
	virtual void		EnableLogging( void ) = 0;
	/// pure
	virtual void		DisableLogging( void ) = 0;
	/// pure
	virtual FILogFile*	GetLog(LPCWSTR wszFileName, uint uiFlags, const TCHAR* szTitleString = NULL) = 0;		// create if necessary
	/// pure
	virtual FILogFile*	GetLog(LPCSTR szFileName, uint uiFlags, const TCHAR* szTitleString = NULL) = 0;		// create if necessary

	/// pure
	virtual bool		DeleteLog(FILogFile* pLog) = 0;
	/// pure
	virtual bool		DeleteLog(LPCWSTR wszFileName) = 0;

	/// pure
	virtual bool		IsLoggingEnabled( void ) = 0;

protected:
	/// 0x0ff8b708
	static FILogFileMgr* ms_pInstance;

	/// inline
	FILogFileMgr() {};
};

/// 0x08d893f4
inline FILogFileMgr::~FILogFileMgr() {}

#ifdef FXS_IS_DLL
#define LOGFILEMGR FILogFileMgr::GetInstance()
#else
#define LOGFILEMGR FLogFileMgr::GetInstance()
#endif

#endif // _FILOGFILE_H_

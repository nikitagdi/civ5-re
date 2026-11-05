/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FFileStream.h
//
//  AUTHOR:		Mustafa Thamer	--  11/25/2003
//
//  PURPOSE:	Stream class for file streams.
//				Easily read and write data to those kinds of streams using the same baseclass interface.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2007 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#ifndef		FFILESTREAM_H
#define		FFILESTREAM_H
#pragma		once

#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FIFile.h"

//
// File stream
//
/// size 0x18
/// vtable 0x0a305780
class FFileStream: public FDataStream
{	
public:
	// Constructors:
	//-------------
	/// 0x08d7ec0a
	FFileStream();

	// Destructors:
	//------------
	/// 0x08d7ec66
	/// deleting 0x08d7ec3e
	virtual ~FFileStream();

	// Virtual:
	//--------
	/// 0x08d7e864
	virtual bool Open(const char* szName, FIFile::OpenMode eMode = FIFile::modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE);
	/// 0x08d7e974
	virtual bool Open(const wchar_t* wszName, FIFile::OpenMode eMode = FIFile::modeRead, dword dwFlags = FFILE_FLAG_DEFAULT, uint uiBufferSize = FFILE_DEFAULT_BUFFER_SIZE);

	/// 0x08d7ea3e
	virtual bool Close();
	/// 0x08d7ea90
	virtual unsigned int ReadIt(unsigned int byteCount, void* buffer);
	/// 0x08d7eaf2
	virtual unsigned int WriteIt(unsigned int byteCount, const void* buffer);
	/// 0x08d7eb3c
	virtual void Skip(unsigned int deltaByteCount);
	/// 0x08d7ecb0
	virtual bool AtEnd() const;
	/// 0x08d7eb7a
	virtual void Rewind();
	/// 0x08d7ecde
	virtual void FastFwd();
	/// 0x08d7ed0c
	virtual void Truncate();
	/// 0x08d7ed44
	virtual void Flush();
	/// 0x08d7ebb0
	virtual unsigned int GetEOF() const;
	/// 0x08d7ed6c
	virtual void RaiseException( std::exception& kException ) const;
		
	// Methods:
	//--------
	/// 0x08d7e96c
	FIFile* GetFile();
	/// 0x08d7e92e
	void SetFile(FIFile* file);
		

private:
	// Members:
	//--------
	FIFile* m_pRef; ///< +0x14
};

#endif	//	FFILESTREAM_H


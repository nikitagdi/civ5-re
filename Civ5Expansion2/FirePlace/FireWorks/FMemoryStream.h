/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FMemoryStream.h
//
//  AUTHOR:		Mustafa Thamer	--  11/25/2003
//
//  PURPOSE:	Stream class for memory streams.
//				Easily read and write data to those kinds of streams using the same baseclass interface.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2006 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#ifndef		FMEMORYSTREAM_H
#define		FMEMORYSTREAM_H
#pragma		once

#include "FirePlace/FireWorks/FDataStream.h"

//
// Memory stream
//
/// size 0x30
/// vtable 0x0a3057f0
class FMemoryStream : public FDataStream 
{
public:
	// Constructors:
	//-------------
	// JAR : todo  - refactor these constructors. FMemoryStream myBroken(char *, int) will silently compile as it converts
	// the char * and int to the 2-bool implementation. E.g. FMemoryStream myCopy(other.GetBuf(), other.GetEOF()), followed
	// by Bad Things.
	/*
	FMemoryStream(int size, const void* data, uint	uMemoryPool, uint uMemoryTag, bool bRealloc=false, bool bReallocMin=false);
	FMemoryStream(uint	uMemoryPool, uint uMemoryTag, bool bRealloc=true, bool bReallocMin=false);
	FMemoryStream(int size, const void* data, bool bRealloc=false, bool bReallocMin=false);
	FMemoryStream(bool bRealloc=true, bool bReallocMin=false);
	*/
	/// 0x08d7eda0
	FMemoryStream(int size, const void* data, uint	uMemoryPool, uint uMemoryTag, bool bRealloc, bool bReallocMin);
	/// 0x08d7ee3c
	FMemoryStream(uint	uMemoryPool, uint uMemoryTag, bool bRealloc, bool bReallocMin);
	/// 0x08d7eea8
	FMemoryStream(int size, const void* data, bool bRealloc, bool bReallocMin);
	/// 0x08d7ef44
	FMemoryStream(int size, const void* data, bool bRealloc);
	/// 0x08d7efdc
	FMemoryStream(int size, const void* data);
	/// 0x08d7f054
	FMemoryStream(bool bRealloc, bool bReallocMin);
	/// 0x08d7f0b8
	FMemoryStream(bool bRealloc);
	/// 0x08d7f1b0
	FMemoryStream();
	/// 0x08d7f11a
	FMemoryStream(const FMemoryStream &);

	// Destructors:
	//------------
	/// 0x08d7f882
	/// deleting 0x08d7f258
	virtual ~FMemoryStream( void );

	// Abstract:
	//---------
	/// 0x08d7f280
	virtual unsigned int ReadIt(unsigned int byteCount, void * buffer);
	/// 0x08d7f30e
	virtual unsigned int WriteIt(unsigned int byteCount, const void* buffer);
	/// 0x08d7f46e
	virtual void Skip(unsigned int deltaByteCount);
	/// 0x08d7f5ca
	virtual void Rewind();

	// Virtual:
	//--------
	/// 0x08d7f4e6
	virtual void Init(int size, const void* data, bool bRealloc=false);
	/// 0x08d7f54c
	virtual void Uninit();
	/// 0x08d7f582
	virtual bool AtEnd() const;
	/// 0x08d7f594
	virtual void FastFwd();
	/// 0x08d7f5e4
	virtual void Truncate();
	/// 0x08d7f600
	virtual unsigned int GetEOF() const;	// returns size
	/// 0x08d7f60c
	virtual void CopyToMem(void* mem);

	// Methods:
	//--------
	/// 0x08d7f3bc
	void  Grow( uint uiMinAdditionalSizeRequired = 0 );
	/// 0x08d7f642
	char* GetBuf();
	/// 0x08d7f4de
	const char* GetBuf() const;
	/// 0x08d7f64a
	void Init(int size, const void* data, bool bRealloc, bool bReallocMin);

	/// 0x08d7f20e
	FMemoryStream & operator=(const FMemoryStream &);

protected:
	// Members:
	//--------
	char*	m_pStart; ///< +0x14
	//!< Current read/write position
	char*	m_pData; ///< +0x18
	//!< End of the file (used contents of the memory buffer)
	char*	m_pStop; ///< +0x1c
	//!< True end of the memory buffer
	char*	m_pEndOfBuffer; ///< +0x20
	//!< Will the memory stream grow if you write too much?
	bool    m_bRealloc; ///< +0x24
	//!< Will the memory stream only allocate the minimum extra requested space (or double)?
	bool	m_bReallocMin; ///< +0x25

	uint m_uMemoryPool; ///< +0x28
	uint m_uMemoryTag; ///< +0x2c
};


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Memory Stream RLE  ==  FMemoryStream w/RLE compression
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size 0x38
/// vtable 0x0a305860
class FMemoryStreamRLE : public FMemoryStream 
{
public:
	// Constructors:
	//-------------
	/// inline
	FMemoryStreamRLE( int size, const void* data, bool bRealloc=false ) : FMemoryStream( size, data, bRealloc, false ) { Reset(); }
	/// inline
	FMemoryStreamRLE( bool bRealloc=true ) : FMemoryStream( bRealloc, false ) { Reset(); }

	// Abstract:
	//---------
	/// 0x08d7f7ee
	virtual uint ReadIt ( uint byteCount,       void* buffer );
	/// 0x08d7f692
	virtual uint WriteIt( uint byteCount, const void* buffer );

	/// inline
	virtual void Skip( uint deltaByteCount ) { char ch; while ((int)deltaByteCount-- > 0) ReadIt( 1, &ch ); }
	/// inline
	virtual void Rewind() { Reset(); FMemoryStream::Rewind(); }

protected:
	/// inline
	void Reset() { Run = 0; Last = EOF; }

	// Members:
	//--------
	// the last char in the i/o stream
	uint  Last; ///< +0x30
	// and its current run length
	uint  Run; ///< +0x34
};

#endif	//FMEMORYSTREAM_H

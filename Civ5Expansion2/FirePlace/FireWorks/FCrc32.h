/// origin binary
//	$Revision: #2 $		$Author: mbreitkreutz $ 	$DateTime: 2005/06/13 13:35:55 $
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:    FCrc32.h
//
//  AUTHOR:  David McKibbin  --  11/25/2002
//
//  PURPOSE: CRC (Cyclic Redundancy Check) routines
//
//---------------------------------------------------------------------------------------
//
//      CRCs can be generated either bit-wise or byte-wise.  Bit-wise will
//      always be slower but does not require the 1Kb CRC table to be built.
//      It is also included here for instructional purposes.
//
//      In practice the generated CRC is usually appended to the end of the
//      data block.  Then on the receiving end the CRC is calculated on the
//      data block and CRC and this result is zero if no errors were added.
//      The crc-preset can be anything as long as the generator and receiver
//      use the same value.  In the early days zero was used, but ~0 is now
//      typically used.  Further, PKzip stores the complemented CRC with the
//      file (this is the value displayed) which results in a fixed non-zero
//      magic cookie to check for on the receiving end.
//          
//              crc32 (message +  crc32(message)) == 0x00000000
//              crc32 (message + ~crc32(message)) == 0xDEBB20E3
//
//---------------------------------------------------------------------------------------
//
//      Crc32Calc() is prototyped with default "crc" as follows:
//
//          DWORD Crc32Calc( BYTE *data, DWORD size, DWORD crc = ~0 );
//
//
//      To calculate the CRC for a data block all at once:
//
//          crc = Crc32Calc( pDate, size );
//
//
//      To calculate the CRC for a data block in chunks:
//
//          crc = ~0;
//          while (!done)
//          {
//              GetChunk( pData, size );
//              crc = Crc32Calc( pData, size, crc );
//          }
//
//---------------------------------------------------------------------------------------
//
//   The generator polynomial for CRC-32 is defined as:
//
//      g(x) = x32 + x26 + x23 + x22 + x16 + x12 + x11 + x10 + x8 + x7 + x5 + x4 + x2 + x + 1
//
//      g(x) = 0b1.0000.0100.1100.0001.0001.1101.1011.0111  (bit-32 is implicit)
//
//      g(x) = 0x04C11DB7 : 0xEDB88320 (reciprocal)
//
//---------------------------------------------------------------------------------------
//
//      Greg Morse. Calculating CRCs by Bits and Bytes.
//      Byte Magazine, 115-124, September 1986.
//
//      W. David Schwaderer.  Crc Calculation.
//      PC Tech Journal.  118-132, April 1985.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2004 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------


#ifndef		FCRC_H
#define		FCRC_H
#pragma		once

#include "FirePlace/FireWorks/FFireTypes.h"

#define CRC_INIT	((dword) ~0)

enum
{
	CRC12 = 0x080D,		// CRC-12 polynomial	g(x) = X^12+X^11+X^3+X^2+X+1
	CRC14 = 0x202B,		// CRC-14 polynomial	g(x) = X^14+X^13+X^5+X^3+X^2+1 = (X+1)(20033F)
	CRC16 = 0x8005,		// CRC-16 polynomial	g(x) = X^16+X^15+X^2+1
	CCITT = 0x1021,		// CRC-16 polynomial	g(x) = X^16+X^12+X^5+1
	CRC32 = 0xEDB88320,	// CRC-32 polynomial	g(x) = X^32+X^26+X^23+X^22+X^16+X^12+X^11+X^10+X^8+X^7+X^5+X^4+X^2+X+1
};

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  CLASS:  FCRC
//
//  DESC:   Cyclic Redundancy Check
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size 0x408
class FCRC
{
  public:
//------
	/// 0x08dc001c
	FCRC( dword polynomial = CRC32 );

	/// 0x08dc00aa
	~FCRC();

    // Member functions
	/// 0x08dc00ac
	dword Calc		( const void *pBuf, int nLen, dword crc = CRC_INIT ) const;
	/// 0x08dc040a
	dword CalcBits	( const void *pBuf, int nLen, dword crc = CRC_INIT ) const;
	/// 0x08dc00e2
	dword CalcFile	( const char *pFileName ) const ;
	/// 0x08dc0276
	dword CalcFile  ( const wchar_t* pFileName) const;

//	dword Calcstr	( const char  *pString,   dword crc = CRC_INIT ) const;
//	dword Calcistr	( const char  *pString,   dword crc = CRC_INIT ) const;
//	dword Calcstr	( const wchar *pString,   dword crc = CRC_INIT ) const;
//	dword Calcistr	( const wchar *pString,   dword crc = CRC_INIT ) const;
//	dword Calcstr	( const std::string &str, dword crc = CRC_INIT ) const;
//	dword Calcistr	( const std::string &str, dword crc = CRC_INIT ) const;

  private:
//-------
    // Data members
	dword m_table[ 256 ]; ///< +0x0
	dword m_polynomial; ///< +0x400
	dword m_magic; ///< +0x404

	/// 0x08dc003e
	void InitTable( dword polynomial );

	// Copy constructor protection
	/// absent
	FCRC( const FCRC& obj );

	// Assignment operator protection
	/// absent
	FCRC & operator=( const FCRC& obj );
};

/// 0x100a0970
extern FCRC g_CRC32;

#endif	//FCRC_H

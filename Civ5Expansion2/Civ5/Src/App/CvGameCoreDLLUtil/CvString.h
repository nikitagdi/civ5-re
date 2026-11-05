/// origin binary
#pragma once

#include <Windows.h>

#ifndef CvString_h
#define CvString_h

#include <string>
#include <cstdarg>

#include "Civ5/Src/App/CvGameCoreDLLUtil/CvAssert.h"
class FDataStream;

//
// simple string classes, based on stl, but with a few helpers
//
// DON'T add any data members or virtual functions to these classes, so they stay the same size as their stl counterparts
//
// Mustafa Thamer
// Firaxis Games, copyright 2005
//
/// size 0xc
class CvString : public std::string
{
public:
	/// inline
	CvString() {}
	/// inline
	CvString(int iLen) { reserve(iLen); }
	/// inline
	CvString(const char* s) : std::string(s ? s : "") {CvAssertMsg(s != NULL, "Passing NULL to std::string; possible heap corruption!");}
	/// inline
	CvString(const std::string& s): std::string(s) {}

	/// inline
	~CvString() {}

	// implicit conversion
	/// inline
	operator const char*() const 	{ return c_str(); }												

	// operators
	/// inline
	char& operator[](int i) { return std::string::operator[](i);	}
	/// inline
	char& operator[](std::string::size_type i) { return std::string::operator[](i);	}
	/// inline
	const char operator[](int i) const { return std::string::operator[](i);	}
	/// inline
	CvString& operator=( const char* s) { if (s) assign(s); else clear();	return *this; }	
	/// inline
	CvString& operator=( const std::string& s) { assign(s.c_str());	return *this; }	

	// FString compatibility
	/// inline
	bool IsEmpty() const { return (empty() || c_str()[0] == '\0');}
	/// inline
	const char* GetCString() const 	{ return c_str(); }
	/// inline
	int CompareNoCase( const char* lpsz ) const { return _stricmp(lpsz, c_str()); }
	/// inline
	int CompareNoCase( const char* lpsz, int iLength ) const { return _strnicmp(lpsz, c_str(), iLength);  }
	/// inline
	void Format( const char* lpszFormat, ... );
	/// inline
	int GetLength() const { return (int) size(); }
	/// inline
	int Replace( char chOld, char chNew );

	// static helpers
	/// inline
	static bool formatv(std::string& out, const char * fmt, va_list args);
	/// inline
	static bool format(std::string & out, const char * fmt, ...);
	/// inline
	static CvString format(const char * fmt, ...);
	/// inline
	static std::string formatv(const char * fmt, va_list args);
};

/// size 0xc
class CvStringBuffer
{
public:
	/// inline
	CvStringBuffer()
	{
		m_pBuffer = NULL;
		m_iLength = 0;
		m_iCapacity = 0;
	}

	/// inline
	~CvStringBuffer()
	{
		delete [] m_pBuffer;
		m_pBuffer = NULL;
	}

	/// inline
	void append(char character)
	{
		int newLength = m_iLength + 1;
		ensureCapacity(newLength + 1);
		m_pBuffer[m_iLength] = character;
		m_pBuffer[m_iLength + 1] = 0; //null character
		m_iLength = newLength;
	}

	/// inline
	void append(const char* szCharacters)
	{
		if(szCharacters == NULL)
			return;

		int inputLength = (int) strlen(szCharacters);
		int newLength = m_iLength + inputLength;
		ensureCapacity(newLength + 1);

		//append data
		memcpy(m_pBuffer + m_iLength, szCharacters, sizeof(char) * (inputLength + 1)); //null character
		m_iLength = newLength;
	}

	/// inline
	void append(const CvString &szString)
	{
		append(szString.GetCString());
	}

	/// inline
	void append(const CvStringBuffer &szStringBuffer)
	{
		append(szStringBuffer.m_pBuffer);
	}

	/// inline
	void assign(const CvString &szString)
	{
		assign(szString.c_str());
	}

	/// inline
	void assign(const char* szCharacters)
	{
		clear();
		append(szCharacters);
	}

	/// inline
	void clear()
	{
		if(m_pBuffer != NULL)
		{
			m_iLength = 0;
			m_pBuffer[0] = 0; //null character
		}
	}

	/// inline
	bool isEmpty() const
	{
		if(m_iLength == 0)
			return true;
		else
			return false;
	}

	/// inline
	const char *getCString()
	{
		ensureCapacity(1);
		return m_pBuffer;
	}

private:
	/// inline
	void ensureCapacity(int newCapacity)
	{
		if(newCapacity > m_iCapacity)
		{
			m_iCapacity = 2 * newCapacity; //grow by %100
			char *newBuffer = new char[m_iCapacity];

			//copy data
			if(m_pBuffer != NULL)
			{
				memcpy(newBuffer, m_pBuffer, sizeof(char) * (m_iLength + 1)); //null character
				//erase old memory
				delete [] m_pBuffer;
			}
			else
			{
				newBuffer[0] = 0; //null character
			}

			m_pBuffer = newBuffer;
		}
	}

	char *m_pBuffer; ///< +0x0
	int m_iLength; ///< +0x4
	int m_iCapacity; ///< +0x8
};

//
// INLINES
// Don't move these into a cpp file, since I don't want CvString to be part of the DLL, MT
//

/// inline
inline int CvString::Replace( char chOld, char chNew )
{
	int i, iCnt = 0;
	for(i=0;i<(int)size();i++)
	{
		if ((*this)[i] == chOld)
		{
			replace(i, 1, std::string(1, chNew) );
			iCnt++;
		}
	}
	return iCnt;
}

//
// static
//
/// inline
inline bool CvString::formatv(std::string & out, const char * fmt, va_list args)
{
	char buf[2048];
	char * pbuf = buf;
	int len = 0;
	int attempts = 0;
	bool success = false;
	const int kMaxAttempts = 40;

	do
	{
		int maxlen = 2047+2048*attempts;
		len = _vsnprintf(pbuf,maxlen,fmt,args);
		attempts++;
		success = (len>=0 && len<=maxlen);
		if (!success)
		{
			if (pbuf!=buf)
				delete [] pbuf;
			pbuf = new char[2048+2048*attempts];
		}
	}
	while (!success && attempts<kMaxAttempts);

	if ( attempts==kMaxAttempts )
	{
		// dxPrintNL( "CvString::formatv - Max reallocs occurred while formatting string. Result is likely truncated!", 0 );
	}

	if (success)
		out = pbuf;
	else
		out = "";

	if (pbuf!=buf)
		delete [] pbuf;

	return success;
}

//
// static
//
/// inline
inline std::string CvString::formatv(const char * fmt, va_list args)
{
	std::string result;
	formatv( result, fmt, args );
	return result;
}
//
// static
//
/// inline
inline CvString CvString::format(const char * fmt, ...)
{
	std::string result;
	va_list args;
	va_start(args,fmt);
	formatv(result,fmt,args);
	va_end(args);
	return CvString(result);
}

//
// static
//
/// inline
inline bool CvString::format(std::string & out, const char * fmt, ...)
{
	va_list args;
	va_start(args,fmt);
	bool r = formatv(out,fmt,args);
	va_end(args);
	return r;
}


//
//
//
/// inline
inline void CvString::Format( const char* lpszFormat, ... )
{
	std::string result;
	va_list args;
	va_start(args,lpszFormat);
	formatv(result,lpszFormat,args);
	va_end(args);
	*this = result;
}

#endif	// CvString_h


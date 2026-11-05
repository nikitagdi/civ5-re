/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FDataStream.h
//
//  AUTHOR:		Mustafa Thamer	--  11/25/2003
//
//  PURPOSE:	Basic stream class.
//				Easily read and write data to those kinds of streams using the same baseclass interface.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2006 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#ifndef		FDATASTREAM_H
#define		FDATASTREAM_H
#pragma		once

#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FArray.h"
#include "FirePlace/FireWorks/FEndian.h"

class YouMustImplementASerializeOperatorForThisTypeError;
class FStringA;
class FStringW;

//
// Stream base class
//
/// size 0x14
/// vtable 0x0a305710
class FDataStream
{
public:
	// Types:
	//------
	//typedef void (*CallbackFxnType)();	// register an optional CB fxn.  It will be called every time a read/write occurs

	// Constructors:
	//-------------
	/// 0x08d7ba70
	FDataStream(void);

	// Destructors:
	//------------
	/// 0x08d7baba
	/// deleting 0x08d7baa6
	virtual	~FDataStream(void);

	// Operators:
	//----------
	/// inline
	template <typename T>
	FDataStream& operator<<(const T& kData);
	/// inline
	template <typename T>
	FDataStream& operator>>(T& kData);
	// Abstract:
	//---------
	/// pure
	virtual unsigned int ReadIt(unsigned int byteCount, void* buffer) = 0;
	/// pure
	virtual unsigned int WriteIt(unsigned int byteCount, const void* buffer) = 0;
	/// pure
	virtual void Skip(unsigned int deltaByteCount) = 0;
	/// pure
	virtual void Rewind() = 0;

	// Virtual:
	//--------
	/// pure
	virtual bool AtEnd() const = 0;
	/// pure
	virtual void FastFwd() = 0;
	/// 0x08d7babc
	virtual unsigned int GetBytesUsed() const;
	/// 0x08d7bac4
	virtual unsigned int GetPosition() const;
	/// 0x08d7bacc
	virtual void SetPosition(unsigned int position);
	/// 0x08d7baf4
	virtual void Truncate();
	/// 0x08d7baf6
	virtual void Flush();
	/// 0x08d7baf8
	virtual unsigned int GetEOF() const;
	/// 0x08d7bb42
	virtual unsigned int GetSizeLeft() const;
	/// 0x08d7bafc
	virtual void CopyToMem(void* mem);
	/// 0x08d7bafe
	virtual void RaiseException( std::exception& kException ) const;

	// Methods:
	//--------

	// Endianness
	/// inline
	void SetDesiredEndianNess(FEndian::EEndianness e) { m_eEndianness = e; }
	/// inline
	FEndian::EEndianness GetDesiredEndianNess() const { return m_eEndianness; }

	// Deprecate these
	/// 0x08d7c020
	unsigned int WriteString(const char* szName);
	/// 0x08d7c118
	unsigned int WriteString(const wchar* szName);
	/// 0x08d7c17a
	char* ReadString();			// allocates and returns memory , be sure to delete it, or there will be a leak
	/// 0x08d7c2a0
	wchar* ReadWideString();		// allocates and returns memory, be sure to delete it or there will be a leak
	/// 0x08d7c378
	unsigned int ReadString(char* szName); // reads characters from stream, can't check szName for size, may overrun memory
	/// 0x08d7c438
	unsigned int ReadString(wchar* szName); // reads characters from stream, can't check szName for size, may overrun memory

protected:
	// only access through shift operators << and >>
	// Writing
	/// 0x08d7bc52
	unsigned int WriteString(const FStringA& szName);
	/// 0x08d7bce6
	unsigned int WriteString(int count, const FStringA values[]);
	/// 0x08d7bd3e
	unsigned int WriteString(const FStringW& szName);
	/// 0x08d7bdd6
	unsigned int WriteString(int count, const FStringW values[]);
	/// 0x08d7be2e
	unsigned int WriteString(const std::string& szName);
	/// 0x08d7bed6
	unsigned int WriteString(int count, const std::string values[]);
	/// 0x08d7bf2e
	unsigned int WriteString(const std::wstring& szName);
	/// 0x08d7bfdc
	unsigned int WriteString(int count, const std::wstring values[]);
	/// absent
	void Write(const YouMustImplementASerializeOperatorForThisTypeError & szName);
	/// 0x08d7bc3e
	void Write(const FStringA& szName);
	/// 0x08d7bca8
	void Write(int count, const FStringA values[]);
	/// 0x08d7bd2a
	void Write(const FStringW& szName);
	/// 0x08d7bd98
	void Write(int count, const FStringW values[]);
	/// 0x08d7be1a
	void Write(const std::string& szName);
	/// 0x08d7be98
	void Write(int count, const std::string values[]);
	/// 0x08d7bf1a
	void Write(const std::wstring& szName);
	/// 0x08d7bf9e
	void Write(int count, const std::wstring values[]);

	/*template <typename T>
	void Write(const T& value);*/
	/// inline
	template <typename T>
	void Write(int count, const T values[]);

	/// 0x08d7bb76
	void Write(const GUID &); // JAR : todo  -- GUID isn't portable and should probably be handled elsewhere
	/// 0x08d7bba8
	void Write(const bool &);
	/// 0x08d7bbda
	void Write(const unsigned char &);
	/// 0x08d7bc0c
	void Write(const char &);
	/// 0x08d7c086
	void Write(const INT32 &);
	/// 0x08d7cd5a
	void Write(const UINT32 &);
	/// 0x08d7cdec
	void Write(const UINT64 &);
	/// 0x08d7ce4a
	void Write(const INT64 &);
#ifndef		_NDS
	// On DS, UINT32 is a define for u32 which is a define for "unsigned long"
	/// 0x08d7cefe
	void Write(const unsigned long &);
#endif	//	_NDS
	/// 0x08d7cf90
	void Write(const INT16 &);
	/// 0x08d7d012
	void Write(const UINT16 &);
	/// 0x08d7d094
	void Write(const WCHAR &);
	/// 0x08d7d114
	void Write(const float &);
	/// 0x08d7d1a2
	void Write(const double &);

	/// 0x08d7d262
	void Write(int, const INT32 values[]);
	/// 0x08d7d2a0
	void Write(int, const UINT32 values[]);
	/// 0x08d7d31c
	void Write(int, const INT64 values[]);
	/// 0x08d7d2de
	void Write(int, const UINT64 values[]);
#ifndef		_NDS
	// On DS, UINT32 is a define for u32 which is a define for "unsigned long"
	/// 0x08d7d35a
	void Write(int, const unsigned long values[]);
#endif	//	_NDS
	/// 0x08d7d398
	void Write(int, const INT16 values[]);
	/// 0x08d7d3d6
	void Write(int, const UINT16 values[]);
	/// 0x08d7d414
	void Write(int, const WCHAR values[]);
	/// 0x08d7d452
	void Write(int, const float values[]);
	/// 0x08d7d490
	void Write(int, const double values[]);
	/// absent
	void Write(int, const GUID values[]);

		// Reading
	/// 0x08d7c502
	unsigned int ReadString(FStringA& szName);
	/// 0x08d7c61e
	unsigned int ReadString(int count, FStringA values[]);
	/// 0x08d7c662
	unsigned int ReadString(FStringW& szName);
	/// 0x08d7c790
	unsigned int ReadString(int count, FStringW values[]);
	/// 0x08d7c7d4
	unsigned int ReadString(std::string& szName);
	/// 0x08d7c8d8
	unsigned int ReadString(int count, std::string values[]);
	/// 0x08d7c91c
	unsigned int ReadString(std::wstring& szName);
	/// 0x08d7ca34
	unsigned int ReadString(int count, std::wstring values[]);
	
	/// absent
	void Read(YouMustImplementASerializeOperatorForThisTypeError & szName);
	/// 0x08d7cb40
	void Read(FStringA& szName);
	/// 0x08d7cb54
	void Read(int count, FStringA values[]);
	/// 0x08d7cb92
	void Read(FStringW& szName);
	/// 0x08d7cba6
	void Read(int count, FStringW values[]);
	/// 0x08d7cbe4
	void Read(std::string& szName);
	/// 0x08d7cbf8
	void Read(int count, std::string values[]);
	/// 0x08d7cc36
	void Read(std::wstring& szName);
	/// 0x08d7cc4a
	void Read(int count, std::wstring values[]);
	/*template <class T>
	void Read(T& value); */
	
	/// inline
	template <class T>
	void Read(int count, T values[]);
	
	/// 0x08d7ca78
	void Read(GUID &);
	/// 0x08d7caaa
	void Read(bool &);
	/// 0x08d7cadc
	void Read(unsigned char &);
	/// 0x08d7cb0e
	void Read(char &);
	// specializations for plain old data types
	/// 0x08d7c248
	void Read(INT32 &);
	/// 0x08d7d4ce
	void Read(UINT32 &);
	/// 0x08d7d526
	void Read(UINT64 &);
	/// 0x08d7d55e
	void Read(INT64 &);
#ifndef		_NDS
	// On DS, UINT32 is a define for u32 which is a define for "unsigned long"
	/// 0x08d7d5ce
	void Read(unsigned long &);
#endif	//	_NDS
	/// 0x08d7d626
	void Read(INT16 &);
	/// 0x08d7d672
	void Read(UINT16 &);
	/// 0x08d7d6be
	void Read(WCHAR &);
	/// 0x08d7d70a
	void Read(float &);
	/// 0x08d7d762
	void Read(double &);

	/// absent
	void Read(int, GUID values[]); 
	/// 0x08d7d7d2
	void Read(int, INT32 values[]);
	/// 0x08d7d810
	void Read(int, UINT32 values[]);
	/// 0x08d7d89a
	void Read(int, INT64 values[]);
	/// 0x08d7d84e
	void Read(int, UINT64 values[]);
#ifndef		_NDS
	// On DS, UINT32 is a define for u32 which is a define for "unsigned long"
	/// 0x08d7d8d8
	void Read(int, unsigned long values[]);
#endif	//	_NDS
	/// 0x08d7d916
	void Read(int, INT16 values[]);
	/// 0x08d7d954
	void Read(int, UINT16 values[]);
	/// 0x08d7d992
	void Read(int, WCHAR values[]);
	/// 0x08d7d9d0
	void Read(int, float values[]);
	/// 0x08d7da0e
	void Read(int, double values[]);



protected:
	// Members:
	//--------
	bool m_bEnableCB; ///< +0x4
	// the desired endianness of data
	FEndian::EEndianness m_eEndianness; ///< +0x8

	unsigned int m_uiBytesRead; ///< +0xc
	unsigned int m_uiPosition; ///< +0x10
};

/// 0x08d7cc87
FDataStream & operator<<(FDataStream & saveTo, FDataStream & readFrom);
/// 0x08d7ccf0
FDataStream & operator>>(FDataStream & loadFrom, FDataStream & writeTo);

/// inline
template<typename ValueType, size_t count>
FDataStream & operator<<(FDataStream & saveTo, const ValueType (&readFrom)[count])
{
	size_t i = 0;
	for(i = 0; i < count; ++i)
	{
		saveTo << readFrom[i];
	}
	return saveTo;
}

/// inline
template<typename ValueType, size_t count>
FDataStream & operator>>(FDataStream & loadFrom, ValueType (&writeTo)[count])
{
	size_t i = 0;
	for(i = 0; i < count; ++i)
	{
		loadFrom >> writeTo[i];
	}
	return loadFrom;
}

/// inline
template < class T >
inline FDataStream& FDataStream::operator <<( const T& kData )
{
	// If the compiler produces an error here complaining about 
	// YouMustImplementASerializeOperatorForThisTypeError then
	// it could not find a specialization for the type supplied
	// in scope. If a specialization exists, be sure to include
	// the header (FStlContainerSerialization.h, for example), 
	// if not, implement :
	// FDataStream & operator<<(FDataStream &, YourCustomType &)
	Write( kData );
	return ( *this );
}

/// inline
template < class T >
inline FDataStream& FDataStream::operator >>( T& kData )
{
	// If the compiler produces an error here complaining about 
	// YouMustImplementASerializeOperatorForThisTypeError then
	// it could not find a specialization for the type supplied
	// in scope. If a specialization exists, be sure to include
	// the header (FStlContainerSerialization.h, for example), 
	// if not, implement :
	// FDataStream & operator>>(FDataStream &, YourCustomType &)
	Read( kData );
	return ( *this );
}

/* JAR  - disabling this catch-all. It takes any non-specialized
          type, gets sizeof(T), then dumps it through a void *
		  producing a raw memory dump of the type in the stream.
		  Disabling this uncovered dozens of probably unknown bugs 
		  in serialization.

                                    ________
                              , -‘”          ``~ ,
                          , -”                    “- ,
                        ,/                           ”:,
                     ,?                                 \,
                    /                                    ,}
                  /                               ,:`^`  }
                /                              ,:”       /
               ?    __                        :`        /
              /__ (   “~-,_                 ,:`        /
             /(_  ”~,_    “~,_             ,:`       _/
            {  _$;_  ”=,_    “-,_     ,-~-,}, ~”;/  }
             ((   *~_  ”=- _    “;,, /`  /”          /
     ,,,___ \`~,     “~ ,           `     }         /
              (  `=-,,   `                 (  ;_,,-”
              / `~,      `-                 \   /\
               \`~ *-,                       |, / \,__
  ,,_          } >- _\                      |         `=~-,
       `=~-,_\_      `\,                     \
                  `=~-,, \,                    \
                            `:,,                `\        __
                                `=-,            ,%`>--==``
                                   _\    _,-%       `\
                                 ,<`  _|_,-&``      `\

             P I C A R D    F A C E P A L M
Because expressing how dumb that was in words just doesn't work

template < class T >
inline void FDataStream::Write( const T& value )
{
	WriteIt( sizeof ( T ), &value );
}
template < class T >
inline void FDataStream::Read( T& value )
{
ReadIt( sizeof ( T ), &value );
}
*/
/// inline
template < class T >
inline void FDataStream::Write( int count, const T values[] )
{
	if (sizeof(T) == 1)
		WriteIt( count, values );
	else
	{
		int i;
		for(i=0;i<count;i++)
		{
			//Write( values[i] );
			*this << values[i];
		}
	}
}

/// inline
template < class T >
inline void FDataStream::Read( int count, T values[] )
{
	if (sizeof(T) == 1)
		ReadIt( count, values );
	else
	{
		int i;
		for(i=0;i<count;i++)
		{
			// JAR  - comment this out, allow data stream to 
			// call user-supplied serialization, and not
			// restrict the class to the hard-coded types
			// available here
		//	Read( values[i] );
			*this >> values[i];
		}
	}
}

/*
template<typename T>
FDataStream & operator<<(FDataStream & saveTo, const T & readFrom)
{
	saveTo.Write(readFrom);
	return saveTo;
}

template<typename T>
FDataStream & operator>>(FDataStream & loadFrom, T & writeTo)
{
	loadFrom.Read(writeTo);
	return loadFrom;
}
*/

//-----------------------------------------------------------------------
// Support a bunch of legacy code that passes raw pointer data, arrays with magic number counts
// and dynamically allocated arrays. These should probably be encapsulated in containers
// for safety and manageability (std::vector, for example)
//-----------------------------------------------------------------------
/// size ?
template<typename ValueType>
class ArrayWrapper
{
public:
	/// inline
	ArrayWrapper(int count, ValueType * values)
		: m_values(values), m_count(count) {}

	/// inline
	ValueType * getArray()
	{
		return m_values;
	}
	/// inline
	const ValueType * getArray() const
	{
		return m_values;
	}

	/// inline
	int getCount() const
	{
		return m_count;
	}

	// Read an array, assuming there is a count value at the current position
	// in the stream.  If the count is less that the array size, the remaining entries
	// in the array will be filled with the supplied value.  If the count is greater than
	// the array, the remaining items in the stream will be skipped
	/// inline
	void readCounted(FDataStream & readFrom, ValueType tFill = (ValueType)0)
	{
		int i = 0;
		int iStreamCount;
		readFrom >> iStreamCount;
		int iArrayCount = getCount();
		int readCount = (iStreamCount < iArrayCount)?iStreamCount:iArrayCount;
		ValueType * values = getArray();
		for(i = 0; i < readCount; ++i)
		{
			readFrom >> values[i];
		}
		if (iStreamCount < iArrayCount)
		{
			for(; i < iArrayCount; ++i)
			{
				values[i] = tFill;
			}
		}
		else
		if (iStreamCount > iArrayCount)
		{
			for(; i < iStreamCount; ++i)
			{
				readFrom >> tFill;
			}		
		}
		
	}

	// Writes out the array, with a count value first.
	/// inline
	void writeCounted(FDataStream & writeTo)
	{
		int i = 0;
		int count = getCount();
		writeTo << count;
		const ValueType * values = getArray();
		for(i = 0; i < count; ++i)
		{
			writeTo << values[i];
		}
	}

private:
	// disabled, this wrapper does not belong in containers 
	// or allow copy construction
	/// absent
	ArrayWrapper();
	/// absent
	ArrayWrapper(const ArrayWrapper &);
	/// absent
	ArrayWrapper & operator=(const ArrayWrapper &);

	ValueType * m_values; ///< +0x0
	int         m_count; ///< +0x4
};

//Const version for writing (stores only a const pointer)
/// size ?
template< typename ValueType >
class ArrayWrapperConst
{
public:
	/// inline
	ArrayWrapperConst(int count, const ValueType * values)
		: m_values(values), m_count(count) {}

	/// inline
	const ValueType* getArray() const
	{
		return m_values;
	}
	/// inline
	int getCount() const
	{
		return m_count;
	}

	// Writes out the array, with a count value first.
	/// inline
	void writeCounted(FDataStream & writeTo)
	{
		int i = 0;
		int count = getCount();
		writeTo << count;
		const ValueType * values = getArray();
		for(i = 0; i < count; ++i)
		{
			writeTo << values[i];
		}
	}

private:

	// disabled, this wrapper does not belong in containers 
	// or allow copy construction
	/// absent
	ArrayWrapperConst();
	/// absent
	ArrayWrapperConst(const ArrayWrapperConst &);
	/// absent
	ArrayWrapperConst & operator=(const ArrayWrapperConst &);

	const ValueType * m_values; ///< +0x0
	int         m_count; ///< +0x4
};

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
// Stream operators for array wrapper classes
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------

/// inline
template<typename ValueType>
FDataStream & operator<<(FDataStream & saveTo, const ArrayWrapper<ValueType> & v)
{
	int i = 0;
	int count = v.getCount();
	const ValueType * values = v.getArray();
	for(i = 0; i < count; ++i)
	{
		saveTo << values[i];
	}
	return saveTo;
}

/// inline
template<typename ValueType>
FDataStream & operator>>(FDataStream & loadFrom, ArrayWrapper<ValueType> & v)
{
	int i = 0;
	int count = v.getCount();
	ValueType * values = v.getArray();
	for(i = 0; i < count; ++i)
	{
		loadFrom >> values[i];
	}
	return loadFrom;
}

/// inline
template<typename ValueType>
FDataStream & operator<<(FDataStream & saveTo, const ArrayWrapperConst<ValueType> & v)
{
	int i = 0;
	int count = v.getCount();
	const ValueType * values = v.getArray();
	for(i = 0; i < count; ++i)
	{
		saveTo << values[i];
	}
	return saveTo;
}

/// inline
template<typename ValueType>
FDataStream & operator>>(FDataStream & loadFrom, ArrayWrapperConst<ValueType> & v)
{
	int i = 0;
	int count = v.getCount();
	ValueType * values = v.getArray();
	for(i = 0; i < count; ++i)
	{
		loadFrom >> values[i];
	}
	return loadFrom;
}


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Serialization for Firaxis::Array
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
/// inline
template< class T, unsigned int ARRAY_SIZE >
FDataStream & operator >> ( FDataStream & kStream, Firaxis::Array<T, ARRAY_SIZE> & kArray )
{
	for( unsigned int i = 0; i < kArray.size(); ++i )
	{
		kStream >> kArray[i];
	}
	return kStream;
}

/// inline
template< class T, unsigned int ARRAY_SIZE >
FDataStream & operator << (FDataStream & kStream, const Firaxis::Array< T, ARRAY_SIZE > & kArray )
{
	for( unsigned int i = 0; i < kArray.size(); ++i )
	{
		kStream << kArray[i];
	}
	return kStream;
}


//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//Serialization for FFastVector and FStaticVector
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
/// inline
template< class T, bool bPODType, unsigned int AllocPool, unsigned int nSubID >
FDataStream & operator >> ( FDataStream & kStream, FFastVector< T, bPODType, AllocPool, nSubID > & kVec )
{
	unsigned int iSize;
	kStream >> iSize;

	if(iSize > kVec.size())
	{
		unsigned int count = iSize - kVec.size();
		kVec.push_back_copy(T(), count);
	}
	
	unsigned int i = 0;
	for( i = 0; i < iSize; ++i )
	{
		kStream >> kVec[i];
	}
	return kStream;
}

/// inline
template< class T, bool bPODType, unsigned int AllocPool, unsigned int nSubID >
FDataStream & operator << ( FDataStream & kStream, const FFastVector< T, bPODType, AllocPool, nSubID > & kVec )
{
	kStream << kVec.size();
	for( unsigned int i = 0; i < kVec.size(); ++i )
	{
		kStream << kVec[i];
	}
	return kStream;
}

/// inline
template< class T, unsigned int L, bool bPODType, unsigned int AllocPool, unsigned int nSubID >
FDataStream & operator >> ( FDataStream & kStream, FStaticVector< T, L, bPODType, AllocPool, nSubID > & kVec )
{
	unsigned int iSize;
	kStream >> iSize;
	kVec.resize( iSize );

	for( unsigned int i = 0; i < iSize; ++i )
	{
		kStream >> kVec[i];
	}
	return kStream;
}

/// inline
template< class T, unsigned int L, bool bPODType, unsigned int AllocPool, unsigned int nSubID >
FDataStream & operator << ( FDataStream & kStream, const FStaticVector< T, L, bPODType, AllocPool, nSubID > & kVec )
{
	kStream << (unsigned int)kVec.size();

	for( unsigned int i = 0; i < kVec.size(); ++i )
	{
		kStream << kVec[i];
	}
	return kStream;
}

#endif	//FDATASTREAM_H


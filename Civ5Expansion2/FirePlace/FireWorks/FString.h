/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FString.h
//
//  AUTHOR:		Mike Breitkreutz	--  08/26/2003
//
//  PURPOSE:	Implements a string class with support for both unicode and ascii.
// 
//				FStringA represents ANSI strings.
//				FStringW represents UNICODE strings.
//				FString is defined as an FStringW if _UNICODE is defined, FStringA otherwise.
// 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//				IMPORTANT RULES:
//				The m_pszString MUST be the ONLY member of the class.
//				Add additional data members to the FStringData class.
//				FString cannot contain ANY virtual functions.
// 
//				If these rules are not followed, the Format() functions will crash.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//		Nintendo DS Only:
//		*** Header implementation notes are at the bottom of <FStringA.inl> ***
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2007 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#ifndef		FSTRING_H
#define		FSTRING_H
#pragma		once

#include	"FirePlace/FireWorks/FArray.h"
#include	"FirePlace/FireWorks/FCrc32.h"

#ifdef		_DEBUG
#define		ENABLE_FSTRING_STATS
#endif	//	_DEBUG

//---------------------------------------------------------------------------------------
// Defines
//
#ifdef		_PS3
#define		DEFAULT_PATH_DELIMITER_A	'/'
#define		DEFAULT_PATH_DELIMITER_W	L'/'
#else	//	_PS3
#define		DEFAULT_PATH_DELIMITER_A	'\\'
#define		DEFAULT_PATH_DELIMITER_W	L'\\'
#endif	//	_PS3
#define		DEFAULT_PATH_DELIMITER		_T( DEFAULT_PATH_DELIMITER_A )	// Character used to delimit paths

//---------------------------------------------------------------------------------------
// Macros
//
#define	 FSTRING_CHECK_BUFFER	assert( !IsLocked() && "Don't forget to call ReleaseBuffer" )	// Warning for using GetBuffer/ReleaseBuffer safely

//---------------------------------------------------------------------------------------
// Typedefs
//
#ifndef		LPSTR
typedef char* LPSTR;
#endif	//	LPSTR
#ifndef		LPCSTR
typedef const char* LPCSTR;
#endif	//	LPCSTR
#ifndef		LPWSTR
typedef wchar* LPWSTR;
#endif	//	LPWSTR
#ifndef		LPCWSTR
typedef const wchar* LPCWSTR;
#endif	//	LPCWSTR

typedef uint FStringHashType;
//---------------------------------------------------------------------------------------
// Constants
//
/// inline
const size_t nFSTRING_HASH_TRAITS_BUCKET_SIZE	= 3;	// Mean number of elements per bucket -- must be > 0
/// inline
const size_t nFSTRING_HASH_TRAITS_MIN_BUCKETS	= 8;	// Minimum number of buckets to maintain -- must be > 0 and a power of 2

//---------------------------------------------------------------------------------------
// FStringA
//
//	Represents an ANSI string.
//
/// size 0x4
class FStringA
{
#ifndef		_NDS
public:
	// Structures:
	//-----------
	/// size 0xc
	struct FStringAData	// Data for ANSI strings.
	{
		// Using all ints for conformity
		// Number of chars allocated for the string but not including null terminator
		int m_iAllocLength; ///< +0x0
		// Length of the string not including null terminator
		int m_iDataLength; ///< +0x4
		// If true, the buffer is locked; meaning GetBuffer() was called to get write access to the StringData
		USHORT m_bLocked; ///< +0x8
		// If true, the buffer is a fixed length and it cannot be re-allocated or released
		USHORT m_bFixed; ///< +0xa
		//char szData[ m_iAllocLength ];

		// Access to actual string/character data
		/// inline
		LPSTR StringData( void );
	};
#endif	//	!_NDS

public:
	// Enums:
	//------
	enum CtorType
	{
		FORMAT = 1,	// pass this in to Format constructor
	};

#ifdef		_NDS
	enum FlagType
	{
		LOCKED = 1,	// active GetBuffer(), before ReleaseBuffer()
		STATIC = 2,	// static string alloc'd on Frame Heap
		FIXED  = 4,	// fixed string: no buffer ops
		LARGE  = 8,	// larger than 65499 characters
		UNUSED = 16
	};
#endif	//	_NDS

	// Classes:
	//--------
	/// size 0x1
	class FStringAHashTraits	// Traits for hash_map
	{
	public:
		// Enums:
		//------
		enum
		{
			bucket_size = nFSTRING_HASH_TRAITS_BUCKET_SIZE,	// Mean number of elements per bucket
			min_buckets	= nFSTRING_HASH_TRAITS_MIN_BUCKETS,	// Minimum number of buckets to maintain
		};

		// Constructors:
		//-------------
		/// inline
		FStringAHashTraits( void );

		// Operators:
		//----------
		/// inline
		size_t operator()( const FStringA& str ) const;
		/// inline
		bool operator()( const FStringA& str1, const FStringA& str2 ) const;
	};

	/// size 0x1
	class FStringAHashTraitsNoCase	// Traits for hash_map with case-insensitve compare
	{
	public:
		// Enums:
		//------
		enum
		{
			bucket_size = nFSTRING_HASH_TRAITS_BUCKET_SIZE,	// Mean number of elements per bucket
			min_buckets	= nFSTRING_HASH_TRAITS_MIN_BUCKETS,	// Minimum number of buckets to maintain
		};

		// Constructors:
		//-------------
		/// inline
		FStringAHashTraitsNoCase( void );

		// Operators:
		//----------
		/// inline
		size_t operator()( const FStringA& str ) const;
		/// inline
		bool operator()( const FStringA& str1, const FStringA& str2 ) const;
	};

	/// size 0x10
	class FBuffer	// Convenient class for accessing the internal buffer
	{
	public:
		// Constructors:
		//-------------
		/// inline
		FBuffer( FStringA& str, int iMinBufLength = 0, bool bNullTerminated = true );

		// Destructors:
		//------------
		/// inline
		~FBuffer( void );

		// Operators:
		//----------
		/// inline
		operator LPSTR( void );


	private:
		// Members:
		//--------
		// If true, the string is assumed to be null-terminated
		bool m_bNullTerminated; ///< +0x0

		// The reference to the FString containing the buffer
		FStringA& m_strRef; ///< +0x4

		// The length of the string if it is not null-terminated
		int m_iLength; ///< +0x8

		// The actual string buffer
		LPSTR m_pszBuffer; ///< +0xc
	};

	#ifndef		_NDS
	/// size ?
	template < size_t nLENGTH >
	class FStringAFixedBuffer	// Helper class used to create strings of a fixed-length
	{
	public:
		// Constructors:
		//-------------
		/// inline
		FStringAFixedBuffer( FStringA& str );
		/// inline
		FStringAFixedBuffer();
		
		/// inline
		FStringAData &GetStringData() { return m_kStringData; }

	private:
		// Members:
		//--------
		// Data for the fixed-length string; must come first
		FStringAData m_kStringData; ///< +0x0

		// Fixed-length string buffer; must be second
		char m_szStringBuffer[ nLENGTH ]; ///< +0xc
	};
	#endif	//	_NDS

	// Constructors:
	//-------------
	         /// inline
	         FStringA( void );
	         /// inline
	         FStringA( const FStringA& strSource );
	         /// inline
	         FStringA( char ch, int iRepeat );
	         /// inline
	         FStringA( LPCSTR lpszSource );
	         /// inline
	         FStringA( LPCSTR lpszSource, int iLength );
	/// inline
	explicit FStringA( int iLength );
			 #ifdef		_NDS
	explicit FStringA( int iLength, void* pBuffer );
			 #endif	//	_NDS
	         /// inline
	         FStringA( const std::string& strSource );
	         /// 0x08d7f974
	         FStringA( CtorType /* ignored, but needed to disambiguate vs other constructors */, LPCSTR lpszFormat, ... );
			 #ifdef		_NDS
			 FStringA( FlagType /* ignored */ );
			 #endif	//	_NDS

	// Destructors:
	//------------
	/// inline
	~FStringA( void );

	// Operators:
	//----------
	// Assignment
	/// inline
	const FStringA& operator=( const FStringA& strSource );
	/// inline
	const FStringA& operator=( char ch );
	/// inline
	const FStringA& operator=( LPCSTR lpszSource );
	/// inline
	const FStringA& operator=( const std::string& strSource );
	
	// Addition + assignment
	/// inline
	const FStringA& operator+=( const FStringA& strSource );
	/// inline
	const FStringA& operator+=( char ch );
	/// inline
	const FStringA& operator+=( LPCSTR lpszSource );
	/// inline
	const FStringA& operator+=( const std::string& strSource );
	
	// Addition
	       /// inline
	       FStringA operator+( const FStringA& s2 );
	       /// inline
	       FStringA operator+( const std::string& s2 );
	       /// inline
	       FStringA operator+( char ch );
	friend FStringA operator+( char ch, const FStringA& s2 );
	       /// inline
	       FStringA operator+( LPCSTR s2 );
	friend FStringA operator+( LPCSTR s1, const FStringA& s2 );
	
	// Subtraction + assignment
	/// inline
	const FStringA& operator-=( const FStringA& strSource );
	/// inline
	const FStringA& operator-=( const std::string& strSource );
	/// inline
	const FStringA& operator-=( char ch );
	/// inline
	const FStringA& operator-=( LPCSTR lpszSource );

	// Subtraction
	       /// inline
	       FStringA operator-( const FStringA& s2 );
	       /// inline
	       FStringA operator-( const std::string& s2 );
	       /// inline
	       FStringA operator-( char ch );
	friend FStringA operator-( char ch, const FStringA& s2 );
	       /// inline
	       FStringA operator-( LPCSTR s2 );
	friend FStringA operator-( LPCSTR s1, const FStringA& s2 );

	// Equality
	       /// inline
	       bool operator==( const FStringA& s2 ) const;
	       /// inline
	       bool operator==( const std::string& s2 ) const;
	       /// inline
	       bool operator==( LPCSTR s2 ) const;
	friend bool operator==( LPCSTR s1, const FStringA& s2 );

	// Inequality
	       /// inline
	       bool operator!=( const FStringA& s2 ) const;
	       /// inline
	       bool operator!=( const std::string& s2 ) const;
	       /// inline
	       bool operator!=( LPCSTR s2 ) const;
	friend bool operator!=( LPCSTR s1, const FStringA& s2 );

	// Less than
	       /// inline
	       bool operator<( const FStringA& s2 ) const;
	       /// inline
	       bool operator<( const std::string& s2 ) const;
	       /// inline
	       bool operator<( LPCSTR s2 ) const;
	friend bool operator<( LPCSTR s1, const FStringA& s2 );

	// Less than or equal
	       /// inline
	       bool operator<=( const FStringA& s2 ) const;
	       /// inline
	       bool operator<=( const std::string& s2 ) const;
	       /// inline
	       bool operator<=( LPCSTR s2 ) const;
	friend bool operator<=( LPCSTR s1, const FStringA& s2 );

	// Greater than
	       /// inline
	       bool operator>( const FStringA& s2 ) const;
	       /// inline
	       bool operator>( const std::string& s2 ) const;
	       /// inline
	       bool operator>( LPCSTR s2 ) const;
	friend bool operator>( LPCSTR s1, const FStringA& s2 );

	// Greater than or equal
	       /// inline
	       bool operator>=( const FStringA& s2 ) const;
	       /// inline
	       bool operator>=( const std::string& s2 ) const;
	       /// inline
	       bool operator>=( LPCSTR s2 ) const;
	friend bool operator>=( LPCSTR s1, const FStringA& s2 );

	// Shifting
	/// inline
	FStringA  operator<< ( int iCount ) const;
	/// inline
	FStringA& operator<<=( int iCount );
	/// inline
	FStringA  operator>> ( int iCount ) const;
	/// inline
	FStringA& operator>>=( int iCount );

	// Subscript
	/// inline
	char& operator[](  int  iIndex );
	/// inline
	char& operator[]( uint uiIndex );
	/// inline
	char  operator[](  int  iIndex ) const;
	/// inline
	char  operator[]( uint uiIndex ) const;

	// Casting
	/// inline
	operator LPCSTR( void ) const;

	// Methods:
	//--------
	// Attributes and operations
	/// inline
	void Empty( void );
	/// inline
	bool IsEmpty( void ) const;
	/// inline
	int  GetLength( void ) const;
	/// absent
	int  GetMaxLength( void ) const;

	// Access to string implementation buffer as "C" character array
	/// 0x08d82020
	void Reserve( int iLength );
	/// inline
	void Release( void );
	/// 0x08d832da
	void FreeExtra( void );
	/// inline
	LPCSTR c_str( void ) const;
	/// inline
	LPCSTR GetCString( void ) const;

	// Character operations
	/// inline
	char First( void ) const;
	/// inline
	char Last( void ) const;
	/// inline
	char GetAt( int iIndex ) const;
	/// 0x08d7fdda
	void SetAt( int iIndex, char ch );
	/// 0x08d7fe88
	void SetAt( int iIndex, int iLength, LPCSTR lpszSource, int iFirst = 0 );

	// String comparison
	/// 0x08d7ff08
	bool MatchesPattern( LPCSTR lpszPattern, bool bCaseSensitive = false ) const;
	/// inline
	int Compare( LPCSTR lpsz ) const;
	/// inline
	int Compare( LPCSTR lpsz, int iLength ) const;
	/// inline
	int CompareNoCase( LPCSTR lpsz ) const;
	/// inline
	int CompareNoCase( LPCSTR lpsz, int iLength ) const;
	/// inline
	int Collate( LPCSTR lpsz ) const;
	/// inline
	int CollateNoCase( LPCSTR lpsz ) const;

	// Simple sub-string extraction
	/// 0x08d8020c
	FStringA Mid( int iFirst, int iCount ) const;
	/// inline
	FStringA Mid( int iFirst ) const;
	/// inline
	FStringA Left( int iCount ) const;
	/// inline
	FStringA Right( int iCount ) const;
	/// 0x08d8039e
	void Mid( FStringA & szDest, int iFirst, int iCount ) const;
	/// inline
	void Mid( FStringA & szDest, int iFirst ) const;
	/// 0x08d80424
	static void Mid( FStringA & szDest, LPCSTR lpszSrc, int iFirst, int iCount );
	/// inline
	void Left( FStringA & szDest, int iCount ) const;
	/// inline
	void Right( FStringA & szDest, int iCount ) const;

	// Advanced sub-string extraction
	/// inline
	FStringA SpanIncluding( LPCSTR lpszCharSet, int iStart = 0 ) const;
	/// inline
	FStringA SpanExcluding( LPCSTR lpszCharSet, int iStart = 0 ) const;
	/// 0x08d804c8
	FStringA Extract( int& iStart, LPCSTR lpszCharSet ) const;
	/// 0x08d80592
	FStringA Extract( int& iStart, char chEnd ) const;
	/// 0x08d80648
	FStringA Extract( int& iStart, char chBegin, char chEnd ) const;
	/// inline
	void SpanIncluding( FStringA & szDest, LPCSTR lpszCharSet, int iStart = 0 ) const;
	/// inline
	void SpanExcluding( FStringA & szDest, LPCSTR lpszCharSet, int iStart = 0 ) const;
	/// 0x08d8079c
	void Extract( FStringA & szDest, int& iStart, LPCSTR lpszCharSet ) const;
	/// 0x08d80870
	void Extract( FStringA & szDest, int& iStart, char chEnd ) const;
	/// 0x08d808d6
	void Extract( FStringA & szDest, int& iStart, char chBegin, char chEnd ) const;
	/// 0x08d8004a
	static void Extract( FStringA & szDest, LPCSTR szSrc, int& iStart, LPCSTR szDelimiters );

	// Path extraction
	/// 0x08d80970
	FStringA ExtractDriveLetter( void ) const;
	/// 0x08d80b4a
	FStringA ExtractFileDir( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d80db0
	FStringA ExtractFileExtension( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d80f7c
	FStringA ExtractFileName( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d8121a
	FStringA ExtractFilePath( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d81474
	FStringA ExtractFileTitle( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d80ad4
	void ExtractDriveLetter( FStringA & szDest ) const;
	/// 0x08d80d10
	void ExtractFileDir( FStringA & szDest, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d80eea
	void ExtractFileExtension( FStringA & szDest, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d81162
	void ExtractFileName( FStringA & szDest, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d813d0
	void ExtractFilePath( FStringA & szDest, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// 0x08d815f6
	void ExtractFileTitle( FStringA & szDest, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// inline
	void ReplaceDriveLetter( LPCSTR lpszNewDrive, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void ReplaceFileDir( LPCSTR lpszNewDir, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void ReplaceFileExtension( LPCSTR lpszNewExt, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void ReplaceFileName( LPCSTR lpszNewName, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void ReplaceFilePath( LPCSTR lpszNewPath, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void ReplaceFileTitle( LPCSTR lpszNewTitle, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void SplitPath( FStringA& strDriveLetter, FStringA& strFileDir, FStringA& strFileName, FStringA& strFileExt, char chPathDelimiter = DEFAULT_PATH_DELIMITER_A ) const;
	/// inline
	void StandardizePath( bool bChangeCase, bool bTerminate );
	/// inline
	void StripDriveLetter( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void StripFileDir( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void StripFileExtension( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void StripFileName( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void StripFilePath( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );
	/// inline
	void StripFileTitle( char chPathDelimiter = DEFAULT_PATH_DELIMITER_A );

	// Upper/Lower/Reverse conversion
	/// inline
	void MakeUpper( void );
	/// inline
	void MakeLower( void );
	/// inline
	void MakeReverse( void );

	// Trimming whitespace
	/// 0x08d81718
	void TrimRight( void );
	/// 0x08d81776
	void TrimRight( char chTarget );
	/// 0x08d817ce
	void TrimRight( LPCSTR lpszTargets );
	/// 0x08d8184e
	void TrimLeft( void );
	/// 0x08d818b4
	void TrimLeft( char chTarget );
	/// 0x08d81918
	void TrimLeft( LPCSTR lpszTargets );
	/// inline
	void Trim( void );
	/// inline
	void Trim( char chTarget );
	/// inline
	void Trim( LPCSTR lpszTargets );
	/// inline
	void TrimWhitespace( void );
	/// 0x08d819b2
	void Truncate( int iNewLength );

	// Advanced manipulation
	/// 0x08d819ee
	int Delete( int iIndex, int iCount = 1 );
	/// 0x08d81a7e
	int Delete( LPCSTR lpszSource );
	/// 0x08d81b64
	int Insert( int iIndex, char ch );
	/// 0x08d81bd0
	int Insert( int iIndex, LPCSTR lpsz );
	/// 0x08d81c70
	int Remove( char chRemove );
	/// 0x08d81cc8
	int Remove( LPCSTR lpszRemove );
	/// 0x08d81d56
	int RemoveRepeated( char chRemove );
	/// 0x08d81e0e
	int Replace( char chOld, char chNew );
	/// 0x08d81e3e
	int Replace( LPCSTR lpszOld, LPCSTR lpszNew );
	/// inline
	int Replace( char chOld, LPCSTR lpszNew );
	/// inline
	int Replace( LPCSTR lpszOld, char chNew );
	/// 0x08d82062
	int Wrap( int iMaxLineLength );

	// Searching
	/// 0x08d805fa
	int Find( char ch, int iStart = 0 ) const;
	/// 0x08d81b16
	int Find( LPCSTR lpszSub, int iStart = 0 ) const;
	/// 0x08d80544
	int FindOneOf( LPCSTR lpszCharSet, int iStart = 0 ) const;
	/// 0x08d80816
	static int FindOneOf( LPCSTR lpszSrc, LPCSTR lpszCharSet, int iStart = 0);

	/// 0x08d80ca0
	int ReverseFind( char ch, int iStart = -1 ) const;
	/// 0x08d822a4
	int FindCount( char ch, int iStart = 0 ) const;
	/// 0x08d822ea
	int FindToken( LPCSTR lpszToken, int iStart = 0 ) const;
	/// 0x08d82396
	int FindNextWord( int iStart = 0 ) const;
	/// 0x08d82412
	int FindPrevWord( int iStart = -1 ) const;

	// Simple formatting
	/// 0x08d8249e
	void __cdecl Format( LPCSTR lpszFormat, ... );
	/// 0x08d824d4
	void __cdecl Format( uint uiFormatID, ... );			// from custom (fxs) string table
#ifdef		_WINPC
	/// 0x08d825c8
	void __cdecl FormatResource( uint uiFormatID, ... );	// from microsoft string table in .rc file
#endif	//	_WINPC
	/// 0x08d82736
	void FormatSize( uint uiSize );
	/// 0x08d7f9b2
	void FormatV( LPCSTR lpszFormat, va_list vaArgList );
	/// 0x08d828c8
	void TranslateEscapeCodes( void );

	// Load from string resources
	/// absent
	bool LoadString( uint uiID );								// from custom (fxs) string table
#ifdef		_WINPC
	/// 0x08d82674
	bool LoadResource( uint uiID, HINSTANCE hInstance = NULL );	// from microsoft string table in .rc file
#endif	//	_WINPC

	// String copying
	/// 0x08d8031a
	void Copy( int iLength, LPCSTR lpszSource, int iFirst = 0 );
	/// 0x08d83354
	void CopyUTF8(LPCSTR lpszSource, char chBad = '*');
	/// 0x08d835be
	void CopyToUTF8(FStringA* pkOutput);

	//Convert a UTF-8 encoded string into the local system code page then Copy.
	//NOTE:	FString does not track character encoding!
	//		Most uses of FString assume the local system code page which may or may not be UTF-8.
	//		This method should always be used when working with UTF-8 buffers.
	/// 0x08d833a2
	bool CopyUTF8toMultiByte(LPCSTR szSource);


	// String concatenation
	/// inline
	void Concat( int iLength, LPCSTR lpszSource, int iFirst = 0 );
	/// inline
	void Concat( LPCSTR lpszSource );

	// Numeric conversion
	/// 0x08d8313e
	int ConvertToInt( int iRadix = 10 , bool bReadHex = true) const;
	/// 0x08d837e6
	float ConvertToFloat( int iRadix = 10 ) const;
	/// inline
	uint Hash( void ) const;

	// Encryption
	/// 0x08d83950
	void Encrypt( void );
	/// 0x08d83a6c
	void Decrypt( void );

	/// inline
	void SetFixedBuffer( FStringAData* pkData );

	/// 0x08d8330c
	void TakeBufferFrom( FStringA& szDonor );

	// Static Methods:
	//---------------
	/// inline
	static int SafeStrlen( LPCSTR lpsz );
	/// inline
	static int SafeStrcmp( LPCSTR lpszStr1, LPCSTR lpszStr2 );
	/// inline
	static int SafeStricmp( LPCSTR lpszStr1, LPCSTR lpszStr2 );
	/// 0x08d83b96
	static TCHAR * SafeStrDup( LPCSTR lpsz );
	
	/// inline
	static size_t SizeofFStringData( void );

	/// inline
	static FStringHashType Hash( LPCSTR lpsz );

	/// absent
	static char* InitializeFixedBufferAt( size_t iSize, void* pkBuffer, size_t iMaxBuffer );
	/// 0x08d8360a
	static void CopyToUTF8(LPCSTR pszSource, FStringA* pkOutput);
		
protected:
	// Methods:
	//--------
	/// inline
	bool IsLocked( void ) const;
	/// inline
	bool IsFixed(  void ) const;
#ifdef		_NDS
	bool IsLarge(  void ) const;
	bool IsStatic( void ) const;
#endif	//	_NDS

	/// 0x08d800d2
	bool MatchesOnePattern( LPCSTR lpszPattern, bool bCaseSensitive = false ) const;
	
#ifndef		_NDS
	/// inline
	FStringAData* GetData( void ) const;
#endif	//	_NDS

	/// 0x08d80a90
	int FindDriveLetter( void ) const;

	/// 0x08d83240
	LPSTR GetBuffer( int iMinBufLength = 0 );

	/// inline
	void Init( void );
	/// inline
	void LockBuffer( void );
	/// 0x08d83290
	void ReleaseBuffer( size_t iNewLength = -1 );
	/// 0x08d7fe2a
	void SetLength( size_t iLength );
	/// inline
	void DecLength( size_t iLength );
	/// inline
	void UnlockBuffer( void );
	/// absent
	void SetCharCount(  size_t iLength );
	/// inline
	int  GetAllocLength( void ) const;

#ifndef		_NDS
	/// 0x08d83c36
	void Reallocate( size_t iAllocSize );
	/// inline
	void Release( FStringAData* pkData );
#endif	//	!_NDS

	/// 0x08d83bec
	void  AllocBuffer( size_t nChars );

#ifdef		_NDS
	void* GetAllocPtr( void );

		  uint& Header( int index );		// Header accessors
	const uint& Header( int index ) const;
#endif	//	_NDS

private:
	// Members:
	//--------
	// WARNING: The m_pszString MUST be the ONLY member of this class!!!
	// Actual string data
	LPSTR m_pszString; ///< +0x0

	friend class FStringHashPalette; //  please don't hate me....
	friend class FStringW;
	// seed: in Civ5XP, not in SDK
public:
	/// 0x08d82588
	UNK_RET LoadStringA(unsigned int a1);
};

// Access to the common initializer data for an empty string
#if !defined(FXS_IS_DLL)
extern "C" 
{
	/// 0x08d7f960
	__declspec(dllexport) extern FStringA::FStringAData* FStringA_GetStringInitData();
}
#else
FStringA::FStringAData* FStringA_GetStringInitData();
#endif

// Friendly helpers
/// inline
FStringA operator+( char ch, const FStringA& s2 );
/// inline
FStringA operator+( LPCSTR s1, const FStringA& s2 );

/// inline
FStringA operator-( char ch, const FStringA& s2 );
/// inline
FStringA operator-( LPCSTR s1, const FStringA& s2 );

/// inline
bool operator==( LPCSTR s1, const FStringA& s2 );
/// inline
bool operator!=( LPCSTR s1, const FStringA& s2 );

/// inline
bool operator<( LPCSTR s1, const FStringA& s2 );
/// inline
bool operator<=( LPCSTR s1, const FStringA& s2 );
/// inline
bool operator>( LPCSTR s1, const FStringA& s2 );
/// inline
bool operator>=( LPCSTR s1, const FStringA& s2 );

//---------------------------------------------------------------------------------------
// FStringW
//
//	Represents a UNICODE string.
//
/// size 0x4
class FStringW
{
#ifndef		_NDS
public:
	// Structures:
	//-----------
	/// size 0xc
	struct FStringWData	// Data for UNICODE strings.
	{
		// Using all ints for conformity
		// Number of wchars allocated for the string but not including null terminator
		int m_iAllocLength; ///< +0x0
		// Length of the string not including null terminator
		int m_iDataLength; ///< +0x4
		// If true, the buffer is locked; meaning GetBuffer() was called to get write access to the StringData
		USHORT m_bLocked; ///< +0x8
		// If true, the buffer is a fixed length and it cannot be re-allocated or released
		USHORT m_bFixed; ///< +0xa
		//wchar szData[ m_iAllocLength ];

		// Access to actual string/character data
		/// inline
		LPWSTR StringData( void );
	};
#endif	//	!_NDS

public:
	// Enums:
	//------
	enum CtorType
	{
		FORMAT = 1,	// pass this in to Format constructor
	};

#ifdef		_NDS
	enum FlagType
	{
		LOCKED = 1,	// active GetBuffer(), before ReleaseBuffer()
		STATIC = 2,	// static string alloc'd on Frame Heap
		FIXED  = 4,	// fixed string: no buffer ops
		LARGE  = 8,	// larger than 65499 characters
		UNUSED = 16
	};
#endif	//	_NDS

	// Classes:
	//--------
	/// size 0x1
	class FStringWHashTraits	// Traits for hash_map
	{
	public:
		// Enums:
		//------
		enum
		{
			bucket_size = nFSTRING_HASH_TRAITS_BUCKET_SIZE,	// Mean number of elements per bucket
			min_buckets	= nFSTRING_HASH_TRAITS_MIN_BUCKETS,	// Minimum number of buckets to maintain
		};

		// Constructors:
		//-------------
		/// inline
		FStringWHashTraits( void );

		// Operators:
		//----------
		/// inline
		size_t operator()( const FStringW& str ) const;
		/// inline
		bool operator()( const FStringW& str1, const FStringW& str2 ) const;
	};

	/// size 0x1
	class FStringWHashTraitsNoCase	// Traits for hash_map with case-insensitve comparison
	{
	public:
		// Enums:
		//------
		enum
		{
			bucket_size = nFSTRING_HASH_TRAITS_BUCKET_SIZE,	// Mean number of elements per bucket
			min_buckets	= nFSTRING_HASH_TRAITS_MIN_BUCKETS,	// Minimum number of buckets to maintain
		};

		// Constructors:
		//-------------
		/// inline
		FStringWHashTraitsNoCase( void );

		// Operators:
		//----------
		/// inline
		size_t operator()( const FStringW& str ) const;
		/// inline
		bool operator()( const FStringW& str1, const FStringW& str2 ) const;
	};

	/// size 0x10
	class FBuffer	// Convenient class for accessing the internal buffer
	{
	public:
		// Constructors:
		//-------------
		/// inline
		FBuffer( FStringW& str, int iMinBufLength = 0, bool bNullTerminated = true );

		// Destructors:
		//------------
		/// inline
		~FBuffer( void );

		// Operators:
		//----------
		/// inline
		operator LPWSTR( void );


	private:
		// Members:
		//--------
		// If true, the string is assumed to be null-terminated
		bool m_bNullTerminated; ///< +0x0

		// The reference to the FString containing the buffer
		FStringW& m_strRef; ///< +0x4

		// The length of the string if it is not null-terminated
		int m_iLength; ///< +0x8

		// The actual string buffer
		LPWSTR m_pszBuffer; ///< +0xc
	};

	#ifndef		_NDS
	/// size ?
	template < size_t nLENGTH >
	class FStringWFixedBuffer	// Helper class used to create strings of a fixed-length
	{
	public:
		// Constructors:
		//-------------
		/// inline
		FStringWFixedBuffer( FStringW& str );
		/// inline
		FStringWFixedBuffer();

		/// inline
		FStringWData &GetStringData() { return m_kStringData; }

	private:
		// Members:
		//--------
		// Data for the fixed-length string; must come first
		FStringWData m_kStringData; ///< +0x0
		
		// Fixed-length string buffer; must be second
		wchar m_szStringBuffer[ nLENGTH ]; ///< +0xc
	};
	#endif	//	_NDS

	// Constructors:
	//-------------
	         /// inline
	         FStringW( void );
	         /// inline
	         FStringW( const FStringW& strSource );
	         /// inline
	         FStringW( wchar ch, int iRepeat );
	         /// inline
	         FStringW( LPCWSTR lpszSource );
	         /// inline
	         FStringW( LPCWSTR lpszSource, int iLength );
	/// inline
	explicit FStringW( int iLength );
			 #ifdef		_NDS
	explicit FStringW( int iLength, void* pBuffer );
			 #endif
	         /// inline
	         FStringW( const std::wstring& strSource );
	         /// 0x08d83ce6
	         FStringW( CtorType /* ignored, but needed to disambiguate vs other constructors */, LPCWSTR lpszFormat, ... );
			 #ifdef		_NDS
			 FStringW( FlagType /* ignored */ );
			 #endif	//	_NDS

	// Destructors:
	//------------
	/// inline
	~FStringW( void );

	// Operators:
	//----------
	// Assignment
	/// inline
	const FStringW& operator=( const FStringW& strSource );
	/// 0x08d8419a
	const FStringW& operator=( const std::wstring& strSource );
	/// inline
	const FStringW& operator=( wchar ch );
	/// inline
	const FStringW& operator=( LPCWSTR lpszSource );

	// Addition + assignment
	/// inline
	const FStringW& operator+=( const FStringW& strSource );
	/// 0x08d84274
	const FStringW& operator+=( const std::wstring& strSource );
	/// inline
	const FStringW& operator+=( wchar ch );
	/// inline
	const FStringW& operator+=( LPCWSTR lpszSource );

	// Addition
	       /// inline
	       FStringW operator+( const FStringW& s2 );
	       /// 0x08d842cc
	       FStringW operator+( const std::wstring& s2 );
	       /// inline
	       FStringW operator+( wchar ch );
	friend FStringW operator+( wchar ch, const FStringW& s2 );
	       /// inline
	       FStringW operator+( LPCWSTR s2 );
	friend FStringW operator+( LPCWSTR s1, const FStringW& s2 );

	// Subtraction + assignment
	/// inline
	const FStringW& operator-=( const FStringW& strSource );
	/// inline
	const FStringW& operator-=( const std::wstring& strSource );
	/// inline
	const FStringW& operator-=( wchar ch );
	/// inline
	const FStringW& operator-=( LPCWSTR lpszSource );

	// Subtraction
	       /// inline
	       FStringW operator-( const FStringW& s2 );
	       /// inline
	       FStringW operator-( const std::wstring& s2 );
	       /// inline
	       FStringW operator-( wchar ch );
	friend FStringW operator-( wchar ch, const FStringW& s2 );
	       /// inline
	       FStringW operator-( LPCWSTR s2 );
	friend FStringW operator-( LPCWSTR s1, const FStringW& s2 );

	// Equality
	       /// inline
	       bool operator==( const FStringW& s2 ) const;
	       /// inline
	       bool operator==( LPCWSTR s2 ) const;
	friend bool operator==( LPCWSTR s1, const FStringW& s2 );

	// Inequality
	       /// inline
	       bool operator!=( const FStringW& s2 ) const;
	       /// inline
	       bool operator!=( LPCWSTR s2 ) const;
	friend bool operator!=( LPCWSTR s1, const FStringW& s2 );

	// Less than
	       /// inline
	       bool operator<( const FStringW& s2 ) const;
	       /// inline
	       bool operator<( LPCWSTR s2 ) const;
	friend bool operator<( LPCWSTR s1, const FStringW& s2 );

	// Less than or equal
	       /// inline
	       bool operator<=( const FStringW& s2 ) const;
	       /// inline
	       bool operator<=( LPCWSTR s2 ) const;
	friend bool operator<=( LPCWSTR s1, const FStringW& s2 );

	// Greater than
	       /// inline
	       bool operator>( const FStringW& s2 ) const;
	       /// inline
	       bool operator>( LPCWSTR s2 ) const;
	friend bool operator>( LPCWSTR s1, const FStringW& s2 );

	// Greater than or equal
	       /// inline
	       bool operator>=( const FStringW& s2 ) const;
	       /// inline
	       bool operator>=( LPCWSTR s2 ) const;
	friend bool operator>=( LPCWSTR s1, const FStringW& s2 );

	// Shifting
	/// inline
	FStringW  operator<< ( int iCount ) const;
	/// inline
	FStringW& operator<<=( int iCount );
	/// inline
	FStringW  operator>> ( int iCount ) const;
	/// inline
	FStringW& operator>>=( int iCount );

	// Subscript
	/// inline
	wchar& operator[]( int iIndex );
	/// inline
	wchar& operator[]( uint uiIndex );
	/// inline
	wchar operator[]( int iIndex ) const;
	/// inline
	wchar operator[]( uint uiIndex ) const;

	// Casting
	/// inline
	operator LPCWSTR( void ) const;

	// Methods:
	//--------
	// Attributes and operations
	/// inline
	void Empty( void );
	/// inline
	bool IsEmpty( void ) const;
	/// inline
	int  GetLength( void ) const;
	/// absent
	int  GetMaxLength( void ) const;

	// Access to string implementation buffer as "C" character array
	/// 0x08d863d6
	void Reserve( int iLength );
	/// inline
	void Release( void );
	/// 0x08d876a2
	void FreeExtra( void );
	/// inline
	LPCWSTR c_str( void ) const;
	/// inline
	LPCWSTR GetCString( void ) const;

	// Character operations
	/// inline
	wchar First( void ) const;
	/// inline
	wchar Last( void ) const;
	/// inline
	wchar GetAt( int iIndex ) const;
	/// 0x08d8437a
	void SetAt( int iIndex, wchar ch );
	/// 0x08d84438
	void SetAt( int iIndex, int iLength, LPCWSTR lpszSource, int iFirst = 0 );

	// String comparison
	/// 0x08d844be
	bool MatchesPattern( LPCWSTR lpszPattern, bool bCaseSensitive = false ) const;
	/// inline
	int Compare( LPCWSTR lpsz ) const;
	/// inline
	int Compare( LPCWSTR lpsz, int iLength ) const;
	/// inline
	int CompareNoCase( LPCWSTR lpsz ) const;
	/// inline
	int CompareNoCase( LPCWSTR lpsz, int iLength ) const;
	/// inline
	int Collate( LPCWSTR lpsz ) const;
	/// inline
	int CollateNoCase( LPCWSTR lpsz ) const;

	// Simple sub-string extraction
	/// 0x08d8483a
	FStringW Mid( int iFirst, int iCount ) const;
	/// inline
	FStringW Mid( int iFirst ) const;
	/// inline
	FStringW Left( int iCount ) const;
	/// inline
	FStringW Right( int iCount ) const;
	/// 0x08d84948
	void Mid( FStringW & szDest, int iFirst, int iCount ) const;
	/// inline
	void Mid( FStringW & szDest, int iFirst ) const;
	/// inline
	void Left( FStringW & szDest, int iCount ) const;
	/// inline
	void Right( FStringW & szDest, int iCount ) const;

	// Advanced sub-string extraction
	/// inline
	FStringW SpanIncluding( LPCWSTR lpszCharSet, int iStart = 0 ) const;
	/// inline
	FStringW SpanExcluding( LPCWSTR lpszCharSet, int iStart = 0 ) const;
	/// 0x08d849ce
	FStringW Extract( int& iStart, LPCWSTR lpszCharSet ) const;
	/// 0x08d84682
	FStringW Extract( int& iStart, wchar chEnd ) const;
	/// 0x08d84aee
	FStringW Extract( int& iStart, wchar chBegin, wchar chEnd ) const;
	/// inline
	void SpanIncluding( FStringW & szDest, LPCWSTR lpszCharSet, int iStart = 0 ) const;
	/// inline
	void SpanExcluding( FStringW & szDest, LPCWSTR lpszCharSet, int iStart = 0 ) const;
	/// 0x08d84c3c
	void Extract( FStringW & szDest, int& iStart, LPCWSTR lpszCharSet ) const;
	/// 0x08d84cce
	void Extract( FStringW & szDest, int& iStart, wchar chEnd ) const;
	/// 0x08d84d34
	void Extract( FStringW & szDest, int& iStart, wchar chBegin, wchar chEnd ) const;

	// Path extraction
	/// 0x08d84de4
	FStringW ExtractDriveLetter( void ) const;
	/// 0x08d84f86
	FStringW ExtractFileDir( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d8519e
	FStringW ExtractFileExtension( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d8536c
	FStringW ExtractFileName( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d855b0
	FStringW ExtractFilePath( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d857d4
	FStringW ExtractFileTitle( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d84f26
	void ExtractDriveLetter( FStringW & szDest ) const;
	/// 0x08d8511e
	void ExtractFileDir( FStringW & szDest, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d852d8
	void ExtractFileExtension( FStringW & szDest, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d8552a
	void ExtractFileName( FStringW & szDest, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d85746
	void ExtractFilePath( FStringW & szDest, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// 0x08d85956
	void ExtractFileTitle( FStringW & szDest, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// inline
	void ReplaceDriveLetter( LPCWSTR lpszNewDrive, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void ReplaceFileDir( LPCWSTR lpszNewPath, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void ReplaceFileExtension( LPCWSTR lpszNewExt, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void ReplaceFileName( LPCWSTR lpszNewName, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void ReplaceFilePath( LPCWSTR lpszNewPath, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void ReplaceFileTitle( LPCWSTR lpszNewTitle, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void SplitPath( FStringW& strDriveLetter, FStringW& strFileDir, FStringW& strFileName, FStringW& strFileExt, wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W ) const;
	/// inline
	void StandardizePath( bool bChangeCase, bool bTerminate );
	/// inline
	void StripDriveLetter( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void StripFileDir( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void StripFileExtension( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void StripFileName( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void StripFilePath( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );
	/// inline
	void StripFileTitle( wchar chPathDelimiter = DEFAULT_PATH_DELIMITER_W );

	// Upper/Lower/Reverse conversion
	/// inline
	void MakeUpper( void );
	/// inline
	void MakeLower( void );
	/// inline
	void MakeReverse( void );

	// Trimming whitespace
	/// 0x08d85a32
	void TrimRight( void );
	/// 0x08d85a96
	void TrimRight( wchar chTarget );
	/// 0x08d85af4
	void TrimRight( LPCWSTR lpszTargets );
	/// 0x08d85b78
	void TrimLeft( void );
	/// 0x08d85be4
	void TrimLeft( wchar chTarget );
	/// 0x08d85c4a
	void TrimLeft( LPCWSTR lpszTargets );
	/// inline
	void Trim( void );
	/// inline
	void Trim( wchar chTarget );
	/// inline
	void Trim( LPCWSTR lpszTargets );
	/// inline
	void TrimWhitespace( void );
	/// 0x08d85ce6
	void Truncate( int iNewLength );

	// Advanced manipulation
	/// 0x08d85d24
	int Delete( int iIndex, int iCount = 1 );
	/// 0x08d85db8
	int Delete( LPCWSTR lpszSource );
	/// 0x08d85ea6
	int Insert( int iIndex, wchar ch );
	/// 0x08d85f14
	int Insert( int iIndex, LPCWSTR lpsz );
	/// 0x08d85fc4
	int Remove( wchar chRemove );
	/// 0x08d8604a
	int Remove( LPCWSTR lpszRemove );
	/// 0x08d8610e
	int RemoveRepeated( wchar chRemove );
	/// 0x08d861b8
	int Replace( wchar chOld, wchar chNew );
	/// 0x08d861ec
	int Replace( LPCWSTR lpszOld, LPCWSTR lpszNew );
	/// inline
	int Replace( wchar chOld, LPCWSTR lpszNew );
	/// inline
	int Replace( LPCWSTR lpszOld, wchar chNew );
	/// 0x08d86418
	int Wrap( int iMaxLineLength );

	// Searching
	/// 0x08d84a9c
	int Find( wchar ch, int iStart = 0 ) const;
	/// 0x08d85e54
	int Find( LPCWSTR lpszSub, int iStart = 0 ) const;
	/// 0x08d84a4a
	int FindOneOf( LPCWSTR lpszCharSet, int iStart = 0 ) const;
	/// 0x08d850aa
	int ReverseFind( wchar ch, int iStart = -1 ) const;
	/// 0x08d86648
	int FindCount( wchar ch, int iStart = 0 ) const;
	/// 0x08d8668c
	int FindToken( LPCWSTR lpszToken, int iStart = 0 ) const;
	/// 0x08d86736
	int FindNextWord( int iStart = 0 ) const;
	/// 0x08d867b8
	int FindPrevWord( int iStart = -1 ) const;

	// Simple formatting
	/// 0x08d86848
	void __cdecl Format( LPCWSTR lpszFormat, ... );
	/// 0x08d8687e
	void __cdecl Format( uint uiFormatID, ... );			// from custom (fxs) string table
#ifdef		_WINPC
	/// 0x08d86972
	void __cdecl FormatResource( uint uiFormatID, ... );	// from microsoft string table in .rc file
#endif	//	_WINPC
	/// 0x08d86ae4
	void FormatSize( uint uiSize );
	/// 0x08d83d24
	void FormatV( LPCWSTR lpszFormat, va_list vaArgList );
	/// 0x08d86c72
	void TranslateEscapeCodes( void );

	// Load from string resources
	/// absent
	bool LoadString( uint uiID );								// from custom (fxs) string table
#ifdef	_WINPC
	/// 0x08d86a1e
	bool LoadResource( uint uiID, HINSTANCE hInstance = NULL );	// from microsoft string table in .rc file
#endif	//_WINPC

	// String copying
	/// 0x08d841e8
	void Copy( int iLength, LPCWSTR lpszSource, int iFirst = 0 );
	/// 0x08d8771c
	void CopySystemLocale(LPCSTR lpszSource);
	/// 0x08d877b4
	void CopyUTF8(LPCSTR lpszSource);
	/// 0x08d8784c
	void CopyToUTF8(FStringA* pkOutput) const;
	/// 0x08d87976
	void CopyToSystemLocale(FStringA* pkOutput) const;

	// String concatenation
	/// inline
	void Concat( int iLength, LPCWSTR lpszSource, int iFirst = 0 );
	/// inline
	void Concat( LPCWSTR lpszSource );

	// Numeric conversion
	/// 0x08d8750c
	int ConvertToInt( int iRadix = 10, bool bReadHex = true ) const;
	/// 0x08d87aa0
	float ConvertToFloat( int iRadix = 10 ) const;
	/// inline
	uint Hash( void ) const;

	// Encryption
	/// 0x08d87bde
	void Encrypt( void );
	/// 0x08d87c10
	void Decrypt( void );

	/// inline
	void SetFixedBuffer( FStringWData* pkData );

	/// 0x08d876d4
	void TakeBufferFrom( FStringW& szDonor );

	// Static Methods:
	//---------------
	/// inline
	static int SafeStrlen( LPCWSTR lpsz );
	/// inline
	static int SafeStrcmp( LPCWSTR lpszStr1, LPCWSTR lpszStr2 );
	/// inline
	static int SafeStricmp( LPCWSTR lpszStr1, LPCWSTR lpszStr2 );
	/// 0x08d87c40
	static WCHAR * SafeStrDup( LPCWSTR lpsz );
		
	/// inline
	static size_t SizeofFStringData( void );

	/// inline
	static FStringHashType Hash( LPCWSTR lpsz );

	/// 0x08d87898
	static void CopyToUTF8(LPCWSTR pszSource, FStringA* pkOutput);
	/// 0x08d879c2
	static void CopyToSystemLocale(LPCWSTR pszSource, FStringA* pkOutput);

protected:
	// Methods:
	//--------
	/// inline
	bool IsLocked( void ) const;
	/// inline
	bool IsFixed(  void ) const;
#ifdef		_NDS
	bool IsLarge(  void ) const;
	bool IsStatic( void ) const;
#endif	//	_NDS

	/// 0x08d846ea
	bool MatchesOnePattern( LPCWSTR lpszPattern, bool bCaseSensitive = false ) const;
	
#ifndef		_NDS
	/// inline
	FStringWData* GetData( void ) const;
#endif	//	_NDS

	/// 0x08d84ede
	int FindDriveLetter( void ) const;
	
	/// 0x08d87606
	LPWSTR GetBuffer( int iMinBufLength = 0 );

	/// inline
	void Init( void );
	/// inline
	void LockBuffer( void );
	/// 0x08d87656
	void ReleaseBuffer( size_t iNewLength = -1 );
	/// 0x08d843ca
	void SetLength( size_t iLength );
	/// inline
	void DecLength( size_t iLength );
	/// inline
	void UnlockBuffer( void );
	/// absent
	void SetCharCount(  size_t iLength );
	/// inline
	int  GetAllocLength( void ) const;

#ifndef		_NDS
	/// 0x08d87cee
	void Reallocate( size_t iAllocSize );
	/// inline
	void Release( FStringWData* pkData );
#endif	//	!_NDS

	/// absent
	void* GetAllocPtr( void );
	/// 0x08d87ca0
	void  AllocBuffer( size_t nChars );

#ifdef		_NDS
		  uint& Header( int index );		// Header accessors
	const uint& Header( int index ) const;
#endif	//	_NDS

private:
	// Members:
	//--------
	// WARNING: The m_pszString MUST be the ONLY member of this class!!!
	// Actual string data
	LPWSTR m_pszString; ///< +0x0
	
	friend class FStringHashPalette; //  please don't hate me....
	// seed: in Civ5XP, not in SDK
public:
	/// 0x08d86932
	UNK_RET LoadStringA(unsigned int a1);
};

// Access to the common initializer data for an empty string
#if !defined(FXS_IS_DLL)
extern "C" 
{
	/// 0x08d83cd2
	__declspec(dllexport) extern FStringW::FStringWData* FStringW_GetStringInitData();
}
#else
FStringW::FStringWData* FStringW_GetStringInitData();
#endif

// Friendly helpers
/// inline
FStringW operator+( wchar ch, const FStringW& s2 );
/// inline
FStringW operator+( LPCWSTR s1, const FStringW& s2 );

/// inline
FStringW operator-( wchar ch, const FStringW& s2 );
/// inline
FStringW operator-( LPCWSTR s1, const FStringW& s2 );

/// inline
bool operator==( LPCWSTR s1, const FStringW& s2 );
/// inline
bool operator!=( LPCWSTR s1, const FStringW& s2 );

/// inline
bool operator<( LPCWSTR s1, const FStringW& s2 );
/// inline
bool operator<=( LPCWSTR s1, const FStringW& s2 );
/// inline
bool operator>( LPCWSTR s1, const FStringW& s2 );
/// inline
bool operator>=( LPCWSTR s1, const FStringW& s2 );

//---------------------------------------------------------------------------------------
// FString
//
//	Represents either an ANSI or UNICODE string depending on the current compilation
#ifndef		_UNICODE
	typedef FStringA FString;
	typedef FStringA::FStringAHashTraits		FStringHashTraits;
	typedef	FStringA::FStringAHashTraitsNoCase	FStringHashTraitsNoCase;
	#ifndef		_NDS
	#define		FStringFixedBufferObj	FStringA::FStringAFixedBuffer
	#else	//	!_NDS
	#define		FStringFixedBufferObj	FStringSTKA
	#define		FStringSTK				FStringSTKA
	#endif	//	!_NDS
#else	//	_UNICODE
	typedef FStringW FString;
	typedef FStringW::FStringWHashTraits		FStringHashTraits;
	typedef	FStringW::FStringWHashTraitsNoCase	FStringHashTraitsNoCase;
	#ifndef		_NDS
	#define		FStringFixedBufferObj	FStringW::FStringWFixedBuffer
	#else	//	!_NDS
	#define		FStringFixedBufferObj	FStringSTKW
	#define		FStringSTK				FStringSTKW
	#endif	//	!_NDS
#endif	//	_UNICODE

#ifdef		_NDS
	#define		FStringFixedBuffer					FStringSTK	// legacy
	#define		FStringSTKA( varname, size )  char varname##_buf[ (4 + size*sizeof( char) + 31) & ~31 ];  FStringA varname( sizeof(varname##_buf), varname##_buf )
	#define		FStringSTKW( varname, size )  char varname##_buf[ (4 + size*sizeof(wchar) + 31) & ~31 ];  FStringW varname( sizeof(varname##_buf), varname##_buf )
#else	//	_NDS
	#define		FStringFixedBuffer( varname, size ) FString varname; FStringFixedBufferObj<size> FStringFixedBuffer_##varname(varname); varname
	#define		FStringFixedBufferA( varname, size ) FStringA varname; FStringA::FStringAFixedBuffer<size> FStringFixedBuffer_##varname(varname); varname
	#define		FStringFixedBufferW( varname, size ) FStringW varname; FStringW::FStringWFixedBuffer<size> FStringFixedBuffer_##varname(varname); varname
#endif	//	_NDS

#ifdef  _NDS
#  define	MAX_PATH  252
#endif

#include	"FirePlace/FireWorks/FStringW.inl"
#include	"FirePlace/FireWorks/FStringA.inl"

#ifndef		LPCTSTR
#ifndef		_UNICODE
#define		LPCTSTR	LPCSTR
#else	//	_UNICODE
#define		LPCTSTR	LPCWSTR
#endif	//	_UNICODE
#endif	//	LPCTSTR

#ifndef		LPTSTR
#ifndef		_UNICODE
#define		LPTSTR	LPSTR
#else	//	_UNICODE
#define		LPTSTR	LPWSTR
#endif	//	_UNICODE
#endif	//	LPCTSTR

// Helper types
typedef FString* 				  FStringPtr;
typedef FArray< FStringPtr, c_eMPoolTypeFString > FStringPtrArray;

// Global data
/// 0x0ff8b530
extern const FString g_strEMPTY;	// A global empty string

typedef std::vector<std::string> StdStringVector;

#endif	//	FSTRING_H

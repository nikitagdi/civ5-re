/// origin binary
#ifndef _FIGAMEINIPARSER_H
#define _FIGAMEINIPARSER_H

#include "FirePlace/FireWorks/FString.h"

#include "FirePlace/FireWorks/FFileDefines.h"

// Interface class to the game ini parser
/// size 0x4
/// vtable ?
class FIGameIniParser
{
public:

	/// inline
	virtual ~FIGameIniParser() {}

	/// pure
	virtual bool GetKeyValue(LPCTSTR szGroupKey, LPCTSTR szKeyValue, FString* pValue, LPCTSTR szKeyDesc=NULL, const char* keyDefaultVal=NULL) = 0;
	/// pure
	virtual bool GetKeyValue(LPCTSTR szGroupKey, LPCTSTR szKeyValue, int* pValue, LPCTSTR szKeyDesc=NULL, int keyDefaultVal = 0) = 0;
	/// pure
	virtual bool GetKeyValue(LPCTSTR szGroupKey, LPCTSTR szKeyValue, uint* pValue, LPCTSTR szKeyDesc=NULL, uint keyDefaultVal = 0) = 0;
	/// pure
	virtual bool GetKeyValue(LPCTSTR szGroupKey, LPCTSTR szKeyValue, float* pValue, LPCTSTR szKeyDesc=NULL, float keyDefaultVal = 0.0f) = 0;

	// Re-write the INI file
	/// pure
	virtual bool Write( void ) = 0;
	/** Write the contents only if the dirty flag is set. 
		@return true if settings were written.  The dirty flag will be reset to false. */
	/// pure
	virtual bool WriteIfDirty() = 0;
	/// pure
	virtual void EnableAutoWriting( bool bEnable = true ) = 0;
	/** Return true if the settings are dirty. */
	/// pure
	virtual bool IsDirty() const = 0;
	/** Return true the ini settings were corrupted and have been reset to their defaults. */
	/// pure
	virtual bool WarnCorrupted() const = 0;
	/** Clear the corrupted warning flag */
	/// pure
	virtual void ClearCorruptedWarning() = 0;
	/// pure
	virtual FileErr GetLastFileErr() = 0;
	/** Return true if the parser has been initialized. */
	/// pure
	virtual bool IsInitialized() = 0;
};

#endif // _FIGAMEINIPARSER_H

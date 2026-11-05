/// origin guess
// $Header
//------------------------------------------------------------------------------
#pragma once 

#include "Civ5/Src/CvGameDatabase/Database.h"

#include <unordered_map>

namespace Database{

/// size 0x18
class ResultsCache
{

public:
	/// 0x08b9aea0
	CvGameDatabaseAPI ResultsCache(Connection& db);
	/// 0x08b9aece
	CvGameDatabaseAPI ~ResultsCache();

	//! Clear stored results
	/// 0x08b9aefa
	CvGameDatabaseAPI void Clear();
	/// 0x08b9af3e
	CvGameDatabaseAPI void Clear(const std::string& strKey);

	//! Retrieves the associated connection object.
	/// absent
	CvGameDatabaseAPI Database::Connection& Connection();

	//! Retrieves a cached result given an arbitrary string key
	/// 0x08b9af86
	CvGameDatabaseAPI Results* Get(const std::string& strKey);

	//! Prepares a cached result given an arbitrary string key
	/// 0x08b9afe0
	CvGameDatabaseAPI Results* Prepare(const std::string& strKey, const char* szStmt, int lenStmt = -1);

	//! Returns the most recent database error message.
	/// 0x08b9b092
	CvGameDatabaseAPI const char* ErrorMessage() const;

private:
	/// absent
	ResultsCache(const ResultsCache&);	//Stub to prevent compiler generated function.
	/// absent
	ResultsCache& operator=(const ResultsCache&);	//Stub to prevent compiler generated function.

	Database::Connection& m_db; ///< +0x0

	typedef std::unordered_map<std::string, Results*> ResultsMap;
	ResultsMap m_storedResults; ///< +0x4
	// seed: in Civ5XP, not in SDK
public:
	/// 0x08b9afd8
	UNK_RET ConnectionObject();
};


}	//namespace Database


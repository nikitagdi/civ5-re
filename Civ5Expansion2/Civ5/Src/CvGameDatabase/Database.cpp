/// origin guess
#include <Windows.h>
#include <sqlite3.h>
#include <sstream>
#include <string.h>
#include <stdio.h>

#include "Civ5/Src/CvGameDatabase/Database.h"
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"

/// 0x08b94170
Database::Connection::Connection():
	m_db(0)
{
	if (!ms_bMemoryManagerInitialized)
	{
		InitMemoryManagement();
		sqlite3_initialize();
	}
}

/// 0x08b9444c
Database::Connection::~Connection()
{
	if (m_db != 0)
	{
		Close();
	}
}

/// 0x08b943d4
bool Database::Connection::Open(const char* filename, int flags)
{
	const char* szFilename = (filename != 0) ? filename : ":memory:";
	int status = sqlite3_open_v2(szFilename, &m_db, flags, 0);
	if (Report(status))
	{
		sqlite3_busy_handler(m_db, Fun_08b945c4, this);
		return true;
	}
	return false;
}

/// 0x08b9466c
void Database::Connection::BeginTransaction()
{
	Execute("SAVEPOINT _;", -1);
}

/// 0x08b94772
void Database::Connection::EndTransaction()
{
	Execute("RELEASE _;", -1);
}

/// 0x08b946a0
bool Database::Connection::Execute(const char* szCommand, int lenCommand) const
{
	sqlite3_stmt* pkStatement = 0;
	bool bSuccess = false;

	if (Report(sqlite3_prepare_v2(m_db, szCommand, lenCommand, &pkStatement, 0)) && pkStatement != 0)
	{
		int iResult;
		do
		{
			iResult = sqlite3_step(pkStatement);
		}
		while (iResult == SQLITE_OK || iResult == SQLITE_ROW);

		Report(sqlite3_finalize(pkStatement));
		bSuccess = true;

		if (iResult != SQLITE_DONE)
		{
			const char* szError = 0;
			if (m_db != 0)
			{
				szError = sqlite3_errmsg(m_db);
			}
			if (m_pkDatabaseLogger.get() != 0)
			{
				m_pkDatabaseLogger->Log(szError, IDatabaseLogger::TYPE_ERROR);
			}
			bSuccess = false;
		}
	}
	return bSuccess;
}

/// 0x08b9499a
int Database::Connection::Count(const char* tableName, bool bCache)
{
	std::string strTable(tableName);

	std::unordered_map<std::string, int>::iterator itValue = m_hshCountValues.find(strTable);
	if (itValue != m_hshCountValues.end() && sqlite3_changes(m_db) == 0)
	{
		return itValue->second;
	}

	sqlite3_stmt* pkStatement;
	bool bWasCached;

	std::unordered_map<std::string, sqlite3_stmt*>::iterator itStatement = m_hshCountStatements.find(strTable);
	if (itStatement == m_hshCountStatements.end())
	{
		char szSQL[256];
		memset(szSQL, 0, sizeof(szSQL));
		strcpy(szSQL, "select count(*) from ");
		strcat(szSQL, tableName);

		int status = sqlite3_prepare_v2(m_db, szSQL, sizeof(szSQL), &pkStatement, 0);
		if (!Report(status))
		{
			return -1;
		}
		if (bCache)
		{
			m_hshCountStatements[strTable] = pkStatement;
		}
		bWasCached = false;
	}
	else
	{
		pkStatement = itStatement->second;
		Report(sqlite3_reset(pkStatement));
		bWasCached = true;
	}

	int iCount = -1;
	if (sqlite3_step(pkStatement) == SQLITE_ROW)
	{
		iCount = sqlite3_column_int(pkStatement, 0);
		m_hshCountValues[strTable] = iCount;

		if (bWasCached || bCache)
		{
			Report(sqlite3_reset(pkStatement));
			Report(sqlite3_clear_bindings(pkStatement));
		}
		else
		{
			Report(sqlite3_finalize(pkStatement));
		}
	}
	return iCount;
}

/// 0x08b94c26
void Database::Connection::ClearCountCache()
{
	for (std::unordered_map<std::string, sqlite3_stmt*>::iterator it = m_hshCountStatements.begin(); it != m_hshCountStatements.end(); ++it)
	{
		Report(sqlite3_finalize(it->second));
	}
	m_hshCountStatements.clear();
	m_hshCountValues.clear();
}

/// 0x08b94c7c
bool Database::Connection::SelectAt(Results& kResults, const char* tableName, int rowID)
{
	Command kCommand(this, 0);
	char szSQL[1024];
	memset(szSQL, 0, sizeof(szSQL));

	sqlite3_snprintf(sizeof(szSQL), szSQL, "select %s from %q where ROWID = %i LIMIT 1", kResults.GetColumns(), tableName, rowID);
	if (AcquireCommand(kCommand, szSQL, -1))
	{
		return kResults.SetCommand(kCommand);
	}
	ReleaseCommand(kCommand);
	return false;
}

/// 0x08b94e96
bool Database::Connection::SelectAt(Results& kResults, const char* tableName, const char* columnName, const char* value)
{
	Command kCommand(this, 0);
	char szSQL[1024];
	memset(szSQL, 0, sizeof(szSQL));

	const char* szLimit = kResults.SingleQuery() ? "LIMIT 1" : "";
	sqlite3_snprintf(sizeof(szSQL), szSQL, "select %s from %q where %s = %Q %s", kResults.GetColumns(), tableName, columnName, value, szLimit);
	if (AcquireCommand(kCommand, szSQL, -1))
	{
		return kResults.SetCommand(kCommand);
	}
	ReleaseCommand(kCommand);
	return false;
}

/// 0x08b94f92
bool Database::Connection::SelectAt(Results& kResults, const char* tableName, const char* columnName, int value)
{
	Command kCommand(this, 0);
	char szSQL[1024];
	memset(szSQL, 0, sizeof(szSQL));

	const char* szLimit = kResults.SingleQuery() ? "LIMIT 1" : "";
	sqlite3_snprintf(sizeof(szSQL), szSQL, "select %s from %q where %s = %i %s", kResults.GetColumns(), tableName, columnName, value, szLimit);
	if (AcquireCommand(kCommand, szSQL, -1))
	{
		return kResults.SetCommand(kCommand);
	}
	ReleaseCommand(kCommand);
	return false;
}

/// 0x08b9518e
bool Database::Connection::SelectAll(Results& kResults, const char* tableName)
{
	Command kCommand(this, 0);
	char szSQL[1024];
	memset(szSQL, 0, sizeof(szSQL));

	const char* szLimit = kResults.SingleQuery() ? "LIMIT 1" : "";
	sqlite3_snprintf(sizeof(szSQL), szSQL, "select %s from %q %s", kResults.GetColumns(), tableName, szLimit);
	if (AcquireCommand(kCommand, szSQL, -1))
	{
		return kResults.SetCommand(kCommand);
	}
	ReleaseCommand(kCommand);
	return false;
}

/// 0x08b95274
bool Database::Connection::SelectWhere(Results& kResults, const char* tableName, const char* condition)
{
	Command kCommand(this, 0);
	char szSQL[1024];
	memset(szSQL, 0, sizeof(szSQL));

	const char* szLimit = kResults.SingleQuery() ? "LIMIT 1" : "";
	sqlite3_snprintf(sizeof(szSQL), szSQL, "select %s from %q where %s %s", kResults.GetColumns(), tableName, condition, szLimit);
	if (AcquireCommand(kCommand, szSQL, -1))
	{
		return kResults.SetCommand(kCommand);
	}
	ReleaseCommand(kCommand);
	return false;
}

/// 0x08b95364
bool Database::Connection::Execute(Results& kResults, const char* szCommand, int lenCommand) const
{
	Command kCommand(this, 0);
	if (AcquireCommand(kCommand, szCommand, lenCommand))
	{
		return kResults.SetCommand(kCommand);
	}
	return false;
}

/// 0x08b953fa
const char* Database::Connection::ErrorMessage() const
{
	if (m_db == 0)
	{
		return 0;
	}
	return sqlite3_errmsg(m_db);
}

/// 0x08b956be
const char* Database::Connection::CalculateMemoryStats()
{
	int iUnused;
	int iMallocCur, iMallocMax;
	int iPageCacheUsedCur, iPageCacheUsedMax;
	int iScratchUsedCur, iScratchUsedMax;
	int iMallocSizeMax, iPageCacheSizeMax, iScratchSizeMax;
	int iPageCacheOverflowCur, iPageCacheOverflowMax;
	int iScratchOverflowCur, iScratchOverflowMax;
	int iLookasideCur, iLookasideMax;

	sqlite3_status(SQLITE_STATUS_MEMORY_USED, &iMallocCur, &iMallocMax, 0);
	sqlite3_status(SQLITE_STATUS_PAGECACHE_USED, &iPageCacheUsedCur, &iPageCacheUsedMax, 0);
	sqlite3_status(SQLITE_STATUS_SCRATCH_USED, &iScratchUsedCur, &iScratchUsedMax, 0);
	sqlite3_status(SQLITE_STATUS_MALLOC_SIZE, &iUnused, &iMallocSizeMax, 0);
	sqlite3_status(SQLITE_STATUS_PAGECACHE_SIZE, &iUnused, &iPageCacheSizeMax, 0);
	sqlite3_status(SQLITE_STATUS_SCRATCH_SIZE, &iUnused, &iScratchSizeMax, 0);
	sqlite3_status(SQLITE_STATUS_PAGECACHE_OVERFLOW, &iPageCacheOverflowCur, &iPageCacheOverflowMax, 0);
	sqlite3_status(SQLITE_STATUS_SCRATCH_OVERFLOW, &iScratchOverflowCur, &iScratchOverflowMax, 0);
	sqlite3_db_status(m_db, SQLITE_DBSTATUS_LOOKASIDE_USED, &iLookasideCur, &iLookasideMax, 0);

	int iStatements = 0;
	for (sqlite3_stmt* pkStatement = sqlite3_next_stmt(m_db, 0); pkStatement != 0; pkStatement = sqlite3_next_stmt(m_db, pkStatement))
	{
		++iStatements;
	}

	std::stringstream ss;
	ss << std::endl;
	ss << "-- SQLite Memory Statistics --" << std::endl;
	ss << "Memory Usage:" << std::endl;
	ss << "\t\t[Cur]\t\t[Max]" << std::endl;
	ss << "Malloc:\t\t" << iMallocCur << "\t\t" << iMallocMax << std::endl;
	ss << "PageCache:\t" << iPageCacheUsedCur << "\t\t" << iPageCacheUsedMax << std::endl;
	ss << "LookAside:\t" << iLookasideCur << "\t\t" << iLookasideMax << std::endl;
	ss << "Scratch:\t" << iScratchUsedCur << "\t\t" << iScratchUsedMax << std::endl;
	ss << std::endl;
	ss << "Static Buffer Overflows:" << std::endl;
	ss << "\t\t[TooLarge]\t[NoSpace]" << std::endl;
	ss << "PageCache:\t" << iPageCacheOverflowCur << "\t\t" << iPageCacheOverflowMax << std::endl;
	ss << "Scratch:\t" << iScratchOverflowCur << "\t\t" << iScratchOverflowMax << std::endl;
	ss << std::endl;
	ss << "Largest Allocations:" << std::endl;
	ss << "Malloc:\t\t" << iMallocSizeMax << std::endl;
	ss << "PageCache:\t" << iPageCacheSizeMax << std::endl;
	ss << "Scratch:\t" << iScratchSizeMax << std::endl;
	ss << std::endl;
	ss << "Prepared Statements:" << std::endl;
	ss << "Current:\t\t" << iStatements << std::endl;
	ss << "------------------------------";

	m_strMemoryStats = ss.str();
	if (m_pkDatabaseLogger.get() != 0)
	{
		m_pkDatabaseLogger->Log(m_strMemoryStats.c_str(), IDatabaseLogger::TYPE_MESSAGE);
	}
	return m_strMemoryStats.c_str();
}

/// 0x08b967a2
bool Database::Connection::ValidateFKConstraints(bool bAllowNULL) const
{
	char szForeignKeys[512];
	char szCheck[512];
	char szFailure[512];
	char szInvalid[1024];
	sqlite3_stmt* pkTables;
	sqlite3_stmt* pkKeys;
	sqlite3_stmt* pkCheck;
	bool bValid;

	if (m_pkDatabaseLogger.get() != 0)
	{
		m_pkDatabaseLogger->Log("Validating Foreign Key Constraints...", IDatabaseLogger::TYPE_MESSAGE);
	}

	if (sqlite3_prepare_v2(m_db, "select name from sqlite_master where type = 'table'", -1, &pkTables, 0) != 0)
	{
		return false;
	}

	bValid = true;
	while (sqlite3_step(pkTables) == SQLITE_ROW)
	{
		const char* szTable = (const char*)sqlite3_column_text(pkTables, 0);
		sprintf_s(szForeignKeys, sizeof(szForeignKeys), "pragma foreign_key_list(%s);", szTable);
		if (sqlite3_prepare_v2(m_db, szForeignKeys, -1, &pkKeys, 0) != 0)
		{
			sqlite3_finalize(pkTables);
			bValid = false;
			goto Finish;
		}

		while (sqlite3_step(pkKeys) == SQLITE_ROW)
		{
			const char* szReferencedTable = (const char*)sqlite3_column_text(pkKeys, 2);
			const char* szFromColumn = (const char*)sqlite3_column_text(pkKeys, 3);
			const char* szToColumn = (const char*)sqlite3_column_text(pkKeys, 4);

			sprintf_s(szCheck, sizeof(szCheck), "select \"%s\" from %s where %s not null and %s not in (select \"%s\" from %s);",
				szFromColumn, szTable, szFromColumn, szFromColumn, szToColumn, szReferencedTable);
			if (sqlite3_prepare_v2(m_db, szCheck, -1, &pkCheck, 0) != 0)
			{
				sprintf_s(szFailure, sizeof(szFailure), "Failure to check references for foreign key %s(%s).", szTable, szFromColumn);
				if (m_pkDatabaseLogger.get() != 0)
				{
					m_pkDatabaseLogger->Log(szFailure, IDatabaseLogger::TYPE_ERROR);
				}
				if (m_pkDatabaseLogger.get() != 0)
				{
					m_pkDatabaseLogger->Log(sqlite3_errmsg(m_db), IDatabaseLogger::TYPE_ERROR);
				}
				sqlite3_finalize(pkTables);
				sqlite3_finalize(pkKeys);
				bValid = false;
				goto Finish;
			}

			if (sqlite3_step(pkCheck) == SQLITE_ROW)
			{
				do
				{
					const char* szMissing = (const char*)sqlite3_column_text(pkCheck, 0);
					sprintf_s(szInvalid, sizeof(szInvalid), "Invalid Reference on %s.%s - \"%s\" does not exist in %s",
						szTable, szFromColumn, szMissing, szReferencedTable);
					if (m_pkDatabaseLogger.get() != 0)
					{
						m_pkDatabaseLogger->Log(szInvalid, IDatabaseLogger::TYPE_ERROR);
					}
				}
				while (sqlite3_step(pkCheck) == SQLITE_ROW);
				bValid = false;
			}
			sqlite3_finalize(pkCheck);
		}
		sqlite3_finalize(pkKeys);
	}
	sqlite3_finalize(pkTables);

Finish:
	if (m_pkDatabaseLogger.get() != 0)
	{
		m_pkDatabaseLogger->Log(bValid ? "Passed Validation." : "Failed Validation.", IDatabaseLogger::TYPE_MESSAGE);
	}
	return bValid;
}

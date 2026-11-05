/// origin guess

#pragma once

#ifndef CvGameDatabaseAPI
#define CvGameDatabaseAPI __declspec(dllimport)
#endif
#include <string>
#include <stddef.h>
#include <hash_map>

#include <fstream>
#include <unordered_map>

#include "Civ5/Src/CvGameDatabase/IDatabaseLogger.h"

#define DB_NUM_PAGES			5000
#define DB_NUM_THREADS			1
#define DB_PAGECACHE_SIZE		1168

//SQLite Forward declarations
struct sqlite3;
struct sqlite3_stmt;


namespace Database{

	class Results;		//Forward declaration.
	class Connection;	//Forward declaration.

	//!Error reporting mechanism used by all Database classes.
	/// absent
	static void Error(const char* szErrorMsg);

	//! Open Flags used for opening a database.
	enum OpenFlags
	{
		//! Opens the database as read only.
		OPEN_READONLY	= 0x00000001,

		//! Opens the database for read/write access.
		OPEN_READWRITE	= 0x00000002,

		//! Creates a new database.
		OPEN_CREATE		= 0x00000004,

		//! Disables thread safety.
		OPEN_NOMUTEX	= 0x00008000,

		//! Enables thread safety.
		OPEN_FULLMUTEX	= 0x00010000,
	};

	/// 0x08b945c4
	int Fun_08b945c4(void* pArg, int iAttempts);

	//! Prepared database command.
	//! Currently just a container but will later contain
	//! methods useful for binding custom parameters to the command.
	/// size 0x8
	class Command
	{
	public:
		/// inline
		Command(const Connection* connection = 0, sqlite3_stmt* stmt = 0):
			connection(connection), stmt(stmt)
		{}

		const Connection* connection; ///< +0x0
		sqlite3_stmt* stmt; ///< +0x4
	};
	
	//! The main database class.
	//! Represents a single database.
	/// size 0x4c
	class Connection
	{
	public:
		/// 0x08b94170
		CvGameDatabaseAPI Connection();
		/// 0x08b942e0
		CvGameDatabaseAPI Connection(const char* filename, int flags = 0);
		/// 0x08b9444c
		CvGameDatabaseAPI ~Connection();

		/// 0x08b943d4
		CvGameDatabaseAPI bool Open(const char* filename, int flags = 0);
		/// 0x08b944ec
		CvGameDatabaseAPI void Close();
		
		//Transactions
		/// 0x08b9466c
		CvGameDatabaseAPI void BeginTransaction();
		/// 0x08b94772
		CvGameDatabaseAPI void EndTransaction();
		/// 0x08b947a6
		CvGameDatabaseAPI void RollbackTransaction();

		/// 0x08b947da
		CvGameDatabaseAPI void BeginDeferredTransaction();
		/// 0x08b9480e
		CvGameDatabaseAPI void BeginImmediateTransaction();
		/// 0x08b94842
		CvGameDatabaseAPI void BeginExclusiveTransaction();
		/// 0x08b94876
		CvGameDatabaseAPI void CommitTransaction();

		//SavePoints
		/// 0x08b948aa
		CvGameDatabaseAPI bool SetSavePoint(const char* savePoint);
		/// 0x08b94922
		CvGameDatabaseAPI bool RollbackToSavePoint(const char* savePoint);

		//Row counts
		/// 0x08b9499a
		CvGameDatabaseAPI int Count(const char* tableName, bool bCache = true);

		//! Clear all cached values and prepared statements from Count().
		/// 0x08b94c26
		CvGameDatabaseAPI void ClearCountCache();

		//Selection
		/// 0x08b94c7c
		CvGameDatabaseAPI bool SelectAt(Results& kResults, const char* tableName, int rowID);
		/// 0x08b94e96
		CvGameDatabaseAPI bool SelectAt(Results& kResults, const char* tableName, const char* columnName, const char* value);
		/// 0x08b94f92
		CvGameDatabaseAPI bool SelectAt(Results& kResults, const char* tableName, const char* columnName, int value);
		/// 0x08b9508e
		CvGameDatabaseAPI bool SelectAt(Results& kResults, const char* tableName, const char* columnName, double value);

		/// 0x08b9518e
		CvGameDatabaseAPI bool SelectAll(Results& kResults, const char* tableName);
		
		/// 0x08b95274
		CvGameDatabaseAPI bool SelectWhere(Results& kResults, const char* tableName, const char* condition);

		//! Executes a database command.
		/// 0x08b95364
		CvGameDatabaseAPI bool Execute(Results& kResults, const char* szCommand, int lenCommand = -1) const;
		/// 0x08b946a0
		CvGameDatabaseAPI bool Execute(const char* szCommand, int lenCommand = -1) const;

		//! Executes multiple statements that are combined.
		/// 0x08b95426
		CvGameDatabaseAPI bool ExecuteMultiple(const char* szCommands, int lenCommands = -1) const;

		//! Retrieves the total number of changes since database was opened.
		/// 0x08b9553e
		CvGameDatabaseAPI int TotalChanges() const;

		//! Analyzes the database and creates a stats table.
		/// 0x08b9556a
		CvGameDatabaseAPI void Analyze() const;

		//! Shrinks database by removing unused allocated space.
		/// 0x08b955c2
		CvGameDatabaseAPI void Vacuum() const;

		//! Statement Information
		/// 0x08b96b3e
		CvGameDatabaseAPI int StatementCount() const;

		//! Busy Timeout
		/// 0x08b96b86
		CvGameDatabaseAPI bool SetBusyTimeout(int ms);

		/// 0x08b96bc2
		CvGameDatabaseAPI const char* StatementSQL(int index) const;

		//! Used for error reporting.  Returns false on error.
		/// 0x08b94570
		bool Report(int status) const;

		//! Push notification logging methods
		/// 0x08b9561a
		CvGameDatabaseAPI void SetLogger(IDatabaseLogger* pkLogger);
		/// 0x08b9564e
		CvGameDatabaseAPI void LogMessage(const char* szMessage) const;
		/// 0x08b95686
		CvGameDatabaseAPI void LogWarning(const char* szWarning) const;
		/// 0x08b953c2
		CvGameDatabaseAPI void LogError(const char* szError) const;

		//! Returns memory statistics used for logging
		/// 0x08b956be
		CvGameDatabaseAPI const char* CalculateMemoryStats();

		//! Validates foreign key constraints
		/// 0x08b967a2
		CvGameDatabaseAPI bool ValidateFKConstraints(bool bAllowNULL = true) const;

		/// 0x08b96c1c
		CvGameDatabaseAPI void* operator new(size_t tSize);
		/// 0x08b96c30
		CvGameDatabaseAPI void operator delete(void* pMem);
		
		//BEGIN INTERNAL USE ONLY
		/// 0x08b96c44
		sqlite3* GetSQLite3() const;

		//! Error message handling 
		/// 0x08b96c4c
		CvGameDatabaseAPI int ErrorCode() const;
		/// 0x08b953fa
		CvGameDatabaseAPI const char* ErrorMessage() const;

		//! Prepared statement cache management
		/// 0x08b94d50
		static bool AcquireCommand(Command& command, const char* szSQL, int lenSQL = -1);
		/// 0x08b94e32
		static void ReleaseCommand(Command& command);
		//END INTERNAL USE ONLY

	protected:
		/// 0x08b9424c
		static void InitMemoryManagement();

	private:
		//Degenerate copy and assignment
		/// absent
		Connection(const Connection&);
		/// absent
		Connection& operator=(const Connection&);

		/// 0x0ccd17ec
		static bool ms_bMemoryManagerInitialized;

		//! Pointer to sqlite3 database.
		sqlite3* m_db; ///< +0x0

		//! Statistics buffer used for logging out memory stats
		std::string m_strMemoryStats; ///< +0x4

		//! Hash map of count statements indexed by table name.
		stdext::hash_map<std::string, sqlite3_stmt*> m_hshCountStatements; ///< +0x10
		stdext::hash_map<std::string, int> m_hshCountValues; ///< +0x2c

		/// 0x0ccd17ed
		static char ms_pPageCacheBuffer[DB_PAGECACHE_SIZE * DB_NUM_PAGES];
		/// 0x0d26346d
		static char ms_pScratchBuffer[DB_PAGECACHE_SIZE * DB_NUM_THREADS * 6];	

		mutable std::auto_ptr<IDatabaseLogger> m_pkDatabaseLogger; ///< +0x48
	};
}

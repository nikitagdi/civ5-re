/// origin guess
#pragma once 

#include <lua.h>

#include "Civ5/Src/CvGameDatabase/Database.h"

#if !defined(NO_LUA_SUPPORT)

namespace Database{ namespace Scripting{

	/// size 0x1
	class Lua
	{
	public:
		//! Pushes database exposures onto the Lua stack.
		//! NOTE: dbConn must remain active until this instance is removed from Lua.
		//! On error, will utilize Lua's error reporting mechanism.
		/// 0x08b997ac
		CvGameDatabaseAPI static void PushDatabase( lua_State* L, Database::Connection& dbConn );
		
		//! Generates and pushes a database query onto the stack.
		/// 0x08b9989a
		CvGameDatabaseAPI static Database::Results* PushDatabaseQuery(lua_State* L, Database::Connection* db, const char* szSQL);

		//! Pushes a database table onto the stack.
		/// 0x08b99aa4
		CvGameDatabaseAPI static void PushDatabaseTable(lua_State* L, Database::Connection& dbConn, const char* szTableName);

		//! Pushes a single database row onto the stack.
		/// 0x08b9a402
		CvGameDatabaseAPI static void PushDatabaseRow(lua_State* L, Database::Results* pResults);

		//! Returns a table containing memory usage information.
		/// 0x08b9acfc
		/// lua ?
		CvGameDatabaseAPI static int lGetMemoryUsage( lua_State* L);

		//! Performs a deeper scan of the internal heap to obtain memory usage statistics.
		/// 0x08b9ae84
		/// lua ?
		CvGameDatabaseAPI static int lCollectMemoryUsage( lua_State* L);
		
	private:
		
		//! Generates and pushes a subtable onto the stack.
		/// 0x08b9a36c
		static void PushSubTable(lua_State* L, const char* tableName);

		//! Database Query meta-methods.
		/// 0x08b9a332
		/// lua ?
		static int lFinalizeDatabaseQuery(lua_State* L);

		//! Auxiliary method for iterating a database table.
		/// 0x08b9a81a
		/// lua ?
		static int lIterateTable(lua_State* L);

		// Database Row meta-methods.
		/// 0x08b9a680
		/// lua ?
		static int lInvalidColumnCheck(lua_State* L);
		/// 0x08b9a7ec
		/// lua ?
		static int lNewIndexDatabaseRow(lua_State* L);

		// Database Table meta-methods.
		/// 0x08b99f96
		/// lua ?
		static int lCallDatabaseTable(lua_State* L);
		/// 0x08b9aa18
		/// lua ?
		static int lIndexDatabaseTable(lua_State* L);
		/// 0x08b99c88
		/// lua ?
		static int lIndexDatabaseTableStub(lua_State* L);
		/// 0x08b9ac64
		/// lua ?
		static int lLengthOfDatabaseTable(lua_State* L);
		/// 0x08b99e74
		/// lua ?
		static int lLengthOfDatabaseTableStub(lua_State* L);
		/// 0x08b99e46
		/// lua ?
		static int lNewIndexDatabaseTable(lua_State* L);

		// Database meta-methods.
		/// 0x08b999b0
		/// lua ?
		static int lIndexDatabase(lua_State* L);
	};

}}	//namespace Database::Scripting

#endif //!defined( NO_LUA_SUPPORT )

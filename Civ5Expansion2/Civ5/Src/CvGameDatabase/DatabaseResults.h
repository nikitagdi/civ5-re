/// origin guess

#pragma once 

#include "Civ5/Src/CvGameDatabase/Database.h"

#include <list>
#include <hash_map>
#include <string>

namespace Database{

	enum ColumnTypes
	{
		COLTYPE_NULL	= 5,	//SQLITE_NULL
		COLTYPE_INT		= 1,	//SQLITE_INTEGER
		COLTYPE_FLOAT	= 2,	//SQLITE_FLOAT
		COLTYPE_TEXT	= 3,	//SQLITE_TEXT
		COLTYPE_BOOL	= 6,	//NON SQLITE ENUM
	};

	/// size 0x30
	/// vtable 0x0a300800
	class Results 
	{
	public:
		/// 0x08b98194
		CvGameDatabaseAPI Results(const char* szColumns = NULL);
		/// 0x08b9826e
		CvGameDatabaseAPI Results(const Results& copy);
		/// 0x08b9943c
		/// deleting 0x08b98334
		CvGameDatabaseAPI virtual ~Results();

		//! Retrieves the columns used by results (default: *).
		/// 0x08b98370
		CvGameDatabaseAPI virtual const char* GetColumns() const;

		//! Sets the columns used by results.
		/// 0x08b9821e
		CvGameDatabaseAPI void SetColumns(const char* szColumns); 

		//! Step and Execute function exactly the same but are meant for different
		//! use cases.  Use Execute when you do not expect any rows to be returned.
		//! Use Step when you expect one or more rows to be returned.
		/// 0x08b98390
		CvGameDatabaseAPI bool Execute();
		//! Similar to execute, but will not return an error for constraint failures.
		//! This is useful when using "insert or abort" commands.
		/// 0x08b9843c
		CvGameDatabaseAPI bool TryExecute();
		/// 0x08b98514
		CvGameDatabaseAPI bool Step();

		//! Resets the statement and also clears all bound arguments.
		/// 0x08b985b0
		CvGameDatabaseAPI bool Reset();
		//! Resets the statement and also clears all bound arguments.
		/// 0x08b985f4
		CvGameDatabaseAPI bool TryReset();

		//! Releases the command used by the results.
		/// 0x08b98634
		CvGameDatabaseAPI void Release();

		// Bind methods used to bind literal values

		//! Bind a UTF-8 string to the indexed parameter.
		//! If bMakeCopy is true, an internal copy of the string will be made.
		/// 0x08b98674
		CvGameDatabaseAPI bool Bind(int idx, const char* szValue, int lenValue, bool bMakeCopy = true);
		/// 0x08b986e6
		CvGameDatabaseAPI bool Bind(int idx, const char* szValue, bool bMakeCopy = true);

		/// 0x08b9872e
		CvGameDatabaseAPI bool Bind(int idx, const wchar_t* wszValue, int lenValue, bool bMakeCopy = true);
		/// 0x08b987d2
		CvGameDatabaseAPI bool Bind(int idx, const wchar_t* wszValue, bool bMakeCopy = true);

		/// 0x08b98816
		CvGameDatabaseAPI bool Bind(int idx, double dValue);
		/// 0x08b98856
		CvGameDatabaseAPI bool Bind(int idx, float fValue);
		/// 0x08b98896
		CvGameDatabaseAPI bool Bind(int idx, int iValue);
		/// 0x08b988d2
		CvGameDatabaseAPI bool Bind(int idx, __int64 iValue);
		/// 0x08b98918
		CvGameDatabaseAPI bool BindNULL(int idx);
		
		/// 0x08b9894c
		CvGameDatabaseAPI int ColumnCount() const;

		/// 0x08b98978
		CvGameDatabaseAPI const char* ColumnName(int iColumn);

		/// 0x08b989ac
		CvGameDatabaseAPI const char*	ColumnTypeName(int iColumn);
		/// 0x08b989e8
		CvGameDatabaseAPI ColumnTypes	ColumnType(int iColumn);

		/// 0x08b99284
		CvGameDatabaseAPI bool	HasColumn(const char* szColumnName);
		/// 0x08b98ad4
		CvGameDatabaseAPI int	ColumnPosition(const char* szColumnName);

		/// 0x08b98aa0
		CvGameDatabaseAPI const char*		GetText(int iColumn);
		/// 0x08b98ba0
		CvGameDatabaseAPI const wchar_t*	GetText16(int iColumn);
		/// 0x08b98c0c
		CvGameDatabaseAPI bool				GetBool(int iColumn);
		/// 0x08b98cd4
		CvGameDatabaseAPI int				GetInt(int iColumn);
		/// 0x08b98d4c
		CvGameDatabaseAPI __int64			GetInt64(int iColumn);
		/// 0x08b98dd8
		CvGameDatabaseAPI float				GetFloat(int iColumn);
		/// 0x08b98e62
		CvGameDatabaseAPI double			GetDouble(int iColumn);

		/// 0x08b98ee6
		CvGameDatabaseAPI void GetValue(int iColumn, bool& bValue);
		/// 0x08b98f72
		CvGameDatabaseAPI void GetValue(int iColumn, int& iValue);
		/// 0x08b98ffe
		CvGameDatabaseAPI void GetValue(int iColumn, __int64& iValue);
		/// 0x08b99096
		CvGameDatabaseAPI void GetValue(int iColumn, float& fValue);
		/// 0x08b99134
		CvGameDatabaseAPI void GetValue(int iColumn, double& dValue);
		/// 0x08b991c6
		CvGameDatabaseAPI void GetValue(int iColumn, const char*& zValue);
		/// absent
		CvGameDatabaseAPI void GetValue(int iColumn, const wchar_t*& wszValue);

		/// 0x08b98a5e
		CvGameDatabaseAPI const char*		GetText(const char* szColumn);
		/// 0x08b98b5c
		CvGameDatabaseAPI const wchar_t*	GetText16(const char* szColumn);
		/// 0x08b98bd2
		CvGameDatabaseAPI bool				GetBool(const char* szColumn);
		/// 0x08b98c92
		CvGameDatabaseAPI int				GetInt(const char* szColumn);
		/// 0x08b98d08
		CvGameDatabaseAPI __int64			GetInt64(const char* szColumn);
		/// 0x08b98d82
		CvGameDatabaseAPI float				GetFloat(const char* szColumn);
		/// 0x08b98e20
		CvGameDatabaseAPI double			GetDouble(const char* szColumn);

		/// 0x08b98e96
		CvGameDatabaseAPI void GetValue(const char* szColumn, bool& bValue);
		/// 0x08b98f28
		CvGameDatabaseAPI void GetValue(const char* szColumn, int& iValue);
		/// 0x08b98fac
		CvGameDatabaseAPI void GetValue(const char* szColumn, __int64& iValue);
		/// 0x08b99040
		CvGameDatabaseAPI void GetValue(const char* szColumn, float& fValue);
		/// 0x08b990de
		CvGameDatabaseAPI void GetValue(const char* szColumn, double& dValue);
		/// 0x08b9917c
		CvGameDatabaseAPI void GetValue(const char* szColumn, const char*& zValue);
		/// absent
		CvGameDatabaseAPI void GetValue(const char* szColumn, const wchar_t*& wszValue);

		/// 0x08b9937c
		CvGameDatabaseAPI bool SingleQuery() const;
		/// 0x08b99386
		CvGameDatabaseAPI void SingleQuery(bool bSingle);

		/// 0x08b993ee
		CvGameDatabaseAPI void* operator new(size_t tSize);
		/// 0x08b9835c
		CvGameDatabaseAPI void operator delete(void* pMem);

		//(INTERNAL USE ONLY)
		//! Attaches a SQLite3 statement to results.
		/// 0x08b99392
		bool SetCommand(Command& command);

	protected:
		/// 0x08b992b6
		CvGameDatabaseAPI void HashColumnPositions();

	private:
		Command m_Command; ///< +0x4
		const char* m_szColumns; ///< +0xc
		bool m_bSingleQuery; ///< +0x10

		stdext::hash_map<std::string, int> m_hshColumnPositions; ///< +0x14
		// seed: in Civ5XP, not in SDK
	public:
		/// 0x08b99200
		UNK_RET GetValue(char const* a1, unsigned short const*& a2);
		/// 0x08b9924a
		UNK_RET GetValue(int a1, unsigned short const*& a2);
	};

	/// size 0x30
	/// vtable 0x0a300820
	class SingleResult : public Results
	{
	public:
		/// 0x08b99402
		CvGameDatabaseAPI SingleResult(const char* szColumns = NULL);
	};

} //namespace Database

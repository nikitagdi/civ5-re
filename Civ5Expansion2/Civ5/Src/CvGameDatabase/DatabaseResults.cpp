/// origin guess
#include <Windows.h>
#include <stdio.h>
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"
#include <sqlite3.h>

/// 0x08b98194
Database::Results::Results(const char* szColumns)
	: m_Command(), m_szColumns(NULL), m_bSingleQuery(false)
{
	SetColumns(szColumns);
}

/// 0x08b9835c
void Database::Results::operator delete(void* pMem)
{
	free(pMem);
}

/// 0x08b98390
bool Database::Results::Execute()
{
	if (m_Command.connection != NULL && m_Command.stmt != NULL)
	{
		int iResult = sqlite3_step(m_Command.stmt);
		if (iResult == SQLITE_DONE)
			return true;
		if (iResult == SQLITE_ROW)
			m_Command.connection->LogWarning("A row was returned during an Execute command.");
		else
			m_Command.connection->LogError(m_Command.connection->ErrorMessage());
		char szMessage[512];
		sprintf_s(szMessage, sizeof(szMessage), "While executing - '%s'", sqlite3_sql(m_Command.stmt));
		m_Command.connection->LogMessage(szMessage);
	}
	return false;
}

/// 0x08b98514
bool Database::Results::Step()
{
	int iResult = sqlite3_step(m_Command.stmt);
	if (iResult != SQLITE_ROW && iResult != SQLITE_DONE && m_Command.stmt != NULL && m_Command.connection != NULL)
	{
		m_Command.connection->LogError(m_Command.connection->ErrorMessage());
		char szMessage[512];
		sprintf_s(szMessage, sizeof(szMessage), "While executing - '%s'", sqlite3_sql(m_Command.stmt));
		m_Command.connection->LogMessage(szMessage);
	}
	return iResult == SQLITE_ROW;
}

/// 0x08b986e6
bool Database::Results::Bind(int idx, const char* szValue, bool bMakeCopy)
{
	return Bind(idx, szValue, -1, bMakeCopy);
}

/// 0x08b98ad4
int Database::Results::ColumnPosition(const char* szColumnName)
{
	if (m_hshColumnPositions.empty())
		HashColumnPositions();

	auto it = m_hshColumnPositions.find(szColumnName);
	if (it == m_hshColumnPositions.end())
		return -1;
	return it->second;
}

/// 0x08b98bd2
bool Database::Results::GetBool(const char* szColumn)
{
	return GetBool(ColumnPosition(szColumn));
}

/// 0x08b98c92
int Database::Results::GetInt(const char* szColumn)
{
	return GetInt(ColumnPosition(szColumn));
}

/// 0x08b98d82
float Database::Results::GetFloat(const char* szColumn)
{
	return GetFloat(ColumnPosition(szColumn));
}

/// 0x08b98a5e
const char* Database::Results::GetText(const char* szColumn)
{
	return GetText(ColumnPosition(szColumn));
}

/// 0x08b98f28
void Database::Results::GetValue(const char* szColumn, int& iValue)
{
	iValue = GetInt(ColumnPosition(szColumn));
}

/// 0x08b99284
bool Database::Results::HasColumn(const char* szColumnName)
{
	return ColumnPosition(szColumnName) != -1;
}

/// 0x08b985b0
bool Database::Results::Reset()
{
	if (m_Command.stmt == NULL || sqlite3_reset(m_Command.stmt) != SQLITE_OK)
		return false;
	return sqlite3_clear_bindings(m_Command.stmt) == SQLITE_OK;
}

/// 0x08b98634
void Database::Results::Release()
{
	if (m_Command.stmt != NULL)
		Connection::ReleaseCommand(m_Command);
	if (m_szColumns != NULL)
		sqlite3_free(const_cast<char*>(m_szColumns));
	m_szColumns = NULL;
}

/// 0x08b98674
bool Database::Results::Bind(int idx, const char* szValue, int lenValue, bool bMakeCopy)
{
	return sqlite3_bind_text(m_Command.stmt, idx, szValue, lenValue, bMakeCopy ? SQLITE_TRANSIENT : SQLITE_STATIC) == SQLITE_OK;
}

/// 0x08b98896
bool Database::Results::Bind(int idx, int iValue)
{
	return sqlite3_bind_int(m_Command.stmt, idx, iValue) == SQLITE_OK;
}

/// 0x08b98918
bool Database::Results::BindNULL(int idx)
{
	return sqlite3_bind_null(m_Command.stmt, idx) == SQLITE_OK;
}

/// 0x08b98aa0
const char* Database::Results::GetText(int iColumn)
{
	if (m_Command.stmt == NULL)
		return NULL;
	return (const char*)sqlite3_column_text(m_Command.stmt, iColumn);
}

/// 0x08b98c0c
bool Database::Results::GetBool(int iColumn)
{
	ColumnTypes type = ColumnType(iColumn);
	if (type == COLTYPE_INT || type == COLTYPE_BOOL)
	{
		if (m_Command.stmt != NULL)
			return sqlite3_column_int(m_Command.stmt, iColumn) != 0;
	}
	else if (type == COLTYPE_TEXT)
	{
		const char* text = (const char*)sqlite3_column_text(m_Command.stmt, iColumn);
		return *text == 'T' || *text == 't';
	}
	return false;
}

/// 0x08b98cd4
int Database::Results::GetInt(int iColumn)
{
	if (m_Command.stmt == NULL)
		return 0;
	return sqlite3_column_int(m_Command.stmt, iColumn);
}

/// 0x08b98dd8
float Database::Results::GetFloat(int iColumn)
{
	if (m_Command.stmt == NULL)
		return 0.0f;
	return (float)sqlite3_column_double(m_Command.stmt, iColumn);
}

/// 0x08b99402
Database::SingleResult::SingleResult(const char* szColumns)
	: Results(szColumns)
{
	SingleQuery(true);
}

/// 0x08b9943c
Database::Results::~Results()
{
	Release();
}

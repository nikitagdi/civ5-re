/// origin guess
// $Header
//------------------------------------------------------------------------------
#pragma once 

#include <list>

#include <string>
#include <stddef.h>

#include "Civ5/Src/CvGameDatabase/Database.h"
namespace Database { class FXmlTraverser; }


class FXmlTraverser;

namespace Database
{
	class ResultsCache;
	/// size 0x328
	class XMLSerializer
	{
	public:
		/// 0x08b9bb68
		CvGameDatabaseAPI XMLSerializer(Connection& database);
		/// 0x08b9bbfa
		CvGameDatabaseAPI ~XMLSerializer();

		//The following methods load XML data from disk.
		//! \param szName - The location on disk to the file.
		//! \param kResultsCache - A cache object used to minimize the amount of queries created.
		/// 0x08b9bc46
		CvGameDatabaseAPI bool Load(const wchar_t* wszFilename);
		/// 0x08b9bcb2
		CvGameDatabaseAPI bool Load(const wchar_t* wszFilename, ResultsCache& kResultsCache);

		//The following methods load XML data from memory.
		//! \param szName -  An identifier used for error reporting.
		//! \param szBuffer - The pointer to an allocated buffer.
		//! \param lenBuffer - The length of the buffer.
		//! \param kResultsCache - A cache object used to minimize the amount of queries created.
		/// 0x08b9c1e0
		CvGameDatabaseAPI bool LoadFromMemory(const wchar_t* wszFilename, char* szBuffer, size_t lenBuffer);
		/// 0x08b9beee
		CvGameDatabaseAPI bool LoadFromMemory(const wchar_t* wszFilename, char* szBuffer, size_t lenBuffer, ResultsCache& kResultsCache);

		//! Tells the serializer whether or not each Load should use a transaction.
		//! Default is true.
		/// 0x08ba2a2a
		CvGameDatabaseAPI bool UseTransactions() const;
		/// 0x08ba2a36
		CvGameDatabaseAPI void UseTransactions(bool bUse);
		
		/// inline
		CvGameDatabaseAPI const char* ErrorMessage() const;

		/// 0x08ba2afa
		CvGameDatabaseAPI void SetPackageTag(const char* szTag);
		/// 0x08ba2b5e
		CvGameDatabaseAPI void AddTableToPackageTagList(const char* szTag);
		/// 0x08ba2c16
		CvGameDatabaseAPI void RemoveTableFromPackageTagList(const char* szTag);
		/// 0x08ba2c84
		CvGameDatabaseAPI void ClearPackageTagList();

	protected:
		enum StringType
		{
			STRINGTYPE_UNKNOWN	= -1,
			STRINGTYPE_NULL		= 0,
			STRINGTYPE_TEXT		= 1,
			STRINGTYPE_INT		= 2,
			STRINGTYPE_REAL		= 3,			
		};

		enum InsertConflictClause
		{
			NONE,
			OR_ABORT,
			OR_IGNORE,
			OR_REPLACE,
		};

		/// 0x08b9be80
		void SetErrorMessage(const char* szMsg);
		/// 0x08b9e0d8
		StringType GetStringType(const char* szString, const size_t lenString);

		/// absent
		bool BuildTable(::FXmlTraverser& kXml);
		/// absent
		bool IndexTable(::FXmlTraverser& kXml);
		/// absent
		bool UpdateTable(::FXmlTraverser& kXml, ResultsCache& kResultsCache);
		/// absent
		bool DeleteMissingReferences(::FXmlTraverser& kXml);
	
		/// absent
		bool InsertRow(::FXmlTraverser& kRow, ResultsCache& kResultsCache, const char* szTableName, size_t lenTableName, InsertConflictClause clause = NONE);
		/// absent
		bool InsertOrIgnoreRow(::FXmlTraverser& kRow, ResultsCache& kResultsCache, const char* szTableName, size_t lenTableName);
		/// absent
		bool ReplaceRow(::FXmlTraverser& kRow, ResultsCache& kResultsCache, const char* szTableName, size_t lenTableName);
		/// absent
		bool DeleteRow(::FXmlTraverser& kRow, ResultsCache& kResultsCache, const char* szTableName, size_t lenTableName);
		/// absent
		bool UpdateRow(::FXmlTraverser& kRows, ResultsCache& kResultsCache, const char* szTableName, size_t lenTableName);

		//! Loads the entire contents of the file szFilename into the buffer pkFilebuffer.
		//! Will allocate or reallocate if necessary depending the preexisting size of the buffer.
		/// 0x08b9bd62
		bool LoadFileIntoBuffer(const wchar_t* wszFilename, char*& pkFilebuffer, size_t& pkNewBufferSize, size_t& kOutFileSize);

		/// 0x08ba2a46
		void PostColumnInsert(std::stringstream& columns, std::list<std::string> &values, const char* szTableName, size_t lenTableName);

	private:
		/// absent
		XMLSerializer(const XMLSerializer&);			//Stub to prevent compiler generated function.
		/// absent
		XMLSerializer& operator=(const XMLSerializer&);	//Stub to prevent compiler generated function.

		char m_szCurrentFileName[256]; ///< +0x0

		char m_szErrorMessage[512]; ///< +0x100
		Connection& m_db; ///< +0x300

		char* m_pkFileBuffer; ///< +0x304
		size_t m_lenFileBuffer; ///< +0x308

		bool m_bUseTransactions; ///< +0x30c

		typedef std::list<std::string> PackageTagTableList;
		PackageTagTableList		m_lstPackageTagTables; ///< +0x310
		std::string				m_szPackageTag; ///< +0x31c
		// seed: in Civ5XP, not in SDK
	public:
		/// 0x08b9c254
		UNK_RET BuildTable(Database::FXmlTraverser& a1);
		/// 0x08b9d164
		UNK_RET IndexTable(Database::FXmlTraverser& a1);
		/// 0x08b9d89e
		UNK_RET DeleteMissingReferences(Database::FXmlTraverser& a1);
		/// 0x08b9dcce
		UNK_RET UpdateTable(Database::FXmlTraverser& a1, Database::ResultsCache& a2);
		/// 0x08b9e1a0
		UNK_RET InsertRow(Database::FXmlTraverser& a1, Database::ResultsCache& a2, char const* a3, unsigned int a4, Database::XMLSerializer::InsertConflictClause a5);
		/// 0x08b9fcf8
		UNK_RET DeleteRow(Database::FXmlTraverser& a1, Database::ResultsCache& a2, char const* a3, unsigned int a4);
		/// 0x08ba109e
		UNK_RET UpdateRow(Database::FXmlTraverser& a1, Database::ResultsCache& a2, char const* a3, unsigned int a4);
		/// 0x08ba298a
		UNK_RET ReplaceRow(Database::FXmlTraverser& a1, Database::ResultsCache& a2, char const* a3, unsigned int a4);
		/// 0x08ba29da
		UNK_RET InsertOrIgnoreRow(Database::FXmlTraverser& a1, Database::ResultsCache& a2, char const* a3, unsigned int a4);
	};

}	//namespace Database

//------------------------------------------------------------------------------
// Note: inline routines
//------------------------------------------------------------------------------
/// inline
inline const char* Database::XMLSerializer::ErrorMessage() const
{
	return m_szErrorMessage;
}

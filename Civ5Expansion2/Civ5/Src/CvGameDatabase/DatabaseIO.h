/// origin guess

#pragma once 

#include "Civ5/Src/CvGameDatabase/Database.h"

namespace Database
{
	/// size 0x4
	class BinaryIO
	{
	public:
		/// 0x08b97c64
		CvGameDatabaseAPI BinaryIO(const char* szFilename);

		/// 0x08b97c70
		CvGameDatabaseAPI bool operator << (Connection& db);
		/// 0x08b97ce0
		CvGameDatabaseAPI bool operator >> (Connection& db);

		/// 0x08b97ca8
		CvGameDatabaseAPI bool Save(Connection& db);
		/// 0x08b97d18
		CvGameDatabaseAPI bool Load(Connection& db);

	protected:
		/// 0x08b97d50
		bool SaveLoad(const char* szFilename, Connection& db, bool bSave);

	private:
		const char* m_pszFilename; ///< +0x0
	};

}	//namespace Database

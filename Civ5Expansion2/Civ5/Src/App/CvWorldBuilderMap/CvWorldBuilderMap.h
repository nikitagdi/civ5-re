/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   CIV 5 World Builder Map   ********************
//
//  FILE:    CvWorldBuilderMap.h
//
//  AUTHOR:  Eric Jordan  --  7/20/2009
//
//  PURPOSE:
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#pragma once

#include <bitset>

#include "FirePlace/FireWorks/FFireTypes.h"
#include "FirePlace/FireWorks/FAssert.h"
#include "FirePlace/FireWorks/FString.h"
#include <algorithm>
#ifndef CvWorldBuilderMap_h
#define CvWorldBuilderMap_h

#include "Civ5/Src/App/CvWorldBuilderMap/FRelationshipBitset.h"
#include "Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMapElementAllocator.h"
#include "Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMapModData.h"
#include "Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMapTypeGroup.h"
#include "FirePlace/FireWorks/FUncopyable.h"

class FIFile;
class CvWorldBuilderMapTypeDesc;

/// size 0xeebe0
class CvWorldBuilderMap : FUncopyable
{
public:
	/// 0x0923e6b8
	static const uint FileVersion = 12;
	/// 0x0923e6bc
	static const uint MaxMapSize = 32768;
	/// 0x0923e6c0
	static const uint MaxPlayers = 32;
	/// 0x0923e6c4
	static const uint MaxCityStates = 64;
	/// 0x0923e6c8
	static const uint MaxTeams = MaxPlayers + MaxCityStates;
	/// inline
	static const uint BarbarianPlayer = MaxPlayers + MaxCityStates;

	/// 0x08c99c7c
	CvWorldBuilderMap();
	/// 0x08c9a158
	~CvWorldBuilderMap();

	/// 0x08c9a2f8
	void Resize(uint uiWidth, uint uiHeight);

#ifndef CV_WORLD_BUILDER_MAP_IN_GAME
	MEMBER_EVENT2(MapResized, uint, uint); // uiWidth, uiHeight
#endif

	// Load / Save
	/// 0x08c9a326
	bool Save( _In_z_ const wchar_t* wszFilename, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario = true) const;
	/// 0x08c9af0a
	bool Load( _In_z_ const wchar_t* wszFilename, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario = true);

	/// inline
	inline uint GetWidth() const { return m_kMapDesc.uiWidth; }
	/// inline
	inline uint GetHeight() const { return m_kMapDesc.uiHeight; }
	/// inline
	inline uint GetPlotCount() const { return m_kMapDesc.uiWidth * m_kMapDesc.uiHeight; }

	/// size 0x8
	class PlotMapData
	{
	public:
		/// 0x08ca8d8c
		PlotMapData();

		/// inline
		__forceinline byte GetTerrainType() const { return m_byTerrainType; }
		/// inline
		__forceinline void SetTerrainType(byte byType) { m_byTerrainType = byType; }

		/// inline
		__forceinline byte GetContinentType() const { return m_byContinentType; }
		/// inline
		__forceinline void SetContinentType(byte byType) { m_byContinentType = byType; }

		/// inline
		static const byte InvalidResource = byte(-1);
		/// inline
		__forceinline byte GetResourceType() const { return m_byResourceType; }
		/// inline
		__forceinline void SetResourceType(byte byType) { m_byResourceType = byType; }

		/// inline
		__forceinline byte GetResourceAmount() const { return m_byResourceAmount; }
		/// inline
		__forceinline void SetResourceAmount(byte byAmount) { m_byResourceAmount = byAmount; }

		/// inline
		static const byte InvalidFeature = byte(-1);
		/// inline
		__forceinline byte GetFeatureType() const { return m_byFeatureType; }
		/// inline
		__forceinline void SetFeatureType(byte byType) { m_byFeatureType = byType; }

		/// inline
		static const byte InvalidNaturalWonder = byte(-1);
		/// inline
		__forceinline byte GetNaturalWonderType() const { return m_byNaturalWonderType; }
		/// inline
		__forceinline void SetNaturalWonderType(byte byType) { m_byNaturalWonderType = byType; }

		enum PlotHeight
		{
			FLAT_TERRAIN = 0,
			HILLS = 1,
			MOUNTAINS = 2,
		};

		/// inline
		__forceinline PlotHeight GetPlotHeight() const { return (PlotHeight)m_byPlotHeight; }
		/// inline
		__forceinline void SetPlotHeight(PlotHeight eHeight) { m_byPlotHeight = (byte)eHeight; }

		enum PlotFlags
		{
			// Rivers
			W_OF_RIVER    = 0x01,
			NW_OF_RIVER   = 0x02,
			NE_OF_RIVER   = 0x04,
			RIVER_FLOW_S  = 0x08,
			RIVER_FLOW_NE = 0x10,
			RIVER_FLOW_NW = 0x20,

			// Starting Positions
			START_POS_MAJOR = 0x40,
			START_POS_MINOR = 0x80,
		};

		/// inline
		__forceinline bool GetFlag(PlotFlags eFlag) const { return (m_byFlags & eFlag) != 0; }
		/// inline
		__forceinline void SetFlag(PlotFlags eFlag) { m_byFlags |= eFlag; }
		/// inline
		__forceinline void ClearFlag(PlotFlags eFlag) { m_byFlags &= ~eFlag; } 
		/// inline
		__forceinline void SetFlag(PlotFlags eFlag, bool bValue)
		{
			if( bValue ) m_byFlags |= eFlag;
			else m_byFlags &= ~eFlag;
		}

		/// inline
		__forceinline byte GetFlags() const { return m_byFlags; }
		/// inline
		__forceinline void SetFlags(byte byFlags) { m_byFlags = byFlags; }

	private:
		// Note: member data forms qword
		byte m_byTerrainType; ///< +0x0
		byte m_byResourceType; ///< +0x1
		byte m_byFeatureType; ///< +0x2
		byte m_byFlags; ///< +0x3
		byte m_byPlotHeight; ///< +0x4
		byte m_byContinentType; ///< +0x5
		byte m_byNaturalWonderType; ///< +0x6
		byte m_byResourceAmount; ///< +0x7
	};

	// Backwards Compatibility Only.  Not used for anything but loading old maps.
	/// size 0xc
	struct OldUnit
	{
		/// inline
		static const byte InvalidUnitType = byte(-1);
		typedef CvWorldBuilderMapElementAllocator<OldUnit, 16> Map;
		typedef Map::Handle Handle;

		/// 0x08ca9028
		OldUnit();

		enum DirectionTypes
		{
			DIRECTION_RANDOM,
			DIRECTION_NORTHEAST,
			DIRECTION_EAST,
			DIRECTION_SOUTHEAST,
			DIRECTION_SOUTHWEST,
			DIRECTION_WEST,
			DIRECTION_NORTHWEST,
		};

		Handle m_hStackedUnit; ///< +0x0
		word m_wExperience; ///< +0x2
		float m_fHealth; ///< +0x4
		byte m_byUnitType; ///< +0x8
		byte m_byOwner; ///< +0x9
		byte m_byDirection; ///< +0xa
		bool m_bFortified : 1; ///< +0xb
		bool m_bEmbarked : 1; ///< +0xb
	};

	// Just a fixed buffer for a unit name.  I want to package these together in a serializable
	// block allocator which is why they aren't variable-length heap allocations.
	/// size 0x40
	struct UnitName
	{
		typedef CvWorldBuilderMapElementAllocator<UnitName, 4> Map;
		typedef Map::Handle Handle;

		/// inline
		static const uint MaxLength = 64;
		char m_szText[MaxLength]; ///< +0x0

		/// inline
		UnitName() { m_szText[0] = 0; }
	};

	/// size 0x30
	struct UnitV2
	{
		/// inline
		static const byte InvalidUnitType = byte(-1);
		typedef CvWorldBuilderMapElementAllocator<UnitV2, 16> Map;
		typedef Map::Handle Handle;

		/// inline
		static const uint MaxPromotionTypes = 256;
		typedef std::bitset<MaxPromotionTypes> PromotionBitset;

		/// inline
		static const uint MaxHealth = 100000;

		/// 0x08ca904e
		UnitV2();

		enum DirectionTypes
		{
			DIRECTION_RANDOM,
			DIRECTION_NORTHEAST,
			DIRECTION_EAST,
			DIRECTION_SOUTHEAST,
			DIRECTION_SOUTHWEST,
			DIRECTION_WEST,
			DIRECTION_NORTHWEST,
		};

		enum Flags
		{
			UNIT_FORTIFIED  = 0x01,
			UNIT_EMBARKED   = 0x02,
			UNIT_GARRISONED = 0x04
		};

		Handle m_hStackedUnit; ///< +0x0
		UnitName::Handle m_hCustomName; ///< +0x2
		uint m_uiExperience; ///< +0x4
		uint m_uiHealth; ///< +0x8
		byte m_byUnitType; ///< +0xc
		byte m_byOwner; ///< +0xd
		byte m_byDirection; ///< +0xe
		byte m_byFlags; ///< +0xf

		PromotionBitset m_kPromotions; ///< +0x10
	};

	/// size 0x54
	struct Unit
	{
		/// inline
		static const uint InvalidUnitType = uint(-1);
		typedef CvWorldBuilderMapElementAllocator<Unit, 16> Map;
		typedef Map::Handle Handle;

		/// inline
		static const uint MaxPromotionTypes = 512;
		typedef std::bitset<MaxPromotionTypes> PromotionBitset;

		/// inline
		static const uint MaxHealth = 100000;

		/// 0x08ca8d9e
		Unit();
		/// 0x08ca8e3e
		explicit Unit(const OldUnit &kOldUnit); // For conversion
		/// 0x08ca8f3e
		explicit Unit(const UnitV2 &kOldUnit); // For conversion

		enum DirectionTypes
		{
			DIRECTION_RANDOM,
			DIRECTION_NORTHEAST,
			DIRECTION_EAST,
			DIRECTION_SOUTHEAST,
			DIRECTION_SOUTHWEST,
			DIRECTION_WEST,
			DIRECTION_NORTHWEST,
		};

		enum Flags
		{
			UNIT_FORTIFIED  = 0x01,
			UNIT_EMBARKED   = 0x02,
			UNIT_GARRISONED = 0x04
		};

		typedef uint UnitType;
		Handle m_hStackedUnit; ///< +0x0
		UnitName::Handle m_hCustomName; ///< +0x2
		uint m_uiExperience; ///< +0x4
		uint m_uiHealth; ///< +0x8
		UnitType m_byUnitType; ///< +0xc
		byte m_byOwner; ///< +0x10
		byte m_byDirection; ///< +0x11
		byte m_byFlags; ///< +0x12

		PromotionBitset m_kPromotions; ///< +0x14

		// Remember floating point error if you choose to use these
		/// inline
		void SetHealthAsFloat(const float fHealth) { m_uiHealth = (uint)(fHealth * MaxHealth); }
		/// inline
		const float GetHealthAsFloat() const { return (float)m_uiHealth / (float)MaxHealth; }

		/// inline
		__forceinline bool GetFlag(Flags eFlag) const { return (m_byFlags & eFlag) != 0; }
		/// inline
		__forceinline void SetFlag(Flags eFlag) { m_byFlags |= eFlag; }
		/// inline
		__forceinline void ClearFlag(Flags eFlag) { m_byFlags &= ~eFlag; } 
		/// inline
		__forceinline void SetFlag(Flags eFlag, bool bValue)
		{
			if( bValue ) m_byFlags |= eFlag;
			else m_byFlags &= ~eFlag;
		}
	};

	// Backwards Compatibility Only.  Not used for anything but loading old maps.
	/// size 0x64
	struct OldCity
	{
		typedef CvWorldBuilderMapElementAllocator<OldCity, 8> Map;
		typedef Map::Handle Handle;

		/// inline
		static const uint MaxBuildingTypes = 256;
		typedef std::bitset<MaxBuildingTypes> BuildingBitset;

		/// 0x08ca930a
		OldCity();

		char m_szName[56]; ///< +0x0
		uint m_uiPopulation; ///< +0x38
		uint m_uiCulture; ///< +0x3c
		byte m_byOwner; ///< +0x40
		bool m_bLocalizedName; ///< +0x41
		BuildingBitset m_kBuildings; ///< +0x44
	};

	/// size 0x68
	struct CityV2
	{
		typedef CvWorldBuilderMapElementAllocator<CityV2, 8> Map;
		typedef Map::Handle Handle;

		/// inline
		static const uint MaxBuildingTypes = 256;
		typedef std::bitset<MaxBuildingTypes> BuildingBitset;

		/// inline
		static const uint MaxNameLength = 64;

		/// inline
		static const uint MaxHealth = 100000;

		enum Flags
		{
			CITY_LOCALIZED_NAME = 0x01,
			CITY_PUPPET_STATE   = 0x02,
			CITY_OCCUPIED       = 0x04
		};

		/// 0x08ca935a
		CityV2();

		char m_szName[MaxNameLength]; ///< +0x0
		byte m_byOwner; ///< +0x40
		byte m_byFlags; ///< +0x41
		word m_wPopulation; ///< +0x42
		uint m_uiHealth; ///< +0x44
		BuildingBitset m_kBuildings; ///< +0x48
	};

	/// size 0x88
	struct City
	{
		typedef CvWorldBuilderMapElementAllocator<City, 8> Map;
		typedef Map::Handle Handle;

		/// inline
		static const uint MaxBuildingTypes = 512;
		typedef std::bitset<MaxBuildingTypes> BuildingBitset;

		/// inline
		static const uint MaxNameLength = 64;

		/// inline
		static const uint MaxHealth = 100000;

		enum Flags
		{
			CITY_LOCALIZED_NAME = 0x01,
			CITY_PUPPET_STATE   = 0x02,
			CITY_OCCUPIED       = 0x04
		};

		/// 0x08ca90b0
		City();
		/// 0x08ca9144
		explicit City(const OldCity &kOldCity); // For conversion
		/// 0x08ca920a
		explicit City(const CityV2 &kOldCity); // For conversion

		char m_szName[MaxNameLength]; ///< +0x0
		byte m_byOwner; ///< +0x40
		byte m_byFlags; ///< +0x41
		word m_wPopulation; ///< +0x42
		uint m_uiHealth; ///< +0x44
		BuildingBitset m_kBuildings; ///< +0x48

		// Remember floating point error if you choose to use these
		/// inline
		void SetHealthAsFloat(const float fHealth) { m_uiHealth = (uint)(fHealth * MaxHealth); }
		/// inline
		const float GetHealthAsFloat() const { return (float)m_uiHealth / (float)MaxHealth; }

		/// inline
		__forceinline bool GetFlag(Flags eFlag) const { return (m_byFlags & eFlag) != 0; }
		/// inline
		__forceinline void SetFlag(Flags eFlag) { m_byFlags |= eFlag; }
		/// inline
		__forceinline void ClearFlag(Flags eFlag) { m_byFlags &= ~eFlag; } 
		/// inline
		__forceinline void SetFlag(Flags eFlag, bool bValue)
		{
			if( bValue ) m_byFlags |= eFlag;
			else m_byFlags &= ~eFlag;
		}
	};

	/// size 0x8
	struct PlotScenarioData
	{
		/// inline
		static const byte InvalidCulture = byte(-1);
		/// inline
		static const byte InvalidImprovement = byte(-1);
		/// inline
		static const byte InvalidRoute = byte(-1);

		/// 0x08ca93b0
		PlotScenarioData();

		// Note: member data forms qword
		City::Handle m_hCity; ///< +0x0
		Unit::Handle m_hUnitStack; ///< +0x2
		byte m_byCulture; ///< +0x4
		byte m_byImprovement; ///< +0x5
		byte m_byRoute; ///< +0x6
		byte m_byRouteOwner; ///< +0x7
	};

	/// size 0x40
	struct Team
	{
		/// 0x08ca93c2
		Team();

		/// inline
		static const uint MaxTechs = 256;
		typedef std::bitset<MaxTechs> TechBitset;

		char m_szName[32]; ///< +0x0

		TechBitset m_kTechs; ///< +0x20
	};

	// Backwards Compatibility Only.  Not used for anything but loading old maps.
	/// size 0x130
	struct OldPlayer
	{
		/// inline
		static const uint MaxPolicies = 256;
		/// inline
		static const byte InvalidTeam = byte(-1);
		typedef std::bitset<MaxPolicies> PolicyBitset;

		/// 0x08ca95c6
		OldPlayer();

		PolicyBitset m_kPolicies; ///< +0x0

		char m_szCivType[64]; ///< +0x20
		char m_szTeamColor[64]; ///< +0x60
		char m_szEra[64]; ///< +0xa0
		char m_szHandicap[60]; ///< +0xe0

		uint m_uiCulture; ///< +0x11c
		uint m_uiGold; ///< +0x120
		uint m_uiStartX; ///< +0x124
		uint m_uiStartY; ///< +0x128

		byte m_byTeam; ///< +0x12c
		bool m_bPlayable; ///< +0x12d
	};

	/// size 0x1b4
	struct Player
	{
		/// inline
		static const uint MaxPolicies = 256;
		/// inline
		static const byte InvalidTeam = byte(-1);
		typedef std::bitset<MaxPolicies> PolicyBitset;

		/// 0x08ca9402
		Player();
		/// 0x08ca949e
		explicit Player(const OldPlayer &kOldPlayer); // Conversion

		PolicyBitset m_kPolicies; ///< +0x0

		char m_szLeaderName[64]; ///< +0x20
		char m_szCivName[64]; ///< +0x60
		char m_szCivType[64]; ///< +0xa0
		char m_szTeamColor[64]; ///< +0xe0
		char m_szEra[64]; ///< +0x120
		char m_szHandicap[64]; ///< +0x160

		uint m_uiCulture; ///< +0x1a0
		uint m_uiGold; ///< +0x1a4
		uint m_uiStartX; ///< +0x1a8
		uint m_uiStartY; ///< +0x1ac

		byte m_byTeam; ///< +0x1b0
		bool m_bPlayable; ///< +0x1b1
	};

	/// 0x08ca8396
	void SetAllPlotData(const PlotMapData &kPlotData, const PlotScenarioData &kScenarioData);
	/// 0x08ca81f4
	void ClearScenarioData(bool bPreserveImprovements = false);

	/// inline
	inline const uint GetPlotIndex(uint uiX, uint uiY) const
	{
		FAssertMsg(uiX < m_kMapDesc.uiWidth && uiY < m_kMapDesc.uiHeight, "Invalid Position");
		return uiX + uiY * m_kMapDesc.uiWidth;
	}

	/// inline
	inline const PlotMapData &GetPlotData(uint uiX, uint uiY) const
	{
		if( uiX < m_kMapDesc.uiWidth && uiY < m_kMapDesc.uiHeight )
		{
			return m_akMapData[uiX + uiY * m_kMapDesc.uiWidth];
		}
		else
		{
			FAssertMsg(false, "Invalid Position");
			return sm_kErrorPlotMapData;
		}
	}

	/// inline
	inline PlotMapData &GetPlotData(uint uiX, uint uiY)
	{
		if( uiX < m_kMapDesc.uiWidth && uiY < m_kMapDesc.uiHeight )
		{
			return m_akMapData[uiX + uiY * m_kMapDesc.uiWidth];
		}
		else
		{
			FAssertMsg(false, "Invalid Position");
			return sm_kErrorPlotMapData;
		}
	}

	/// inline
	inline const PlotMapData &GetPlotData(uint uiIndex) const
	{
		if( uiIndex < m_kMapDesc.uiWidth * m_kMapDesc.uiHeight && uiIndex < MaxMapSize )
		{
			return m_akMapData[uiIndex];
		}
		else
		{
			FAssertMsg(false, "Invalid Position");
			return sm_kErrorPlotMapData;
		}
	}

	/// inline
	inline PlotMapData &GetPlotData(uint uiIndex)
	{
		if( uiIndex < m_kMapDesc.uiWidth * m_kMapDesc.uiHeight && uiIndex < MaxMapSize )
		{
			return m_akMapData[uiIndex];
		}
		else
		{
			FAssertMsg(false, "Invalid Position");
			return sm_kErrorPlotMapData;
		}
	}

	/// inline
	inline const PlotScenarioData &GetPlotScenarioData(uint uiX, uint uiY) const
	{
		if( uiX < m_kMapDesc.uiWidth && uiY < m_kMapDesc.uiHeight )
		{
			return m_akScenarioData[uiX + uiY * m_kMapDesc.uiWidth];
		}
		else
		{
			FAssertMsg(false, "Invalid Position");
			return sm_kErrorPlotScenarioData;
		}
	}

	/// inline
	inline PlotScenarioData &GetPlotScenarioData(uint uiX, uint uiY)
	{
		if( uiX < m_kMapDesc.uiWidth && uiY < m_kMapDesc.uiHeight )
		{
			return m_akScenarioData[uiX + uiY * m_kMapDesc.uiWidth];
		}
		else
		{
			FAssertMsg(false, "Invalid Position");
			return sm_kErrorPlotScenarioData;
		}
	}

	/// inline
	inline const PlotScenarioData &GetPlotScenarioData(uint uiIndex) const
	{
		if( uiIndex < m_kMapDesc.uiWidth * m_kMapDesc.uiHeight && uiIndex < MaxMapSize )
		{
			return m_akScenarioData[uiIndex];
		}
		else
		{
			FAssertMsg(false, "Invalid Position");
			return sm_kErrorPlotScenarioData;
		}
	}

	/// inline
	inline PlotScenarioData &GetPlotScenarioData(uint uiIndex)
	{
		if( uiIndex < m_kMapDesc.uiWidth * m_kMapDesc.uiHeight && uiIndex < MaxMapSize )
		{
			return m_akScenarioData[uiIndex];
		}
		else
		{
			FAssertMsg(false, "Invalid Position");
			return sm_kErrorPlotScenarioData;
		}
	}

	/// inline
	inline const byte GetPlayerCount() const { return m_kGameDesc.byPlayerCount; }
	/// inline
	inline void SetPlayerCount(byte byCount)
	{
		FAssertMsg(byCount < MaxPlayers, "Too many players");
		if( byCount < MaxPlayers )
			m_kGameDesc.byPlayerCount = byCount;
	}

	/// inline
	inline const byte GetCityStateCount() const { return m_kGameDesc.byCityStateCount; }
	/// inline
	inline void SetCityStateCount(byte byCount)
	{
		FAssertMsg(byCount < MaxCityStates, "Too many city states");
		if( byCount < MaxCityStates )
			m_kGameDesc.byCityStateCount = byCount;
	}

	/// inline
	inline const byte GetTeamCount() const { return m_kGameDesc.byTeamCount; }
	/// inline
	inline void SetTeamCount(byte byCount)
	{
		FAssertMsg(byCount < MaxTeams, "Too many teams");
		if( byCount < MaxTeams )
			m_kGameDesc.byTeamCount = byCount;
	}

	/// inline
	inline Player &GetPlayer(byte byPlayer)
	{
		if( byPlayer < m_kGameDesc.byPlayerCount && byPlayer < MaxPlayers )
		{
			return m_akPlayers[byPlayer];
		}
		else
		{
			FAssertMsg(false, "Invalid Player");
			return sm_kErrorPlayer;
		}
	}

	/// inline
	inline const Player &GetPlayer(byte byPlayer) const
	{
		if( byPlayer < m_kGameDesc.byPlayerCount && byPlayer < MaxPlayers )
		{
			return m_akPlayers[byPlayer];
		}
		else
		{
			FAssertMsg(false, "Invalid Player");
			return sm_kErrorPlayer;
		}
	}

	/// inline
	inline Player &GetCityState(byte byPlayer)
	{
		if( byPlayer < m_kGameDesc.byCityStateCount && byPlayer < MaxCityStates )
		{
			return m_akCityStates[byPlayer];
		}
		else
		{
			FAssertMsg(false, "Invalid City State");
			return sm_kErrorPlayer;
		}
	}

	/// inline
	inline const Player &GetCityState(byte byPlayer) const
	{
		if( byPlayer < m_kGameDesc.byCityStateCount && byPlayer < MaxCityStates )
		{
			return m_akCityStates[byPlayer];
		}
		else
		{
			FAssertMsg(false, "Invalid City State");
			return sm_kErrorPlayer;
		}
	}

	/// inline
	inline Team &GetTeam(byte byTeam)
	{
		if( byTeam < m_kGameDesc.byTeamCount && byTeam < MaxTeams )
		{
			return m_akTeams[byTeam];
		}
		else
		{
			FAssertMsg(false, "Invalid Team");
			return sm_kErrorTeam;
		}
	}

	/// inline
	inline const Team &GetTeam(byte byTeam) const
	{
		if( byTeam < m_kGameDesc.byTeamCount && byTeam < MaxTeams )
		{
			return m_akTeams[byTeam];
		}
		else
		{
			FAssertMsg(false, "Invalid Team");
			return sm_kErrorTeam;
		}
	}

	/// 0x08ca83d6
	Unit::Handle AddUnit(uint uiX, uint uiY);
	/// 0x08ca8466
	bool RemoveUnit(uint uiX, uint uiY, Unit::Handle &hUnit);

	/// inline
	inline const bool GetVisibility(uint uiX, uint uiY, uint uiTeam) const
	{
		FAssertMsg(uiX < m_kMapDesc.uiWidth && uiY < m_kMapDesc.uiHeight, "Invalid Position");
		FAssertMsg(uiTeam < m_kGameDesc.byTeamCount, "Invalid Team");
		const uint uiMapSize = m_kMapDesc.uiWidth * m_kMapDesc.uiHeight;
		return m_kVisibility.Get(uiX + uiY * m_kMapDesc.uiWidth + uiTeam * uiMapSize);
	}

	/// inline
	inline void SetVisibility(uint uiX, uint uiY, uint uiTeam, bool bVisible)
	{
		FAssertMsg(uiX < m_kMapDesc.uiWidth && uiY < m_kMapDesc.uiHeight, "Invalid Position");
		FAssertMsg(uiTeam < m_kGameDesc.byTeamCount, "Invalid Team");
		const uint uiMapSize = m_kMapDesc.uiWidth * m_kMapDesc.uiHeight;
		m_kVisibility.Set(uiX + uiY * m_kMapDesc.uiWidth + uiTeam * uiMapSize, bVisible);
	}

	/// 0x08ca851c
	void SetDefaultGameSpeed( _In_z_ const char *szSpeedType);
	/// inline
	inline const char *GetDefaultGameSpeed() const { return m_kGameDesc.szDefaultSpeed; }

	/// 0x08ca856a
	void SetWorldType( _In_z_ const char* szWorldType);
	/// inline
	inline const char* GetWorldType() const {return m_szWorldType;}

	/// inline
	inline void SetStartYear(int iYear) { m_kGameDesc.iStartYear = iYear; }
	/// inline
	inline int GetStartYear() const { return m_kGameDesc.iStartYear; }

	/// inline
	inline void SetMaxTurns(uint uiMaxTurns) { m_kGameDesc.uiMaxTurns = uiMaxTurns; }
	/// inline
	inline uint GetMaxTurns() const { return m_kGameDesc.uiMaxTurns; }

	/// 0x08ca8660
	void SwapPlayers(byte byPlayer1, byte byPlayer2);
	/// 0x08ca8882
	void SwapCityStates(byte byCityState1, byte byCityState2);

	FRelationshipBitset<MaxTeams, true>  m_kTeamsInContact; ///< +0x0
	FRelationshipBitset<MaxTeams, false> m_kTeamsAtWar; ///< +0x23a
	FRelationshipBitset<MaxTeams, true>  m_kTeamsAtPermanentWarOrPeace; ///< +0x474
	FRelationshipBitset<MaxTeams, true>  m_kTeamsSharingOpenBorders; ///< +0x6ae
	FRelationshipBitset<MaxTeams, true>  m_kTeamsSharingDefensivePacts; ///< +0x8e8

	Unit::Map m_kUnits; ///< +0xb24
	UnitName::Map m_kUnitNames; ///< +0xb30
	City::Map m_kCities; ///< +0xb3c

	CvWorldBuilderMapModData m_kModData; ///< +0xb48

	FStringA m_sMapName; ///< +0xfa8
	FStringA m_sMapDescription; ///< +0xfac

	CvWorldBuilderMapTypeGroup m_kVictoryTypes; ///< +0xfb0
	CvWorldBuilderMapTypeGroup m_kGameOptions; ///< +0xfb8

	int m_aiCityStateInfluence[MaxPlayers][MaxCityStates]; ///< +0xfc0

	/// 0x08ca8a9e
	static uint PreviewPlayableCivCount( _In_z_ const wchar_t* wszFilename);

	enum MapFlags
	{
		FLAG_WORLD_WRAP       = 0x01,
		FLAG_RANDOM_RECOURCES = 0x02,
		FLAG_RANDOM_GOODIES   = 0x04,
		FLAG_OLD_GOODIE_RULES = 0x08, // Backwards compatibility thing.  Don't set normally.
		FLAG_TRANSFERABLE     = 0x10, // Determines if a map can be transfered over the internets
	};

	/// inline
	__forceinline bool GetFlag(MapFlags eFlag) const { return (m_uiFlags & eFlag) != 0; }
	/// inline
	__forceinline void SetFlag(MapFlags eFlag) { m_uiFlags |= eFlag; }
	/// inline
	__forceinline void ClearFlag(MapFlags eFlag) { m_uiFlags &= ~eFlag; } 
	/// inline
	__forceinline void SetFlag(MapFlags eFlag, bool bValue)
	{
		if( bValue ) m_uiFlags |= eFlag;
		else m_uiFlags &= ~eFlag;
	}

	uint m_uiFlags; ///< +0x2fc0

private:
	// Map loading for all versions of world builder maps
	/// 0x08c9b2d0
	bool Load0(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08c9ba52
	bool Load1(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08c9c0a8
	bool Load2(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08c9c696
	bool Load3(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08c9d4d2
	bool Load4(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08c9e33e
	bool Load5(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08c9f8b2
	bool Load6(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08ca0e1c
	bool Load7(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08ca23e4
	bool Load8(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08ca3bf0
	bool Load9(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08ca52cc
	bool Load10(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario);
	/// 0x08ca69a2
	bool Load11Plus(FIFile &kFile, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario, byte byFileVersion);


	/// size 0x8
	struct MapDescription
	{
		/// 0x08ca8d4a
		MapDescription();

		dword uiWidth; ///< +0x0
		dword uiHeight; ///< +0x4
	};

	/// size 0x54
	struct GameDescription
	{
		/// 0x08ca8d5c
		GameDescription();

		char szDefaultSpeed[64]; ///< +0x0

		uint uiStartingTurn; ///< +0x40
		uint uiMaxTurns; ///< +0x44
		uint uiTargetScore; ///< +0x48
		int iStartYear; ///< +0x4c

		byte byPlayerCount; ///< +0x50
		byte byCityStateCount; ///< +0x51
		byte byTeamCount; ///< +0x52
	};

	MapDescription m_kMapDesc; ///< +0x2fc4
	GameDescription m_kGameDesc; ///< +0x2fcc
	PlotMapData m_akMapData[MaxMapSize]; ///< +0x3020
	PlotScenarioData m_akScenarioData[MaxMapSize]; ///< +0x43020
	Player m_akPlayers[MaxPlayers]; ///< +0x83020
	Player m_akCityStates[MaxCityStates]; ///< +0x866a0
	Team m_akTeams[MaxTeams]; ///< +0x8d3a0

	//This should really go in either the MapDescription or GameDescription
	//but cannot for the following reasons:
	//GameDescription: Only used during scenarios
	//MapDescription: Read prior to any version handling
	char m_szWorldType[64]; ///< +0x8eba0

	/// size ?
	template<uint TBits>
	class SerializableBitset
	{
	public:
		/// inline
		SerializableBitset()
		{
			ZeroMemory(m_Mem, sizeof(m_Mem));
		}

		/// inline
		void Reset()
		{
			ZeroMemory(m_Mem, sizeof(m_Mem));
		}

		/// inline
		const bool Get(uint uiBitIndex) const
		{
			FAssertMsg(uiBitIndex < TBits, "Out of range");
			const uint uiByte = uiBitIndex / 8;
			const uint uiBit = uiBitIndex % 8;
			const byte byMask = (1 << uiBit);

			if( uiByte < ByteCount )
				return (m_Mem[uiByte] & byMask) != 0;
			else
				return false;
		}

		/// inline
		void Set(uint uiBitIndex, bool bVal)
		{
			FAssertMsg(uiBitIndex < TBits, "Out of range");
			const uint uiByte = uiBitIndex / 8;
			const uint uiBit = uiBitIndex % 8;
			const byte byMask = (1 << uiBit);

			if( uiByte < ByteCount )
			{
				if( bVal ) m_Mem[uiByte] |= byMask;
				else m_Mem[uiByte] &= ~byMask;
			}
		}

		/// inline
		static uint GetSerializedSize(uint uiBitCount)
		{
			return uiBitCount / 8 + ((uiBitCount % 8)? 1 : 0);
		}

		/// inline
		void Serialize(void *pvBuffer, uint uiBitCount) const
		{
			FAssert(uiBitCount < TBits);
			memcpy(pvBuffer, m_Mem, GetSerializedSize(uiBitCount));
		}

		/// inline
		void Deserialize(const void *pvBuffer, uint uiBitCount)
		{
			FAssert(uiBitCount < TBits);
			memcpy(m_Mem, pvBuffer, GetSerializedSize(uiBitCount));
		}

	private:
		static const uint ByteCount = TBits / 8 + ((TBits % 8)? 1 : 0);
		byte m_Mem[ByteCount]; ///< +0x0
	};

	SerializableBitset<MaxMapSize * MaxTeams> m_kVisibility; ///< +0x8ebe0

	// Return references to these when an out-of-range error occurs
	/// 0x0d266fb8
	static PlotMapData sm_kErrorPlotMapData;
	/// 0x0d266fc0
	static PlotScenarioData sm_kErrorPlotScenarioData;
	/// 0x0d266fc8
	static Player sm_kErrorPlayer;
	/// 0x0d26717c
	static Team sm_kErrorTeam;
};

#endif // CvWorldBuilderMap_h

/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   CIV 5 World Builder Map   ********************
//
//  FILE:    CvWorldBuilderMapTypeDesc.h
//
//  AUTHOR:  Eric Jordan  --  4/5/2010
//
//  PURPOSE: Describes what the type IDs used in a world build save represent.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#pragma once

#include "FirePlace/FireWorks/FFireTypes.h"
#include <algorithm>
#include <string.h>
#include <stddef.h>
#ifndef CvWorldBuilderMapTypeDesc_h
#define CvWorldBuilderMapTypeDesc_h

/// size 0x78
class CvWorldBuilderMapTypeDesc
{
public:
	/// 0x08cabc94
	CvWorldBuilderMapTypeDesc();

	// This special string type ensures that all strings used in
	// CvWorldBuilderMapTypeDesc come from the same string palette.
	// This makes for fast string compares and better memory usage.
	/// size 0x4
	class String
	{
	public:
		/// inline
		String() : m_sz(NULL) {}
		/// inline
		String(const String &sRhs) : m_sz(sRhs.m_sz) {}
		/// 0x08cabe2a
		String(_In_opt_z_ const char *sz);

		/// inline
		_Ret_opt_z_ operator const char*() const { return m_sz; }
		/// 0x08cabe64
		_Ret_opt_z_ const char *operator =(_In_opt_z_ const char *sz);

		// This pointer compare is possible because the strings come from the same palette
		/// inline
		const bool operator ==(const String &sRhs) const { return (m_sz == sRhs.m_sz); }
		/// inline
		const bool operator ==(_In_z_ const char *szRhs) const { return (strcmp(m_sz, szRhs) == 0); }

	private:
		const char *m_sz; ///< +0x0
	};

	/// size 0xc
	class TypeMap
	{
		typedef std::vector<String> TypeArray;
		TypeArray asTypes; ///< +0x0

	public:
		/// inline
		TypeMap(uint uiReserve = 0) { if (uiReserve > 0) asTypes.reserve(uiReserve); };

		/// 0x08cabe9e
		bool FindType(const char *szType, uint &uiTypeOut) const;
		/// 0x08cabefe
		const size_t GetSerializedSize() const;

		/// inline
		uint Size() const { return asTypes.size(); };
		/// inline
		void Clear() { asTypes.clear(); };
		/// 0x08cac03c
		const String& Get(uint uiIndex) const;
		/// 0x08cac0a6
		uint Add(const char* szType);
		/// 0x08cac118
		uint Add(const String &szType);

		// Obviously, the calling code is responsible for ensuring the buffer
		// is large enough.  GetSerializedSize() should be sufficient for this.
		/// 0x08cabf4a
		void Serialize(void *pvBuffer) const;

		/// 0x08cabf8c
		void Deserialize(void *pvBuffer, size_t stSize);
	};

	typedef TypeMap TerrainTypeMap;
	TerrainTypeMap m_kTerrainTypes; ///< +0x0

	typedef TypeMap FeatureTypeMap;
	FeatureTypeMap m_kFeatures; ///< +0xc

	typedef TypeMap NaturalWonderTypeMap;
	NaturalWonderTypeMap m_kNaturalWonders; ///< +0x18

	typedef TypeMap ResourceTypeMap;
	ResourceTypeMap m_kResources; ///< +0x24

	typedef TypeMap ImprovementTypeMap;
	ImprovementTypeMap m_kImprovements; ///< +0x30

	typedef TypeMap UnitTypeMap;
	UnitTypeMap m_kUnits; ///< +0x3c

	typedef TypeMap BuildingTypeMap;
	BuildingTypeMap m_kBuildingTypes; ///< +0x48

	typedef TypeMap TechTypeMap;
	TechTypeMap m_kTechs; ///< +0x54

	typedef TypeMap PolicyTypeMap;
	PolicyTypeMap m_kPolicies; ///< +0x60

	typedef TypeMap UnitPromotionTypeMap;
	UnitPromotionTypeMap m_kUnitPromotions; ///< +0x6c

	typedef std::vector<uint> LookupTable;
	/// inline
	static bool BuildLookupTable(const TypeMap &kFrom, const TypeMap &kTo, LookupTable& auiLookupTable)
	{
		// Initialize the lookup table
		uint uiMaxSize = MAX(kFrom.Size(), kTo.Size());
		auiLookupTable.resize(uiMaxSize);
		for( uint i = 0; i < uiMaxSize; ++i )
			auiLookupTable[i] = uint(-1);

		bool bDifferencesFound = kFrom.Size() != kTo.Size();
		for( uint uiFrom = 0; uiFrom < kFrom.Size(); ++uiFrom )
		{
			// I know this looks like a string copy but it's just a string pointer copy
			const String sFrom = kFrom.Get(uiFrom);

			// Note: NULL and empty strings are considered to represent invalid types and are never allowed to have a match.
			const char *szFrom = sFrom;
			if( szFrom && *szFrom )
			{
				for( uint uiTo = 0; uiTo < kTo.Size(); ++uiTo )
				{
					// I know this looks like a string compare but since they're tabled
					// strings it's just a pointer comparison which is pretty fast.
					if( sFrom == kTo.Get(uiTo) )
					{
						auiLookupTable[uiFrom] = uiTo;
						if( uiFrom != uiTo ) bDifferencesFound = true;
						break;
					}
				}
			}
			else
			{
				// If from was NULL or empty but to is not then a difference has been found.
				const char *szTo = kTo.Get(uiFrom);
				if( szTo && *szTo )
					bDifferencesFound = true;
			}
		}

		return bDifferencesFound;
	}
};

#endif // CvWorldBuilderMapTypeDesc_h

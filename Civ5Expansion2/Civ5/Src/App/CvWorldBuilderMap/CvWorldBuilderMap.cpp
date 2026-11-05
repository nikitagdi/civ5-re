/// origin binary
#include "Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMap.h"
#include "Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMapTypeDesc.h"
#include "FirePlace/FireWorks/FFileSystem.h"
#include "FirePlace/FireWorks/FIFile.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FTempHeap.h"
#include "FirePlace/FireWorks/FTimer.h"
#include <new>
#include <stddef.h>
#include <string.h>

// On-disk layout of the start of a map file; Load reads everything up to byPlayableCivCount.
#pragma pack(push, 1)
struct MapFileHeader
{
	byte byVersion; // low 7 bits: file version, 0x80: scenario data present
	dword uiWidth;
	dword uiHeight;
	byte byPlayableCivCount; // scenario files only
};
#pragma pack(pop)

// Second block of the file: sizes of the type tables and strings that follow it.
struct MapFileSizes
{
	uint uiFlags;
	uint uiTerrainTypesSize;
	uint uiFeaturesSize;
	uint uiNaturalWondersSize;
	uint uiResourcesSize;
	uint uiModDataSize;
	uint uiMapNameSize;
	uint uiMapDescriptionSize;
};

// Scenario block of the file; the first fields mirror CvWorldBuilderMap::GameDescription.
struct ScenarioFileHeader
{
	char szDefaultSpeed[64];
	uint uiStartingTurn;
	uint uiMaxTurns;
	uint uiTargetScore;
	int iStartYear;
	byte byPlayerCount;
	byte byCityStateCount;
	byte byTeamCount;
	uint uiImprovementsSize;
	uint uiUnitTypesSize;
	uint uiTechsSize;
	uint uiPoliciesSize;
	uint uiBuildingTypesSize;
	uint uiUnitPromotionsSize;
	uint uiUnitsSize;
	uint uiUnitNamesSize;
	uint uiCitiesSize;
	uint uiVictoryTypesSize;
	uint uiGameOptionsSize;
};

/// 0x08c99c7c
CvWorldBuilderMap::CvWorldBuilderMap()
	: m_uiFlags(FLAG_WORLD_WRAP | FLAG_TRANSFERABLE)
{
	ZeroMemory(m_aiCityStateInfluence, sizeof(m_aiCityStateInfluence));
	ZeroMemory(m_szWorldType, sizeof(m_szWorldType));
}

/// 0x08c9a158
CvWorldBuilderMap::~CvWorldBuilderMap()
{
}

/// 0x08c9a2f8
void CvWorldBuilderMap::Resize(uint uiWidth, uint uiHeight)
{
	if (uiHeight * uiWidth <= MaxMapSize && uiWidth != 0 && uiHeight != 0)
	{
		m_kMapDesc.uiWidth = uiWidth;
		m_kMapDesc.uiHeight = uiHeight;
	}
}

/// 0x08c9a326
bool CvWorldBuilderMap::Save(const wchar_t* wszFilename, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario) const
{
	FTimer kTimer;
	kTimer.Start();

	FFileInstance kFile(wszFilename, FIFile::modeWrite, 0, 0);
	if (!kFile->IsOpen())
		return false;

	bool bSaved = false;
	do
	{
		MapFileHeader kHeader;
		kHeader.byVersion = FileVersion;
		kHeader.byPlayableCivCount = 0;
		if (bScenario)
		{
			kHeader.byVersion |= 0x80;
			for (uint i = 0; i < m_kGameDesc.byPlayerCount; ++i)
				kHeader.byPlayableCivCount += m_akPlayers[i].m_bPlayable;
		}
		kHeader.uiWidth = m_kMapDesc.uiWidth;
		kHeader.uiHeight = m_kMapDesc.uiHeight;
		if (kFile->Write(&kHeader, sizeof(kHeader)) < sizeof(kHeader))
			break;

		MapFileSizes kSizes;
		kSizes.uiFlags = m_uiFlags;
		kSizes.uiTerrainTypesSize = kTypeDesc.m_kTerrainTypes.GetSerializedSize();
		kSizes.uiFeaturesSize = kTypeDesc.m_kFeatures.GetSerializedSize();
		kSizes.uiNaturalWondersSize = kTypeDesc.m_kNaturalWonders.GetSerializedSize();
		kSizes.uiResourcesSize = kTypeDesc.m_kResources.GetSerializedSize();
		kSizes.uiMapNameSize = m_sMapName.GetLength() + 1;
		kSizes.uiMapDescriptionSize = m_sMapDescription.GetLength() + 1;
		if (m_kModData.m_kMapModData.GetFieldCount() == 0 && m_kModData.m_kPlotModData.GetFieldCount() == 0)
			kSizes.uiModDataSize = 0;
		else
			kSizes.uiModDataSize = m_kModData.GetSerializedSize();
		if (kFile->Write(&kSizes, sizeof(kSizes)) < sizeof(kSizes))
			break;

		const uint uiTypesSize = kSizes.uiTerrainTypesSize + kSizes.uiFeaturesSize + kSizes.uiNaturalWondersSize
			+ kSizes.uiResourcesSize + kSizes.uiModDataSize + kSizes.uiMapNameSize + kSizes.uiMapDescriptionSize;
		byte *pbyTypes = (byte*)FTempHeapFastVectorAllocator::AllocAligned(uiTypesSize, 4, c_eMPoolTypeGame, 0);
		byte *pbyCursor = pbyTypes;
		kTypeDesc.m_kTerrainTypes.Serialize(pbyCursor);
		pbyCursor += kSizes.uiTerrainTypesSize;
		kTypeDesc.m_kFeatures.Serialize(pbyCursor);
		pbyCursor += kSizes.uiFeaturesSize;
		kTypeDesc.m_kNaturalWonders.Serialize(pbyCursor);
		pbyCursor += kSizes.uiNaturalWondersSize;
		kTypeDesc.m_kResources.Serialize(pbyCursor);
		pbyCursor += kSizes.uiResourcesSize;
		if (kSizes.uiModDataSize != 0)
		{
			m_kModData.Serialize(pbyCursor);
			pbyCursor += kSizes.uiModDataSize;
		}
		memcpy(pbyCursor, (const char*)m_sMapName, kSizes.uiMapNameSize);
		pbyCursor[kSizes.uiMapNameSize - 1] = 0;
		pbyCursor += kSizes.uiMapNameSize;
		memcpy(pbyCursor, (const char*)m_sMapDescription, kSizes.uiMapDescriptionSize);
		pbyCursor[kSizes.uiMapDescriptionSize - 1] = 0;
		const uint uiTypesWritten = kFile->Write(pbyTypes, uiTypesSize);
		FTempHeapFastVectorAllocator::FreeAligned(pbyTypes);
		if (uiTypesWritten < uiTypesSize)
			break;

		const uint uiWorldTypeSize = sizeof(m_szWorldType);
		kFile->Write(&uiWorldTypeSize, sizeof(uiWorldTypeSize));
		kFile->Write(m_szWorldType, sizeof(m_szWorldType));

		const uint uiPlotCount = m_kMapDesc.uiHeight * m_kMapDesc.uiWidth;
		const uint uiPlotDataSize = uiPlotCount * sizeof(PlotMapData);
		if (kFile->Write(m_akMapData, uiPlotDataSize) < uiPlotDataSize)
			break;

		if (bScenario)
		{
			ScenarioFileHeader kScenario;
			memcpy(kScenario.szDefaultSpeed, m_kGameDesc.szDefaultSpeed, sizeof(kScenario.szDefaultSpeed));
			kScenario.uiStartingTurn = m_kGameDesc.uiStartingTurn;
			kScenario.uiMaxTurns = m_kGameDesc.uiMaxTurns;
			kScenario.uiTargetScore = m_kGameDesc.uiTargetScore;
			kScenario.iStartYear = m_kGameDesc.iStartYear;
			kScenario.byPlayerCount = m_kGameDesc.byPlayerCount;
			kScenario.byCityStateCount = m_kGameDesc.byCityStateCount;
			kScenario.byTeamCount = m_kGameDesc.byTeamCount;
			kScenario.uiImprovementsSize = kTypeDesc.m_kImprovements.GetSerializedSize();
			kScenario.uiUnitTypesSize = kTypeDesc.m_kUnits.GetSerializedSize();
			kScenario.uiTechsSize = kTypeDesc.m_kTechs.GetSerializedSize();
			kScenario.uiPoliciesSize = kTypeDesc.m_kPolicies.GetSerializedSize();
			kScenario.uiBuildingTypesSize = kTypeDesc.m_kBuildingTypes.GetSerializedSize();
			kScenario.uiUnitPromotionsSize = kTypeDesc.m_kUnitPromotions.GetSerializedSize();
			kScenario.uiUnitsSize = m_kUnits.GetSerializedSize();
			kScenario.uiUnitNamesSize = m_kUnitNames.GetSerializedSize();
			kScenario.uiCitiesSize = m_kCities.GetSerializedSize();
			kScenario.uiVictoryTypesSize = m_kVictoryTypes.GetSerializedSize();
			kScenario.uiGameOptionsSize = m_kGameOptions.GetSerializedSize();
			// Unit type tables are only needed when there are units to refer to them
			if (kScenario.uiUnitsSize == 0)
			{
				kScenario.uiUnitTypesSize = 0;
				kScenario.uiUnitPromotionsSize = 0;
			}
			if (kFile->Write(&kScenario, sizeof(kScenario)) < sizeof(kScenario))
				break;

			const uint uiTeamCount = kScenario.byTeamCount;
			const uint uiRelationshipSize = FRelationshipBitset<MaxTeams, true>::GetSerializedSize(uiTeamCount);
			const uint uiInfluenceSize = kScenario.byPlayerCount * sizeof(m_aiCityStateInfluence[0]);
			const uint uiVisibilityBitCount = uiTeamCount * uiPlotCount;
			const uint uiVisibilitySize = SerializableBitset<MaxMapSize * MaxTeams>::GetSerializedSize(uiVisibilityBitCount);
			const uint uiScenarioSize = kScenario.uiImprovementsSize + kScenario.uiUnitTypesSize + kScenario.uiTechsSize
				+ kScenario.uiPoliciesSize + kScenario.uiBuildingTypesSize + kScenario.uiUnitPromotionsSize
				+ kScenario.uiUnitsSize + kScenario.uiUnitNamesSize + kScenario.uiCitiesSize
				+ kScenario.uiVictoryTypesSize + kScenario.uiGameOptionsSize
				+ uiInfluenceSize + uiVisibilitySize + 5 * uiRelationshipSize;

			byte *pbyScenario = (byte*)FTempHeapFastVectorAllocator::AllocAligned(uiScenarioSize, 4, c_eMPoolTypeGame, 0);
			pbyCursor = pbyScenario;
			kTypeDesc.m_kImprovements.Serialize(pbyCursor);
			pbyCursor += kScenario.uiImprovementsSize;
			if (kScenario.uiUnitTypesSize != 0)
			{
				kTypeDesc.m_kUnits.Serialize(pbyCursor);
				pbyCursor += kScenario.uiUnitTypesSize;
			}
			kTypeDesc.m_kTechs.Serialize(pbyCursor);
			pbyCursor += kScenario.uiTechsSize;
			kTypeDesc.m_kPolicies.Serialize(pbyCursor);
			pbyCursor += kScenario.uiPoliciesSize;
			kTypeDesc.m_kBuildingTypes.Serialize(pbyCursor);
			pbyCursor += kScenario.uiBuildingTypesSize;
			if (kScenario.uiUnitPromotionsSize != 0)
			{
				kTypeDesc.m_kUnitPromotions.Serialize(pbyCursor);
				pbyCursor += kScenario.uiUnitPromotionsSize;
			}
			m_kUnits.Serialize(pbyCursor);
			pbyCursor += kScenario.uiUnitsSize;
			m_kUnitNames.Serialize(pbyCursor);
			pbyCursor += kScenario.uiUnitNamesSize;
			m_kCities.Serialize(pbyCursor);
			pbyCursor += kScenario.uiCitiesSize;
			m_kVictoryTypes.Serialize(pbyCursor);
			pbyCursor += kScenario.uiVictoryTypesSize;
			m_kGameOptions.Serialize(pbyCursor);
			pbyCursor += kScenario.uiGameOptionsSize;
			m_kTeamsInContact.Serialize(pbyCursor, uiTeamCount);
			pbyCursor += uiRelationshipSize;
			m_kTeamsAtWar.Serialize(pbyCursor, uiTeamCount);
			pbyCursor += uiRelationshipSize;
			m_kTeamsAtPermanentWarOrPeace.Serialize(pbyCursor, uiTeamCount);
			pbyCursor += uiRelationshipSize;
			m_kTeamsSharingOpenBorders.Serialize(pbyCursor, uiTeamCount);
			pbyCursor += uiRelationshipSize;
			m_kTeamsSharingDefensivePacts.Serialize(pbyCursor, uiTeamCount);
			pbyCursor += uiRelationshipSize;
			memcpy(pbyCursor, m_aiCityStateInfluence, uiInfluenceSize);
			pbyCursor += uiInfluenceSize;
			m_kVisibility.Serialize(pbyCursor, uiVisibilityBitCount);
			const uint uiScenarioWritten = kFile->Write(pbyScenario, uiScenarioSize);
			FTempHeapFastVectorAllocator::FreeAligned(pbyScenario);
			if (uiScenarioWritten < uiScenarioSize)
				break;

			const uint uiTeamsSize = kScenario.byTeamCount * sizeof(Team);
			if (kFile->Write(m_akTeams, uiTeamsSize) < uiTeamsSize)
				break;
			const uint uiPlayersSize = kScenario.byPlayerCount * sizeof(Player);
			if (kFile->Write(m_akPlayers, uiPlayersSize) < uiPlayersSize)
				break;
			const uint uiCityStatesSize = kScenario.byCityStateCount * sizeof(Player);
			if (kFile->Write(m_akCityStates, uiCityStatesSize) < uiCityStatesSize)
				break;
			const uint uiScenarioPlotDataSize = uiPlotCount * sizeof(PlotScenarioData);
			if (kFile->Write(m_akScenarioData, uiScenarioPlotDataSize) < uiScenarioPlotDataSize)
				break;
		}

		bSaved = true;
	}
	while (false);

	kFile->Close();
	if (!bSaved)
	{
		FFileSystem::GetInstance().Delete(wszFilename);
		return false;
	}

	kTimer.Stop();
	return true;
}

/// 0x08c9af0a
bool CvWorldBuilderMap::Load(const wchar_t* wszFilename, const CvWorldBuilderMapTypeDesc &kTypeDesc, bool bScenario)
{
	FFileInstance kFile(wszFilename, FIFile::modeRead, 0, 0);
	if (!kFile->IsOpen())
		return false;

	MapFileHeader kHeader;
	const uint uiHeaderSize = offsetof(MapFileHeader, byPlayableCivCount);
	bool bLoaded = false;
	if (kFile->Read(&kHeader, uiHeaderSize) >= uiHeaderSize && kHeader.uiHeight * kHeader.uiWidth <= MaxMapSize)
	{
		m_kMapDesc.uiWidth = kHeader.uiWidth;
		m_kMapDesc.uiHeight = kHeader.uiHeight;
		m_sMapName.Empty();
		m_sMapDescription.Empty();

		const byte byFileVersion = kHeader.byVersion & 0x7f;
		const bool bFileHasScenario = (kHeader.byVersion & 0x80) != 0;
		ZeroMemory(m_szWorldType, sizeof(m_szWorldType));
		if (byFileVersion <= FileVersion)
		{
			const bool bLoadScenario = bFileHasScenario && bScenario;
			switch (byFileVersion)
			{
			case 0:
				bLoaded = Load0(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 1:
				bLoaded = Load1(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 2:
				bLoaded = Load2(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 3:
				bLoaded = Load3(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 4:
				bLoaded = Load4(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 5:
				bLoaded = Load5(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 6:
				bLoaded = Load6(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 7:
				bLoaded = Load7(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 8:
				bLoaded = Load8(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 9:
				bLoaded = Load9(*kFile, kTypeDesc, bLoadScenario);
				break;
			case 10:
				bLoaded = Load10(*kFile, kTypeDesc, bLoadScenario);
				break;
			default:
				bLoaded = Load11Plus(*kFile, kTypeDesc, bLoadScenario, byFileVersion);
				break;
			}

			if (bLoaded && !bFileHasScenario && bScenario)
				ClearScenarioData(false);
		}
	}

	kFile->Close();
	return bLoaded;
}

/// 0x08ca81f4
void CvWorldBuilderMap::ClearScenarioData(bool bPreserveImprovements)
{
	const uint uiPlotCount = m_kMapDesc.uiWidth * m_kMapDesc.uiHeight;
	if (bPreserveImprovements)
	{
		for (uint i = 0; i < uiPlotCount; ++i)
		{
			PlotScenarioData &kPlot = m_akScenarioData[i];
			const byte byImprovement = kPlot.m_byImprovement;
			kPlot.m_byCulture = PlotScenarioData::InvalidCulture;
			kPlot.m_byImprovement = PlotScenarioData::InvalidImprovement;
			kPlot.m_byRoute = PlotScenarioData::InvalidRoute;
			kPlot.m_byRouteOwner = byte(-1);
			kPlot.m_hCity = City::Handle::InvalidHandle();
			kPlot.m_hUnitStack = Unit::Handle::InvalidHandle();
			kPlot.m_byImprovement = byImprovement;
		}
	}
	else if (uiPlotCount != 0)
	{
		memset(m_akScenarioData, 0xff, uiPlotCount * sizeof(PlotScenarioData));
	}

	m_kGameDesc.uiStartingTurn = 0;
	m_kGameDesc.uiMaxTurns = 0;
	m_kGameDesc.uiTargetScore = 0;
	m_kGameDesc.iStartYear = -4000;
	m_kGameDesc.byPlayerCount = 0;
	m_kGameDesc.byCityStateCount = 0;
	m_kGameDesc.byTeamCount = 0;
	m_kGameDesc.szDefaultSpeed[0] = 0;

	m_kUnits.Release();
	m_kUnitNames.Release();
	m_kCities.Release();
	m_kVictoryTypes.Clear();
	m_kGameOptions.Clear();
	m_kVisibility.Reset();
	ZeroMemory(m_aiCityStateInfluence, sizeof(m_aiCityStateInfluence));
}

/// 0x08ca856a
void CvWorldBuilderMap::SetWorldType(const char* szWorldType)
{
	if (szWorldType && *szWorldType && strlen(szWorldType) < sizeof(m_szWorldType))
	{
		strcpy_s(m_szWorldType, sizeof(m_szWorldType), szWorldType);
		return;
	}
	ZeroMemory(m_szWorldType, sizeof(m_szWorldType));
}

/// 0x08ca8660
void CvWorldBuilderMap::SwapPlayers(byte byPlayer1, byte byPlayer2)
{
	if (byPlayer1 >= m_kGameDesc.byPlayerCount || byPlayer2 >= m_kGameDesc.byPlayerCount || byPlayer1 == byPlayer2)
		return;

	const Player kTemp = m_akPlayers[byPlayer1];
	m_akPlayers[byPlayer1] = m_akPlayers[byPlayer2];
	m_akPlayers[byPlayer2] = kTemp;

	// Every owner reference on the map has to follow the swap
	const auto SwapOwner = [](byte &byOwner, byte byFirst, byte bySecond)
	{
		if (byOwner == byFirst)
			byOwner = bySecond;
		else if (byOwner == bySecond)
			byOwner = byFirst;
	};
	const uint uiPlotCount = m_kMapDesc.uiWidth * m_kMapDesc.uiHeight;
	for (uint uiPlot = 0; uiPlot < uiPlotCount; ++uiPlot)
	{
		PlotScenarioData &kPlot = GetPlotScenarioData(uiPlot);

		SwapOwner(kPlot.m_byCulture, byPlayer1, byPlayer2);
		SwapOwner(kPlot.m_byRouteOwner, byPlayer1, byPlayer2);

		City *pkCity = m_kCities[kPlot.m_hCity];
		if (pkCity != NULL)
			SwapOwner(pkCity->m_byOwner, byPlayer1, byPlayer2);

		for (Unit *pkUnit = m_kUnits[kPlot.m_hUnitStack]; pkUnit != NULL; pkUnit = m_kUnits[pkUnit->m_hStackedUnit])
			SwapOwner(pkUnit->m_byOwner, byPlayer1, byPlayer2);
	}

	for (uint i = 0; i < m_kGameDesc.byCityStateCount; ++i)
		std::swap(m_aiCityStateInfluence[byPlayer1][i], m_aiCityStateInfluence[byPlayer2][i]);
}

/// 0x08ca8a9e
uint CvWorldBuilderMap::PreviewPlayableCivCount(const wchar_t* wszFilename)
{
	FFileInstance kFile(wszFilename, FIFile::modeRead, 0, 0);
	uint uiPlayableCount = 0;
	if (kFile->IsOpen())
	{
		MapFileHeader kHeader;
		const uint uiRead = kFile->Read(&kHeader, sizeof(kHeader));
		kFile->Close();

		const byte byFileVersion = kHeader.byVersion & 0x7f;
		// Only scenario files from version 3 on carry a playable civ count
		if (uiRead == sizeof(kHeader) && (kHeader.byVersion & 0x80) != 0 && byFileVersion >= 3)
		{
			if (byFileVersion <= 4)
			{
				// Versions 3 and 4 do not store the count in the header, so the whole map is loaded
				void *pvMem = FTempHeapFastVectorAllocator::AllocAligned(sizeof(CvWorldBuilderMap), 4, c_eMPoolTypeGame, 0);
				if (pvMem != NULL)
				{
					CvWorldBuilderMap *pkMap = new(pvMem) CvWorldBuilderMap();
					{
						CvWorldBuilderMapTypeDesc kTypeDesc;
						pkMap->Load(wszFilename, kTypeDesc, true);
						for (byte byPlayer = 0; byPlayer < pkMap->GetPlayerCount(); ++byPlayer)
							uiPlayableCount += pkMap->GetPlayer(byPlayer).m_bPlayable;
					}
					pkMap->~CvWorldBuilderMap();
					FTempHeapFastVectorAllocator::FreeAligned(pkMap);
				}
			}
			else
			{
				uiPlayableCount = kHeader.byPlayableCivCount;
			}
		}
	}
	return uiPlayableCount;
}

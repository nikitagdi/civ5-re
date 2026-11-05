/// origin binary
#include "Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMapTypeDesc.h"

/// 0x08cabc94
CvWorldBuilderMapTypeDesc::CvWorldBuilderMapTypeDesc()
	: m_kTerrainTypes(0x20),
	  m_kFeatures(0x80),
	  m_kNaturalWonders(0x80),
	  m_kResources(0x40),
	  m_kImprovements(0x40),
	  m_kUnits(0x100),
	  m_kBuildingTypes(0x100),
	  m_kTechs(0x100),
	  m_kPolicies(0x100),
	  m_kUnitPromotions(0x100)
{
}

/// 0x08cac0a6
uint CvWorldBuilderMapTypeDesc::TypeMap::Add(const char* szType)
{
	asTypes.push_back(String(szType));
	return asTypes.size() - 1;
}

/// origin binary
#pragma once 

#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FFireTypes.h"
#include "FirePlace/FireWorks/FFreeListArrayBase.h"

/// size 0x10
struct CvColorA 
{
	/// inline
	CvColorA(float fr, float fg, float fb, float fa) : r(fr), g(fg), b(fb), a(fa) {}
	/// inline
	CvColorA() : r(0.0f), g(0.0f), b(0.0f), a(0.0f)  {}
	float r; ///< +0x0
	float g; ///< +0x4
	float b; ///< +0x8
	float a; ///< +0xc
};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  CLASS:      CvCombatMemberEntry
//!  \brief		A single member in combat.
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size 0x24
struct CvCombatMemberEntry {
	PlayerTypes		m_ePlayer; ///< +0x0
	int				m_iID; ///< +0x4
	uint16			m_bfFlags; ///< +0x8

	enum INFO_FLAGS
	{
		INFO_FLAG_IS_CITY	= (1 << 0)
	};
public:

	enum MEMBER_TYPE
	{
		MEMBER_UNIT,
		MEMBER_CITY
	};

	// Amount of damage for this combat
	int16			m_iDamage; ///< +0xa
	// Amount of unit damage in total (damage this combat + previous damage)
	int16			m_iFinalDamage; ///< +0xc
	int16			m_iMaxHitPoints; ///< +0xe
	// Plot X
	int16			m_iX; ///< +0x10
	// Plot X
	int16			m_iY; ///< +0x12
	UnitCombatTypes	m_eCombatType; ///< +0x14
	// Era type for the unit/city
	EraTypes		m_eEra; ///< +0x18

	// Used for cities
	uint32			m_uiWeaponType; ///< +0x1c
	// Used for cities
	float			m_fWeaponOption; ///< +0x20

	/// inline
	PlayerTypes GetPlayer() const { return m_ePlayer; }
	/// inline
	void		SetPlayer(PlayerTypes ePlayer) { m_ePlayer = ePlayer; }

	/// inline
	bool		IsValid() const { return m_ePlayer != NO_PLAYER && m_iID != -1; }
	/// inline
	int			GetID() const { return m_iID; }
	/// inline
	void		SetID(int iID, MEMBER_TYPE eType) 
	{ 
		m_iID = iID; 
		if (eType == MEMBER_UNIT)
			m_bfFlags &= ~(uint)INFO_FLAG_IS_CITY; 
		else
			m_bfFlags |= (uint)INFO_FLAG_IS_CITY; 
	}

	/// inline
	bool		IsUnit() const { return IsValid() && (m_bfFlags & (uint)INFO_FLAG_IS_CITY) == 0; }
	/// inline
	int			GetUnitID() const { if (IsUnit()) return m_iID; else return -1; }
	/// inline
	void		SetUnitID(int iID) { m_iID = iID; m_bfFlags &= ~(uint)INFO_FLAG_IS_CITY; }

	/// inline
	bool		IsCity() const { return IsValid() && (m_bfFlags & (uint)INFO_FLAG_IS_CITY) != 0; }
	/// inline
	int			GetCityID() const { if (IsCity()) return m_iID; else return -1; }
	/// inline
	void		SetCityID(int iID) { m_iID = iID; m_bfFlags |= (uint)INFO_FLAG_IS_CITY; }

	/// inline
	bool		IsType(MEMBER_TYPE eType) const 
	{ 
		if (IsValid())
		{
			if ((m_bfFlags & (uint)INFO_FLAG_IS_CITY) != 0)
				return eType == MEMBER_CITY;
			else
				return eType == MEMBER_UNIT;
		}

		return false;
	}
	/// inline
	int			GetDamage() const { return (int)m_iDamage; }
	/// inline
	void		SetDamage(int iDamage) { m_iDamage = (int16)iDamage; }

	/// inline
	int			GetFinalDamage() const { return (int)m_iFinalDamage; }
	/// inline
	void		SetFinalDamage(int iFinalDamage) { m_iFinalDamage = (int16)iFinalDamage; }

	/// inline
	int			GetMaxHitPoints() const { return (int)m_iMaxHitPoints; }
	/// inline
	void		SetMaxHitPoints(int iMaxHitPoints) { m_iMaxHitPoints = (int16)iMaxHitPoints; }

	/// inline
	UnitCombatTypes GetCombatType() const { return m_eCombatType; }
	/// inline
	void		SetCombatType(UnitCombatTypes eCombatType) { m_eCombatType = eCombatType; }

	/// inline
	EraTypes	GetEra() const { return m_eEra; }
	/// inline
	void		SetEra(EraTypes eEra) { m_eEra = eEra; }

	/// inline
	int			GetX() const { return m_iX; }
	/// inline
	int			GetY() const { return m_iY; }
	/// inline
	void		SetPlot(int32 iX, int32 iY) { m_iX = (int16)iX; m_iY = (int16)iY; }

	/// inline
	uint32		GetWeaponType() const { return m_uiWeaponType; }
	/// inline
	void		SetWeaponType(uint32 uiWeaponType) { m_uiWeaponType = uiWeaponType; }

	/// inline
	float		GetWeaponOption() const { return m_fWeaponOption; }
	/// inline
	void		SetWeaponOption(float fWeaponOption) { m_fWeaponOption = fWeaponOption; }

	/// inline
	CvCombatMemberEntry() : m_ePlayer(NO_PLAYER), m_iID(-1), m_bfFlags(0), m_iDamage(0), m_iFinalDamage(0), m_iMaxHitPoints(0), m_eCombatType(NO_UNITCOMBAT), m_eEra(NO_ERA), m_iX(0), m_iY(0), m_uiWeaponType(0), m_fWeaponOption(0.f) {}
};

#define POPUP_PARAM_INT_ARRAY(x)	sizeof(x)/sizeof(int), &x[0]
#define POPUP_PARAM_BOOL_ARRAY(x)	sizeof(x)/sizeof(bool), &x[0]

//------------------------------------------------------------------------------
/// size 0x218
struct CvPopupInfo
{
public:
	/// inline
	CvPopupInfo(ButtonPopupTypes buttonPopupType = BUTTONPOPUP_TEXT, int data1 = -1, int data2 = -1, int data3 = -1, int flags = 0, bool option1 = false, bool option2 = false)
		: iData1(data1)
		, iData2(data2)
		, iData3(data3)
		, iFlags(flags)
		, bOption1(option1)
		, bOption2(option2)
		, eButtonPopupType(buttonPopupType)
	{
		//Nothing
		szText[0] = 0;
	}
	
	int iData1; ///< +0x0
	int iData2; ///< +0x4
	int iData3; ///< +0x8
	int iFlags; ///< +0xc
	bool bOption1; ///< +0x10
	bool bOption2; ///< +0x11
	ButtonPopupTypes eButtonPopupType; ///< +0x14
	char szText[512]; ///< +0x18
};

/// size 0x14
struct CvMapInitData
{
	int m_iGridW; ///< +0x0
	int m_iGridH; ///< +0x4
	int m_iTopLatitude; ///< +0x8
	int m_iBottomLatitude; ///< +0xc

	bool m_bWrapX; ///< +0x10
	bool m_bWrapY; ///< +0x11

	/// inline
	CvMapInitData(int iGridW = 0, int iGridH = 0, int iTopLatitude = 90, int iBottomLatitude = -90, bool bWrapX = true, bool bWrapY = false) :
	m_iGridH(iGridH),m_iGridW(iGridW),m_iTopLatitude(iTopLatitude),m_iBottomLatitude(iBottomLatitude),m_bWrapY(bWrapY),m_bWrapX(bWrapX)
	{ }
};

//------------------------------------------------------------------------------
/// size 0x1c
struct CvWorldBuilderMapLoaderMapInfo
{
	unsigned int uiWidth; ///< +0x0
	unsigned int uiHeight; ///< +0x4
	unsigned int uiPlayers; ///< +0x8
	unsigned int uiCityStates; ///< +0xc
	unsigned int uiMajorCivStartingPositions; ///< +0x10
	unsigned int uiMinorCivStartingPositions; ///< +0x14
	bool bWorldWrap; ///< +0x18
	bool bRandomResources; ///< +0x19

	/// inline
	CvWorldBuilderMapLoaderMapInfo() 
		: uiWidth(0)
		, uiHeight(0)
		, uiPlayers(0)
		, uiCityStates(0)
		, uiMajorCivStartingPositions(0)
		, uiMinorCivStartingPositions(0)
		, bWorldWrap(false)
		, bRandomResources(false)
	{

	}
};
//------------------------------------------------------------------------------
/// size 0x8
struct IDInfo
{
	/// inline
	IDInfo(PlayerTypes eOwner=NO_PLAYER, int iID=FFreeList::INVALID_INDEX) : eOwner(eOwner), iID(iID) {}
	PlayerTypes eOwner; ///< +0x0
	int iID; ///< +0x4

	/// inline
	bool operator== (const IDInfo& info) const
	{
		return (eOwner == info.eOwner && iID == info.iID);
	}

	/// inline
	bool operator !=(const IDInfo& other) const
	{
		return (eOwner != other.eOwner || iID != other.iID);
	}

	/// inline
	void reset()
	{
		eOwner = NO_PLAYER;
		iID = FFreeList::INVALID_INDEX;
	}

	/// inline
	bool isInvalid() const
	{
		return eOwner == NO_PLAYER && iID == FFreeList::INVALID_INDEX;
	}
};
//------------------------------------------------------------------------------
/// size 0x1c
struct UnitMoveRate 
{
	/// inline
	UnitMoveRate():m_fTotalMoveRate(1.0f),m_fEaseIn(0.0f),m_fEaseOut(0.0f),m_fIndividualOffset(0.0f), m_fRowOffset(0.0f), m_fCurveRoll(0.0f), m_iPathSubdivision(1)
	{
	}

	float m_fTotalMoveRate; ///< +0x0
	float m_fEaseIn; ///< +0x4
	float m_fEaseOut; ///< +0x8
	float m_fIndividualOffset; ///< +0xc
	float m_fRowOffset; ///< +0x10
	float m_fCurveRoll; ///< +0x14
	int   m_iPathSubdivision; ///< +0x18
};


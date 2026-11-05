/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   CIV 5 World Builder Map   ********************
//
//  FILE:    CvWorldBuilderMapModData.h
//
//  AUTHOR:  Eric Jordan  --  5/13/2010
//
//  PURPOSE:
//		This little monstrosity was created to support the Civ 5 World Builder map format.
//		It's designed to allow modders to inject custom data formats into their map files.
//
//      If you are a modder looking at this then run!  You can access this functionality
//      from the world builder UI and script.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2010 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#pragma once

#include "FirePlace/FireWorks/FFireTypes.h"
#include <stddef.h>
#include "FirePlace/FireWorks/FFastVector.h"
#ifndef CvWorldBuilderMapModData_h
#define CvWorldBuilderMapModData_h

#include "FirePlace/FireWorks/FUncopyable.h"

class CvWorldBuilderMapModData;
class ModType;

/// size 0xc
class ModDataStringBuffer : FUncopyable
{
	friend CvWorldBuilderMapModData;

public:
	/// 0x08caa6fe
	uint AddString(const char *sz);
	/// 0x08caa7de
	_Ret_z_ const char *GetString(uint uiStringID) const;

private:
	/// 0x08caa66c
	ModDataStringBuffer();
	/// 0x08caa686
	~ModDataStringBuffer();

	/// 0x08caa6c2
	void Clear();

	/// 0x08caa802
	size_t GetSerializedSize() const;
	/// 0x08caa80c
	void Serialize(void *pv) const;
	/// 0x08caa84a
	void Deserialize(const void *pv);

	uint m_uiSize; ///< +0x0
	uint m_uiReservedSize; ///< +0x4
	char *m_szBuffer; ///< +0x8
};

/// size 0x34
class ModEnum
{
	friend ModType;

public:
	/// 0x08caa8d0
	ModEnum();
	/// 0x08caa910
	ModEnum(const ModEnum &kRhs);
	/// inline
	~ModEnum() {}

	/// 0x08caa944
	void AddMember(const char *szMember);

	/// 0x08caa990
	uint GetMemberCount() const;
	/// 0x08caa998
	_Ret_opt_z_ const char *GetMember(uint uiMember) const;

private:
	/// 0x08caa8f0
	ModEnum(ModDataStringBuffer &kStringBuffer);

	/// 0x08caa9d4
	size_t GetSerializedSize() const;
	/// 0x08caa9e4
	void Serialize(void *pv) const;
	/// 0x08caaa24
	void Deserialize(const void *pv);

	ModDataStringBuffer *m_pkStringBuffer; ///< +0x0
	FStaticVector<uint, 8, true> m_kMembers; ///< +0x4
};

/// size 0x8
class ModDataEntry
{
	friend CvWorldBuilderMapModData;

public:
	/// 0x08cab044
	void SetField(uint uiField, bool bValue);
	/// 0x08cab058
	void SetField(uint uiField, int iValue);
	/// 0x08cab06c
	void SetField(uint uiField, float fValue);
	/// 0x08cab084
	void SetField(uint uiField, const char *szValue);
	/// 0x08cab0ca
	void SetFieldToDefault(uint uiField);

	/// 0x08cab0e8
	bool GetFieldAsBool(uint uiField) const;
	/// 0x08cab0fe
	int GetFieldAsInt(uint uiField) const;
	/// 0x08cab10e
	float GetFieldAsFloat(uint uiField) const;
	/// 0x08cab11e
	_Ret_z_ const char *GetFieldAsString(uint uiField) const;
	/// 0x08cab150
	bool IsFieldDefaultValue(uint uiField) const;

private:
	/// 0x08cab032
	ModDataEntry(ModType &kModType, dword *aFields);

	// There is no definition for this as this class is not supposed to use assignment
	/// absent
	const ModDataEntry &operator=(const ModDataEntry &kRhs);

	ModType &m_kModType; ///< +0x0
	dword *m_aFields; ///< +0x4
};

/// size 0x224
class ModType
{
	friend CvWorldBuilderMapModData;
	friend ModDataEntry;

public:
	enum FieldType
	{
		TYPE_INVALID = -1,
		TYPE_BOOL = 0,
		TYPE_FLOAT,
		TYPE_STRING,
		TYPE_ENUM // Must be last
	};

	/// 0x08caab18
	void AddField(const char *szName, bool bDefaultValue);
	/// 0x08caab6e
	void AddField(const char *szName, float fDefaultValue);
	/// 0x08caabc6
	void AddField(const char *szName, const char *szDefaultValue);
	/// 0x08caac2a
	ModEnum &AddEnumField(const char *szName, int iDefaultValue);

	/// 0x08caace4
	uint GetFieldCount() const;

	/// 0x08caacec
	_Ret_opt_z_ const char *GetFieldName(uint uiField) const;
	/// 0x08caad28
	FieldType GetFieldType(uint uiField) const;

	/// 0x08caad50
	bool GetFieldDefaultAsBool(uint uiField) const;
	/// 0x08caad74
	int GetFieldDefaultAsInt(uint uiField) const;
	/// 0x08caad8e
	float GetFieldDefaultAsFloat(uint uiField) const;
	/// 0x08caadb6
	const char *GetFieldDefaultAsString(uint uiField) const;

	/// 0x08caadee
	_Ret_opt_ const ModEnum *GetFieldEnumType(uint uiField) const;

private:
	/// size 0xc
	struct Field
	{
		dword dwFieldType; ///< +0x0
		// String ID
		dword dwFieldName; ///< +0x4
		dword dwDefault; ///< +0x8
	};

	/// 0x08caaa84
	ModType(ModDataStringBuffer &kStringBuffer);
	/// 0x08caaaba
	ModType(const ModType &kRhs);
	/// inline
	~ModType() {}

	// There is no definition for this as this class is not supposed to use assignment
	/// absent
	const ModType &operator=(const ModType &kRhs);

	/// 0x08caae1c
	void Clear();

	/// 0x08caae48
	size_t GetSerializedSize() const;
	/// 0x08caae7c
	void Serialize(void *pv) const;
	/// 0x08caaf1e
	void Deserialize(const void *pv);

	ModDataStringBuffer &m_kStringBuffer; ///< +0x0
	FStaticVector<Field, 8, true> m_kFields; ///< +0x4
	FStaticVector<ModEnum, 8> m_kEnums; ///< +0x74
};

/// size 0x460
class CvWorldBuilderMapModData : FUncopyable
{
	friend ModType;
	friend ModEnum;

public:
	/// 0x08cab172
	CvWorldBuilderMapModData();

	/// 0x08cab22c
	void InitDataEntries(uint uiPlotCount);
	/// 0x08cab284
	void ResetTypes();

	ModType m_kMapModData; ///< +0x0
	ModType m_kPlotModData; ///< +0x224

	// Yep, this returns by value.  Don't worry editing it still
	// edits the real mod data.  The ModDataEntry is really just
	// a reference to a ModType and a memory location to store fields.
	/// 0x08cab302
	ModDataEntry GetMapDataEntry();
	/// 0x08cab318
	ModDataEntry GetPlotDataEntry(uint uiPlot);

	/// 0x08cab348
	size_t GetSerializedSize() const;
	/// 0x08cab392
	void Serialize(void *pv) const;
	/// 0x08cab466
	void Deserialize(const void *pv);

private:
	FFastVector<dword, true> m_kData; ///< +0x448
	ModDataStringBuffer m_kStringBuffer; ///< +0x454
};

#endif // CvWorldBuilderMapModData_h

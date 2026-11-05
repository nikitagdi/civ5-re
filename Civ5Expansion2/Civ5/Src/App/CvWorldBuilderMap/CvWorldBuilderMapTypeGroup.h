/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   CIV 5 World Builder Map   ********************
//
//  FILE:    CvWorldBuilderMapTypeGroup.h
//
//  AUTHOR:  Eric Jordan  --  5/24/2010
//
//  PURPOSE:
//		This little monstrosity was created to support the Civ 5 World Builder map format.
//		It's designed to store a group of related type strings.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2010 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#pragma once

#include "FirePlace/FireWorks/FFireTypes.h"
#ifndef CvWorldBuilderMapTypeGroup_h
#define CvWorldBuilderMapTypeGroup_h

/// size 0x8
class CvWorldBuilderMapTypeGroup
{
public:
	/// 0x08cac8c0
	CvWorldBuilderMapTypeGroup();
	/// 0x08cac8d2
	CvWorldBuilderMapTypeGroup(const CvWorldBuilderMapTypeGroup &kRhs);
	/// 0x08cac91e
	~CvWorldBuilderMapTypeGroup();

	/// 0x08cac986
	const CvWorldBuilderMapTypeGroup &operator=(const CvWorldBuilderMapTypeGroup &kRhs);

	/// 0x08cac9ee
	bool Contains(_In_z_ const char *szType) const;

	/// 0x08caca5e
	void Add(_In_z_ const char *szType);
	/// 0x08cacb32
	void Remove(_In_z_ const char *szType);
	/// 0x08cac952
	void Clear();

	/// inline
	uint GetSerializedSize() const { return m_uiBufferSize; }
	/// 0x08cacc56
	void Serialize(void *p) const;
	/// 0x08cacc8c
	void Deserialize(const void *p, uint uiSize);

private:
	char *m_szBuffer; ///< +0x0
	uint m_uiBufferSize; ///< +0x4
};

#endif // CvWorldBuilderMapTypeGroup_h

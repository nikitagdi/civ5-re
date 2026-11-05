/// origin binary
//------------------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//! \file		FBitFlags.h
//! \author		Bart Muzzin -- 5-2-2006
//! \brief		Going to finally make a bitflags macro!
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2006 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#pragma once
#ifndef FBITFLAGS_H
#define FBITFLAGS_H

#define FBITFLAGS(FlagType) \
	public:\
		bool GetBit(FlagType uiFlag) const { return (m_uiFlags & uiFlag) != 0; } \
		void SetBit(FlagType uiFlag, bool bOn) {  if (bOn) m_uiFlags |= uiFlag; else  m_uiFlags &= ~uiFlag; } \
		void ToggleBit( FlagType uiFlag ) {  if( !(m_uiFlags & uiFlag) ) m_uiFlags |= uiFlag; else  m_uiFlags &= ~uiFlag; } \
		void ClearFlags() { m_uiFlags = 0; } \
		FlagType GetFlags() const { return m_uiFlags; } \
		void SetFlags(FlagType val) { m_uiFlags = val; } \
	private: \
		FlagType m_uiFlags;

template <typename T>
class FTBitFlag;

/// size 0x1
template <>
class FTBitFlag<unsigned char>
{
public:
	/// inline
	bool GetBit(unsigned char uiFlag) const { return (m_uiFlags & uiFlag) != 0; }
	/// inline
	void SetBit(unsigned char uiFlag, bool bOn) {  if (bOn) m_uiFlags |= uiFlag; else  m_uiFlags &= ~uiFlag; }
	/// inline
	void ToggleBit( unsigned char uiFlag ) {  if( !(m_uiFlags & uiFlag) ) m_uiFlags |= uiFlag; else  m_uiFlags &= ~uiFlag; }
	/// inline
	void ClearFlags() { m_uiFlags = 0; }
	/// inline
	unsigned char GetFlags() const { return m_uiFlags; }
	/// inline
	void SetFlags(unsigned char val) { m_uiFlags = val; }
private:
	unsigned char m_uiFlags; ///< +0x0
public:
	/// inline
	FTBitFlag() : m_uiFlags(0) { }
	/// inline
	FTBitFlag(unsigned char uiInit) : m_uiFlags(uiInit) {}
};

/// size 0x2
template <>
class FTBitFlag<unsigned short>
{
public:
	/// inline
	bool GetBit(unsigned short uiFlag) const { return (m_uiFlags & uiFlag) != 0; }
	/// inline
	void SetBit(unsigned short uiFlag, bool bOn) {  if (bOn) m_uiFlags |= uiFlag; else  m_uiFlags &= ~uiFlag; }
	/// inline
	void ToggleBit( unsigned short uiFlag ) {  if( !(m_uiFlags & uiFlag) ) m_uiFlags |= uiFlag; else  m_uiFlags &= ~uiFlag; }
	/// inline
	void ClearFlags() { m_uiFlags = 0; }
	/// inline
	unsigned short GetFlags() const { return m_uiFlags; }
	/// inline
	void SetFlags(unsigned short val) { m_uiFlags = val; }
private:
	unsigned short m_uiFlags; ///< +0x0
public:
	/// inline
	FTBitFlag() : m_uiFlags(0) { }
	/// inline
	FTBitFlag(unsigned short uiInit) : m_uiFlags(uiInit) {}
};

/// size 0x4
template <>
class FTBitFlag<unsigned int>
{
public:
	/// inline
	bool GetBit(unsigned int uiFlag) const { return (m_uiFlags & uiFlag) != 0; }
	/// inline
	void SetBit(unsigned int uiFlag, bool bOn) {  if (bOn) m_uiFlags |= uiFlag; else  m_uiFlags &= ~uiFlag; }
	/// inline
	void ToggleBit( unsigned int uiFlag ) {  if( !(m_uiFlags & uiFlag) ) m_uiFlags |= uiFlag; else  m_uiFlags &= ~uiFlag; }
	/// inline
	void ClearFlags() { m_uiFlags = 0; }
	/// inline
	unsigned int GetFlags() const { return m_uiFlags; }
	/// inline
	void SetFlags(unsigned int val) { m_uiFlags = val; }
private:
	unsigned int m_uiFlags; ///< +0x0
public:
	/// inline
	FTBitFlag() : m_uiFlags(0) { }
	/// inline
	FTBitFlag(unsigned int uiInit) : m_uiFlags(uiInit) {}
};

typedef FTBitFlag<unsigned int> FBitFlag;

#endif // FBITFLAGS_H

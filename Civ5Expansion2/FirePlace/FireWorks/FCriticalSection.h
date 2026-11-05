/// origin binary
//------------------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FCriticalSection.h
//
//  AUTHOR:		Mike Breitkreutz	--  05/03/2005
//
//  PURPOSE:	Critical Section object wrapper to bury platform-specific code.
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2007 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef		FCRITICALSECTION_H
#define		FCRITICALSECTION_H
#pragma		once

#include <Windows.h>

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  CLASS:		FCriticalSection
//
//  PURPOSE:	Critical section object wrapper
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size ?
class FCriticalSection
{
public:
	// Constructors:
	//-------------
	/// 0x08dc23e4
	FCriticalSection( void );

	// Destructors:
	//------------
	/// 0x08dc23f8
	~FCriticalSection( void );

	// Methods:
	//--------
	/// 0x08dc2432
	bool Try( void );

	/// 0x08dc245c
	void Enter( void );
	/// 0x08dc2470
	void Leave( void );

	/// 0x08dc2422
	int GetLockCount() const;
	/// 0x08dc242a
	int GetRecursionCount() const;

private:
	// Members:
	//--------
#ifdef		WIN32
		// The critical section object
		CRITICAL_SECTION m_kCriticalSection; ///< +0x0
#else	//	WIN32
#ifdef		_PS3
		sys_lwmutex_t m_kCriticalSection;		// The critical section object
#endif	//	_PS3
#endif	//	WIN32

};

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  CLASS:		FScopedCriticalSection
//
//  PURPOSE:	Critical section object wrapper
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// size ?
class FScopedCriticalSection
{
public:
	// Constructors:
	//-------------
	/// inline
	FScopedCriticalSection( FCriticalSection& kCriticalSection );

	// Destructors:
	//------------
	/// inline
	~FScopedCriticalSection( void );


private:
	// Members:
	//--------
	// A reference to the critical section object
	FCriticalSection& m_kCriticalSection; ///< +0x0
};

#include	"FirePlace/FireWorks/FCriticalSection.inl"

#endif	//	FCRITICALSECTION_H

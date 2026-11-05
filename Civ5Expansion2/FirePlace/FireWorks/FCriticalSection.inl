/// origin binary
//---------------------------------------------------------------------------------------
//  Copyright (c) 2007 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

//
// Constructors

#pragma once
#include "FirePlace/FireWorks/FCriticalSection.h"
/// inline
inline FScopedCriticalSection::FScopedCriticalSection( FCriticalSection& kCriticalSection ): m_kCriticalSection( kCriticalSection )
{
	m_kCriticalSection.Enter();
}

//
// Destructors

/// inline
inline FScopedCriticalSection::~FScopedCriticalSection( void )
{
	m_kCriticalSection.Leave();
}


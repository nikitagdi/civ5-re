/// origin guess
//------------------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:    FObjectHandle.h
//
//  AUTHOR:  Justin Randall  --  4/13/2007
//
//  PURPOSE: Provides pointer-like access to objects in ways that can be easily checked for object
//           validity
//
//------------------------------------------------------------------------------------------------
//  Copyright (c) 2007 Firaxis Games, Inc. All rights reserved.
//------------------------------------------------------------------------------------------------

#ifndef _INCLUDED_FObjectHandle_H
#define _INCLUDED_FObjectHandle_H

#include <set>
#include "FirePlace/FireWorks/FAssert.h"

// JAR - uncomment this if a stale handle assert is thrown and you don't know
// where the object handle was allocated
// #define TRACK_HANDLE_STACKS 1
//#define TRACK_DESTRUCTION_NOTIFICATION 1

#if defined(TRACK_HANDLE_STACKS)
#	define OBJECT_HANDLE_STACK recordCallstack()
#	ifdef _WINPC
#		include "FirePlace/FireWorks/Win32/FDebugHelper.h"
#	endif//_WINPC
#else
#	define OBJECT_HANDLE_STACK
#endif

/// size ?
template<typename NotificationTarget>
struct DestructionNotification
{
	/// inline
	~DestructionNotification()
	{
		typename std::set<const NotificationTarget *>::const_iterator i;
		for(i = m_targets.begin(); i != m_targets.end(); ++i)
		{
			const NotificationTarget * t = *i;
			t->removeTarget();
		}
	}

	/// inline
	void addTarget(const NotificationTarget & target)
	{
#ifdef TRACK_DESTRUCTION_NOTIFICATION
		m_targets.insert(&target);
#endif
	}

	/// inline
	void removeTarget(const NotificationTarget & target)
	{
#ifdef TRACK_DESTRUCTION_NOTIFICATION
		m_targets.erase(&target);
#endif
	}

	std::set<const NotificationTarget *> m_targets; ///< +0x0
};

/// size ?
template<class PointingTo>
class FObjectHandle
{
public:

	/// inline
	PointingTo * pointer()
	{
		return m_target;
	}

	/// inline
	PointingTo * pointer() const
	{
		return m_target;
	}

	/// inline
	PointingTo * operator -> ()
	{
		return m_target;
	}

	/// inline
	const PointingTo * operator -> () const
	{
		return m_target;
	}

	/// inline
	PointingTo & operator * ()
	{
		return *m_target;
	}

	/// inline
	const PointingTo & operator * () const
	{
		return *m_target;
	}

	/// inline
	operator bool() const
	{
		return m_target != 0;
	}

	/// inline
	bool operator ! () const
	{
		return m_target == 0;
	}

	/// inline
	bool operator != (const PointingTo * rhs) const
	{
		return m_target != rhs;
	}

	/// inline
	bool operator == (const PointingTo * rhs) const
	{
		return m_target == rhs;
	}

	/// inline
	void ignoreDestruction(bool ignore)
	{
		m_ignoreDestruction = ignore;
	}

	/// inline
	void removeTarget() const
	{
#if defined(TRACK_HANDLE_STACKS)
		std::string ownerCallstackString = m_ownerCallstack.toString(false);
		std::string msg = "An object is being destroyed while a handle for it still exists. The handle is no longer valid. If this is acceptable, invoke handle.ignoreDestruction() before use and this assert will not trigger.\n";
		msg += "Owner Callstack : \n";
		msg += ownerCallstackString;
		FAssertMsg(m_ignoreDestruction, msg.c_str());
#else
		FAssertMsg(m_ignoreDestruction, "An object is being destroyed while a handle for it still exists. The handle is no longer valid. If this is acceptable, invoke handle.ignoreDestruction() before use and this assert will not trigger.");
#endif//TRACK_OBJECT_STACKS
		const_cast<FObjectHandle *>(this)->m_target = 0;
	}

	/// inline
	~FObjectHandle()
	{
		if(m_target) 
		{
			m_target->getDestructionNotification().removeTarget(*this);
		}
	}

	/// inline
	FObjectHandle(PointingTo * target) :
	m_target(target)
	, m_ignoreDestruction(false)
	{
		OBJECT_HANDLE_STACK;
		if(m_target)
		{
			m_target->getDestructionNotification().addTarget(*this);
		}
	}

	/// inline
	FObjectHandle & operator=(PointingTo * target)
	{
		OBJECT_HANDLE_STACK;
		if(m_target)
		{
			m_target->getDestructionNotification().removeTarget(*this);
		}
		m_target = target;
		if(m_target)
		{
			m_target->getDestructionNotification().addTarget(*this);
		}
		return *this;
	}

	/// inline
	const FObjectHandle & operator=(const PointingTo * target) const
	{
		OBJECT_HANDLE_STACK;
		if(m_target)
		{
			const_cast<PointingTo *>(m_target)->getDestructionNotification().removeTarget(*this);
		}
		const_cast<FObjectHandle *>(this)->m_target = const_cast<PointingTo *>(target);
		if(m_target)
		{
			const_cast<PointingTo *>(m_target)->getDestructionNotification().addTarget(*this);
		}
		return *this;
	}

	/// inline
	const FObjectHandle & operator=(const FObjectHandle & rhs) const
	{
		if(&rhs != this)
		{
			OBJECT_HANDLE_STACK;
			if(m_target)
			{
				const_cast<FObjectHandle *>(this)->m_target->getDestructionNotification().removeTarget(*this);
			}
			const_cast<FObjectHandle *>(this)->m_target = rhs.m_target;
			if(m_target)
			{
				const_cast<FObjectHandle *>(this)->m_target->getDestructionNotification().addTarget(*this);
			}
		}
		return *this;
	}

	/// inline
	FObjectHandle() :
	m_target(0)
	, m_ignoreDestruction(false)
	{
		OBJECT_HANDLE_STACK;
	}

	/// inline
	FObjectHandle(const FObjectHandle & source) :
	m_target(source.m_target)
	, m_ignoreDestruction(source.m_ignoreDestruction)
	{
		OBJECT_HANDLE_STACK;
		if(m_target)
		{
			m_target->getDestructionNotification().addTarget(*this);
		}
	}
#if defined(TRACK_HANDLE_STACKS)

private:
	void recordCallstack() const
	{
		FDebugHelper & debugHelper = FDebugHelper::GetInstance();
		debugHelper.GetCallStack(&m_ownerCallstack, 2);
	}

#endif//TRACK_HANDLE_STACKS
private:
	PointingTo *  m_target; ///< +0x0
	bool          m_ignoreDestruction; ///< +0x4
#if defined(TRACK_HANDLE_STACKS)
	mutable FCallStack   m_ownerCallstack;
#endif//TRACK_HANDLE_STACKS
};

/// inline
template<class PointingTo>
const FObjectHandle<PointingTo> ConstHandle(const PointingTo * target) 
{
	FObjectHandle<PointingTo> result;
	result = target;
	return result;
}

#endif//_INCLUDED_FObjectHandle_H

/// origin guess
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FAutoArchiveClassContainer.h
//
//  AUTHOR:		Justin Randall	--  07/20/2009
//
//  PURPOSE:	Specializes the AutoArchive to track class instances for use with 
//              FAutoVariable<> members. Useful for debugging and implementing member
//              function callbacks.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------
#ifndef _INCLUDED_FAutoArchiveClassContainer_H
#define _INCLUDED_FAutoArchiveClassContainer_H

#include <map>
#include <string>
#include <algorithm>

//---------------------------------------------------------------------------------------

#include "FirePlace/FireWorks/FAutoArchive.h"

//---------------------------------------------------------------------------------------

// Because some FAutoVariable's need to know about their
// owner container AND the AutoArchive they belong to
/// size ?
/// vtable ?
template<class ClassType>
class FAutoArchiveClassContainer : public FAutoArchive
{
public:
	/// inline
	FAutoArchiveClassContainer(ClassType & owner) :
	FAutoArchive()
	, m_classInstance(owner)
	{
	}

	/// inline
	ClassType & owner()
	{
		return m_classInstance;
	}

	/// inline
	const ClassType & owner() const
	{
		return m_classInstance;
	}

	/// inline
	virtual const std::string * getVariableName(const FAutoVariableBase & var) const
	{
		const std::string * result = 0;

		size_t key = std::distance(m_contents.begin(), std::find(m_contents.begin(), m_contents.end(), &var));
		std::map<size_t, std::string> & names = getNames();
		std::map<size_t, std::string>::const_iterator f = names.find(key);
		
		if(f != names.end())
		{
			result = &f->second;
		}
		return result;
	}

	/// inline
	virtual void setVariableName(const FAutoVariableBase & var, const std::string & name) const
	{
		std::map<size_t, std::string> & names = getNames();

		size_t offset = std::distance(m_contents.begin(), std::find(m_contents.begin(), m_contents.end(), &var));
		std::map<size_t, std::string>::const_iterator f = names.find(offset);
		if(f == names.end())
		{
			names[offset] = name;
		}
	}

	/// inline
	virtual std::string debugDump(const FAutoVariableBase & var) const
	{
		return std::string("\n") + m_classInstance.debugDump(var);
	}

	/// inline
	virtual std::string stackTraceRemark(const FAutoVariableBase & var) const
	{
		return m_classInstance.stackTraceRemark(var);
	}

protected:
	/// inline
	std::map<size_t, std::string> & getNames() const;

private:
	ClassType & m_classInstance; ///< +0x1c
};

//---------------------------------------------------------------------------------------

// rather than storing a duplicate string for each autovariable, wasting memory
// let's put these in a map. Names are used mostly for slow serialization and
// debugging.
/// inline
template<class ClassType>
std::map<size_t, std::string> & FAutoArchiveClassContainer<ClassType>::getNames() const
{
	static std::map<size_t, std::string> names;
	return names;
}

//---------------------------------------------------------------------------------------

/// inline
template<class ClassType>
FDataStream & operator<<(FDataStream & saveTo, const FAutoArchiveClassContainer<ClassType> & readFrom)
{
	readFrom.save(saveTo);
	return saveTo;
}

//---------------------------------------------------------------------------------------

/// inline
template<class ClassType>
FDataStream & operator>>(FDataStream & loadFrom, FAutoArchiveClassContainer<ClassType> & writeTo)
{
	writeTo.load(loadFrom);
	return loadFrom;
}

//---------------------------------------------------------------------------------------

#endif//_INCLUDED_FAutoArchiveClassContainer_H

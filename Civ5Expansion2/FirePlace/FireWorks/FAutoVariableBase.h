/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FAutoVariableBase.cpp
//
//  AUTHOR:		Justin Randall	--  07/20/2009
//
//  PURPOSE:	Base class declaration for FAutoVariables. See FAutoVariable.h
//              for a concrete description.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------
#ifndef _INCLUDED_FAutoVariableBase_H
#define _INCLUDED_FAutoVariableBase_H

#ifndef FINAL_RELEASE
#	include "FirePlace/FireWorks/FCallStack.h"
#endif//FINAL_RELEASE

#ifdef _WINPC
#	pragma warning ( disable : 4355 )  // Clients of this class will need to pass 'this' along to construct AutoVariables
#endif//_WINPC
#include <vector>

class FAutoArchive;
class FDataStream;
class FAutoVariableBase;

namespace FSerialization
{
	// converting types to printable strings, for debugging
	/// 0x08dc4429
	std::string toString(const bool & source);
	/// 0x08dc4165
	std::string toString(const unsigned char & source);
	/// 0x08dc41bd
	std::string toString(const char & source);
	/// 0x08dc4215
	std::string toString(const unsigned short & source);
	/// 0x08dc426d
	std::string toString(const short & source);
	/// 0x08dc42c5
	std::string toString(const unsigned int & source);
	/// 0x08dc431c
	std::string toString(const int & source);
	/// 0x08dc4373
	std::string toString(const float & source);
	/// 0x08dc43ce
	std::string toString(const double & source);
	/// 0x08dc455d
	std::string toString(const std::string & source); // for completeness, no a-priori knowlege of what the var type is
	/// 0x08dc45a7
	std::string toString(const FAutoVariableBase &);
	/// inline
	template<typename ValueType>
	std::string toString(const std::vector<ValueType> & source)
	{
		std::string result = "{";
		typename std::vector<ValueType>::const_iterator i;
		for(i = source.begin(); i != source.end(); ++i)
		{
			if(i != source.begin())
				result += std::string(",");
			result += toString(*i);
		}
		return result;
	}

	// temporary fall-through do-nothing case
	// comment this to let the compiler tell you which
	// types need implementations or which implementations
	// are not in scope for code that fails after removing
	// this function
	/// inline
	template<typename ValueType>
	std::string toString(const ValueType &)
	{
		static const std::string result("UNKNOWN");
		return result;
	}
}

/// size 0x4
/// vtable 0x0a306750
class FAutoVariableBase
{
public:
	/// 0x08dc3f6c
	FAutoVariableBase(const std::string & name, FAutoArchive & owner);

	// used for extended debugging out of sync errors. Does nothing in release builds
	/// 0x08dc3fbc
	FAutoVariableBase(const std::string & name, FAutoArchive & owner, bool callStackTracking);

	/// 0x08dc4020
	/// deleting 0x08dc400c
	virtual ~FAutoVariableBase() = 0;
	/// pure
	virtual void load(FDataStream & loadFrom) = 0;
	/// pure
	virtual void loadDelta(FDataStream & loadFrom) = 0;
	/// pure
	virtual void save(FDataStream & saveTo) const = 0;
	/// pure
	virtual void saveDelta(FDataStream & saveTo) const = 0;
	/// pure
	virtual void clearDelta() = 0;
	/// pure
	virtual bool compare(FDataStream & otherValue) const = 0;
	/// pure
	virtual void reset() = 0;

	/// pure
	virtual const std::string & name() const = 0;

	// used for extended debugging out of sync errors. Does nothing in release builds
	/// 0x08dc4022
	std::string  getStackTrace() const;
	/// 0x08dc4058
	std::string  getStackTraceRemark() const;
	/// pure
	virtual void setStackTraceRemark() = 0;
	/// 0x08dc4126
	virtual std::string  debugDump(const std::vector<std::pair<std::string, std::string> > &) const;
	/// pure
	virtual std::string toString() const = 0;

protected:
#ifndef FINAL_RELEASE
	friend class FAutoArchive;
	FCallStack   m_lastCallStackToChangeThisVariable;
	std::string  m_callStackRemark;
	bool         m_callStackTracking;
#endif
private:
	// keep these out of containers by value, they won't do what is expected
	/// absent
	FAutoVariableBase(const FAutoVariableBase &);
	/// absent
	FAutoVariableBase & operator=(const FAutoVariableBase &);

private:
	//std::string  m_name; // JAR : todo  - this is redundant for each class instance, this could be a FlyWeight
};

#endif//_INCLUDED_FAutoVariableBase_H

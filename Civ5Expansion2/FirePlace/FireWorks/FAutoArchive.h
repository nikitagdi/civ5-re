/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FAutoArchive.h
//
//  AUTHOR:		Justin Randall	--  07/20/2009
//
//  PURPOSE:	Contains FAutoVariableBase types which intercept read/writes, track 
//              deltas, compare differences. It is currently used for OOS checks, but
//              could also be used for general, automatic serialization.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#ifndef _INCLUDED_FAutoArchive_H
#define _INCLUDED_FAutoArchive_H

//---------------------------------------------------------------------------------------

#include <set>
#include <vector>

//---------------------------------------------------------------------------------------

class FDataStream;
class FAutoVariableBase;

//---------------------------------------------------------------------------------------

#if !defined(FINAL_RELEASE)
#define FAUTOARCHIVE_DEBUG
#endif

/// size 0x1c
/// vtable 0x0a306720
class FAutoArchive
{
public:
	/// 0x08dc3564
	FAutoArchive();
	/// 0x08dc35f4
	/// deleting 0x08dc35aa
	virtual ~FAutoArchive() = 0;
	/// 0x08dc3638
	void add(FAutoVariableBase &);
	/// 0x08dc368a
	void remove(FAutoVariableBase &);

	/// 0x08dc3758
	void load(FDataStream & loadFrom);
	/// 0x08dc3716
	void save(FDataStream & saveTo) const;

	/// 0x08dc379a
	void saveDelta(FDataStream & saveTo, std::vector<std::pair<std::string, std::string> > & callStacks) const; // verbose version
	/// 0x08dc3928
	void loadDelta(FDataStream & loadFrom);
	/// 0x08dc387c
	void clearDelta();

	/// 0x08dc38f4
	void reset();

	/// 0x08dc3a6a
	bool hasDeltas() const;
	/// inline
	__forceinline void touch(FAutoVariableBase & dirtyVariable)
	{
#ifdef FAUTOARCHIVE_DEBUG
		debugHelp(dirtyVariable);
#endif//FAUTOARCHIVE_DEBUG
		m_deltas.insert(&dirtyVariable);
	}


	/// pure
	virtual const std::string * getVariableName(const FAutoVariableBase &) const = 0;
	/// pure
	virtual void setVariableName(const FAutoVariableBase &, const std::string &) const = 0;
	/// pure
	virtual std::string debugDump(const FAutoVariableBase &) const = 0;
	/// pure
	virtual std::string stackTraceRemark(const FAutoVariableBase &) const = 0;
	// JAR : todo  - move this into a SyncArchive type
	/// 0x08dc3996
	std::vector<const FAutoVariableBase *> findMismatchedVariables(FDataStream &) const;
	/// 0x08dc3aa6
	const FAutoVariableBase * findVariable(const std::string & name) const;

	// Debugging support
	/// 0x08dc3a7a
	static	void EnableCallStacks(bool bState);
	/// 0x08dc3a92
	static	bool AreCallStacksEnabled();
protected:
#ifdef FAUTOARCHIVE_DEBUG
	void debugHelp(FAutoVariableBase & dirtyVariable);
#endif
	std::vector<FAutoVariableBase *>       m_contents; ///< +0x4
	/// 0x0a312f38
	static bool ms_bCallStacksEnabled;
private:
	// JAR  - book keeping container, used to track changes
	mutable std::set<FAutoVariableBase *>  m_deltas; ///< +0x10
};

//---------------------------------------------------------------------------------------

/// 0x08dc3b71
FDataStream & operator<<(FDataStream & saveTo, const FAutoArchive & readFrom);
/// 0x08dc3bb2
FDataStream & operator>>(FDataStream & loadFrom, FAutoArchive & writeTo);

//---------------------------------------------------------------------------------------

#endif//_INCLUDED_FAutoArchive_H

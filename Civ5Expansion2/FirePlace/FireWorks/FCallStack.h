/// origin binary
#ifndef _INCLUDED_FCallStack_H
#define _INCLUDED_FCallStack_H

#include "FirePlace/FireWorks/FSTLBlockAllocator.h"
#include <map>
#include <vector>

#ifdef _WINPC
typedef unsigned long long FCallStackAddressEntry;
typedef unsigned long long FCallStackHashEntry;
#else
typedef UINT64 FCallStackAddressEntry;
typedef UINT64 FCallStackHashEntry;
#endif

// A call stack, which is just a simple array of addresses
/// size 0x10
class FCallStack : public std::vector<FCallStackAddressEntry, FSTLBlockAllocator<FCallStackAddressEntry, 1024> >
{
public:
	unsigned int m_uiReferences; ///< +0xc
	/// inline
	FCallStack() : m_uiReferences(0) {};
	/// 0x08dcc7e0
	std::string toString(bool stripPaths, int iCSVIndent = 0) const;
};

// A map of call stacks, keyed to a hash value for quick location
typedef std::pair <const FCallStackHashEntry, FCallStack*> FCallStackMapEntry;
typedef std::map<FCallStackHashEntry, FCallStack*, std::less<const FCallStackHashEntry>, FSTLBlockAllocator<FCallStackMapEntry, 1024> > FCallStackMap;

#endif//_INCLUDED_FCallStack_H

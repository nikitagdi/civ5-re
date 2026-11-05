/// origin binary
//---------------------------------------------------------------------------------------
//
//  *****************   FIRAXIS GAME ENGINE   ********************
//
//  FILE:		FStlContainerSerialization.h
//
//  AUTHOR:		Justin Randall
//
//  PURPOSE:	Support STL containers with any type serializable with FDataStream.
//
//---------------------------------------------------------------------------------------
//  Copyright (c) 2009 Firaxis Games, Inc. All rights reserved.
//---------------------------------------------------------------------------------------

#ifndef _INCLUDED_FStlContainerSerialization_H
#define _INCLUDED_FStlContainerSerialization_H

#include <unordered_map>
#if defined(_MSC_VER)
#include <hash_map>
#endif

#include "FirePlace/FireWorks/FDataStream.h"

//---------------------------------------------------------------------
// Specific container specializations help the compiler associate
// the right code for the type serializing with an FDataStream. 
//
// Many containers are represented identically in an FDataStream, so
// use more generic methods for moving data to/from the FDataStream.
// For example, lists, vectors and deques are all sequence containers
// and so all use the same load/save code, but specify explicit 
// specializations to match the template with the container calling
// the shift operator. 
//
// maps and sets are associative containers.
// Though hash_maps aren't implemented in this file, they can use the 
// same associative container code here and simply provide the 
// specialized template wrapper.
//---------------------------------------------------------------------

#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <unordered_set>


/// inline
template<typename FirstType, typename SecondType>
FDataStream & operator<<(FDataStream & saveTo, const std::pair<FirstType, SecondType> & readFrom)
{
	saveTo << readFrom.first;
	saveTo << readFrom.second;
	return saveTo;
}

/// inline
template<typename FirstType, typename SecondType>
FDataStream & operator>>(FDataStream & loadFrom, std::pair<FirstType, SecondType> & writeTo)
{
	loadFrom >> writeTo.first;
	loadFrom >> writeTo.second;
	return loadFrom;
}

// STL sequence containers are all serialized the same way.
// deques, vectors and lists are identical in a stream, so
// the Serialize[To/From]SequenceContainer functors
// handle the specifics of serialization.
/// size ?
template<typename ElementType, typename ContainerType>
struct SerializeFromSequenceContainer
{
	/// inline
	SerializeFromSequenceContainer(FDataStream & saveTo, const ContainerType & container) :
	m_saveTo(saveTo)
	, m_container(container)
	{
		m_saveTo << container.size();
	}

	/// inline
	void operator() (ElementType & i)
	{
		m_saveTo << i;
	}

	FDataStream & m_saveTo; ///< +0x0
	const ContainerType & m_container; ///< +0x4
};
/// inline
template<typename ElementType, typename ContainerType>
void SerializeToSequenceContainer(FDataStream & loadFrom, ContainerType & container)
{
	container.clear();
	typename ContainerType::size_type count = 0;
	loadFrom >> count;
	typename ContainerType::size_type i = 0;
	for(i = 0; i < count; ++i)
	{
		ElementType v;
		loadFrom >> v;
		container.push_back(v);
	}
}

/*
template<typename ElementType, typename ContainerType>
struct SerializeToSequenceContainer
{
	SerializeToSequenceContainer(FDataStream & loadFrom, ContainerType & container) :
	m_loadFrom(loadFrom)
	, m_container(container)
	{
		container.clear();
		size_t count = 0;
		m_loadFrom >> count;
		container.resize(count);
	}

	void operator() (ElementType & i)
	{
		m_loadFrom >> i;
	}

	FDataStream & m_loadFrom;
	ContainerType & m_container;
};
*/
/// size ?
template<typename ElementType, typename ContainerType>
struct SerializeFromAssociativeContainer
{
	/// inline
	SerializeFromAssociativeContainer(FDataStream & saveTo, ContainerType & container) :
	m_saveTo(saveTo)
	, m_container(container)
	{
		size_t count = container.size();
		m_saveTo << count;
	}

	/// inline
	void operator()(const ElementType & i)
	{
		m_saveTo << i;
	}

	FDataStream & m_saveTo; ///< +0x0
	ContainerType & m_container; ///< +0x4
};

/// inline
template<typename ElementType, typename ContainerType>
void SerializeToAssociativeContainer(FDataStream & loadFrom, ContainerType & container)
{
	container.clear();
	size_t count = 0;
	loadFrom >> count;
	size_t i = 0;
	for(i = 0; i < count; ++i)
	{
		ElementType e;
		loadFrom >> e;
		container.insert(e);
	}
}

/// inline
template<typename KeyType, typename ElementType>
FDataStream & operator<<(FDataStream & saveTo, const std::map<KeyType, ElementType> & readFrom)
{
	std::for_each(readFrom.begin(), readFrom.end(), SerializeFromAssociativeContainer<std::pair<KeyType, ElementType>, const std::map<KeyType, ElementType> >(saveTo, readFrom));
	return saveTo;
}

/// inline
template<typename KeyType, typename ElementType>
FDataStream & operator<<(FDataStream & saveTo, const std::unordered_map<KeyType, ElementType> & readFrom)
{
	std::for_each(readFrom.begin(), readFrom.end(), SerializeFromAssociativeContainer<std::pair<KeyType, ElementType>, const std::unordered_map<KeyType, ElementType> >(saveTo, readFrom));
	return saveTo;
}

#if defined(_MSC_VER)
template<typename KeyType, typename ElementType>
FDataStream & operator<<(FDataStream & saveTo, const stdext::hash_map<KeyType, ElementType> & readFrom)
{
	std::for_each(readFrom.begin(), readFrom.end(), SerializeFromAssociativeContainer<std::pair<KeyType, ElementType>, const stdext::hash_map<KeyType, ElementType> >(saveTo, readFrom));
	return saveTo;
}
#endif

/// inline
template<typename KeyType>
FDataStream & operator <<(FDataStream & saveTo, const std::unordered_set<KeyType>& readFrom)
{
	std::for_each(readFrom.begin(), readFrom.end(), SerializeFromAssociativeContainer<KeyType, const std::unordered_set<KeyType> >(saveTo, readFrom));
	return saveTo;
}

/// inline
template<typename ElementType>
FDataStream & operator<<(FDataStream & saveTo, const std::set<ElementType> & readFrom)
{
	std::for_each(readFrom.begin(), readFrom.end(), SerializeFromAssociativeContainer<ElementType, const std::set<ElementType> >(saveTo, readFrom));
	return saveTo;
}

/// inline
template<typename ElementType> 
FDataStream & operator<<(FDataStream & saveTo, const std::deque<ElementType> & readFrom)
{
	std::for_each(readFrom.begin(), readFrom.end(), SerializeFromSequenceContainer<const ElementType, const std::deque<ElementType> >(saveTo, readFrom));
	return saveTo;
}

/// inline
template<typename ElementType> 
FDataStream & operator<<(FDataStream & saveTo, const std::list<ElementType> & readFrom)
{
	std::for_each(readFrom.begin(), readFrom.end(), SerializeFromSequenceContainer<const ElementType, const std::list<ElementType> >(saveTo, readFrom));
	return saveTo;
}

/// inline
template<typename ElementType> 
FDataStream & operator<<(FDataStream & saveTo, const std::vector<ElementType> & readFrom)
{
	std::for_each(readFrom.begin(), readFrom.end(), SerializeFromSequenceContainer<const ElementType, const std::vector<ElementType> >(saveTo, readFrom));
	return saveTo;
}

/// inline
template<typename ElementType>
FDataStream & operator>>(FDataStream & loadFrom, std::deque<ElementType> & writeTo)
{
	// The functor needs to be instantiated to properly resize the container based 
	// on how many elements the stream says it should have before passing it along
	// to std::for_each
	SerializeToSequenceContainer<ElementType, std::deque<ElementType> >(loadFrom, writeTo);
	return loadFrom;
}

/// inline
template<typename ElementType>
FDataStream & operator>>(FDataStream & loadFrom, std::list<ElementType> & writeTo)
{
	// The functor needs to be instantiated to properly resize the container based 
	// on how many elements the stream says it should have before passing it along
	// to std::for_each
	SerializeToSequenceContainer<ElementType, std::list<ElementType> >(loadFrom, writeTo);
	return loadFrom;
}

/// inline
template<typename ElementType>
FDataStream & operator>>(FDataStream & loadFrom, std::vector<ElementType> & writeTo)
{
	// The functor needs to be instantiated to properly resize the container based 
	// on how many elements the stream says it should have before passing it along
	// to std::for_each
	SerializeToSequenceContainer<ElementType, std::vector<ElementType> >(loadFrom, writeTo);

	//std::for_each(writeTo.begin(), writeTo.end(), func);
	return loadFrom;
}

/// inline
template<typename KeyType, typename ElementType>
FDataStream & operator>>(FDataStream & loadFrom, std::map<KeyType, ElementType> & writeTo)
{
	SerializeToAssociativeContainer<std::pair<KeyType, ElementType>, std::map<KeyType, ElementType> >(loadFrom, writeTo);
	return loadFrom;
}

/// inline
template<typename KeyType, typename ElementType>
FDataStream & operator>>(FDataStream & loadFrom, std::unordered_map<KeyType, ElementType> & writeTo)
{
	SerializeToAssociativeContainer<std::pair<KeyType, ElementType>, std::unordered_map<KeyType, ElementType> >(loadFrom, writeTo);
	return loadFrom;
}

#if defined(_MSC_VER)
template<typename KeyType, typename ElementType>
FDataStream & operator>>(FDataStream & loadFrom, stdext::hash_map<KeyType, ElementType> & writeTo)
{
	SerializeToAssociativeContainer<std::pair<KeyType, ElementType>, stdext::hash_map<KeyType, ElementType> >(loadFrom, writeTo);
	return loadFrom;
}
#endif

/// inline
template<typename KeyType>
FDataStream & operator>>(FDataStream & loadFrom, std::unordered_set<KeyType> & writeTo)
{
	SerializeToAssociativeContainer<KeyType, std::unordered_set<KeyType> >(loadFrom, writeTo);
	return loadFrom;
}

/// inline
template<typename ElementType>
FDataStream & operator>>(FDataStream & loadFrom, std::set<ElementType> & writeTo)
{
	SerializeToAssociativeContainer<ElementType, std::set<ElementType> >(loadFrom, writeTo);
	return loadFrom;
}

/// 0x08dc4b9c
FDataStream & operator<<(FDataStream & saveTo, const std::string & readFrom);
/// 0x08dc4c26
FDataStream & operator>>(FDataStream & loadFrom, std::string & writeTo);

#endif//_INCLUDED_FStlContainerSerialization_H

/// origin binary
#include "FirePlace/FireWorks/FStlContainerSerialization.h"

/// 0x08dc4b9c
FDataStream & operator<<(FDataStream & saveTo, const std::string & readFrom)
{
	UINT32 size = static_cast<UINT32>(readFrom.size());
	saveTo << size;
	for (std::string::const_iterator it = readFrom.begin(); it != readFrom.end(); ++it)
		saveTo << *it;
	return saveTo;
}

/// 0x08dc4c26
FDataStream & operator>>(FDataStream & loadFrom, std::string & writeTo)
{
	UINT32 size = 0;
	loadFrom >> size;
	writeTo.clear();
	for (UINT32 i = 0; i < size; ++i)
	{
		char c = 0;
		loadFrom >> c;
		writeTo.push_back(c);
	}
	return loadFrom;
}

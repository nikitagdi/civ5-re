/// origin binary
#include "FirePlace/FireWorks/FAutoVariableBase.h"

#include <cstdio>
#include <string>

#include "FirePlace/FireWorks/FAutoArchive.h"

namespace FSerialization
{
	/// 0x08dc4429
	std::string toString(const bool & source)
	{
		static const std::string sTrue("true");
		static const std::string sFalse("false");
		return source ? sTrue : sFalse;
	}

	/// 0x08dc41bd
	std::string toString(const char & source)
	{
		char buffer[5];
		std::snprintf(buffer, sizeof(buffer), "%hhd", static_cast<int>(source));
		return std::string(buffer);
	}

	/// 0x08dc42c5
	std::string toString(const unsigned int & source)
	{
		char buffer[11];
		// "%ud" as in the binary: the value followed by a literal 'd'.
		std::sprintf(buffer, "%ud", source);
		return std::string(buffer);
	}

	/// 0x08dc431c
	std::string toString(const int & source)
	{
		char buffer[12];
		std::snprintf(buffer, sizeof(buffer), "%d", source);
		return std::string(buffer);
	}

	/// 0x08dc4373
	std::string toString(const float & source)
	{
		char buffer[32];
		std::snprintf(buffer, sizeof(buffer), "%f", static_cast<double>(source));
		return std::string(buffer);
	}

	/// 0x08dc455d
	std::string toString(const std::string & source)
	{
		return source;
	}

	/// 0x08dc45a7
	std::string toString(const FAutoVariableBase & source)
	{
		return source.toString();
	}
}

/// 0x08dc3f6c
FAutoVariableBase::FAutoVariableBase(const std::string & name, FAutoArchive & owner)
{
	owner.add(*this);
	owner.setVariableName(*this, name);
}

/// 0x08dc3fbc
FAutoVariableBase::FAutoVariableBase(const std::string & name, FAutoArchive & owner, bool callStackTracking)
{
	owner.add(*this);
	owner.setVariableName(*this, name);
}

/// 0x08dc4020
FAutoVariableBase::~FAutoVariableBase()
{
}

/// 0x08dc4022
std::string FAutoVariableBase::getStackTrace() const
{
	return "Stack Trace unavailable";
}

/// 0x08dc4126
std::string FAutoVariableBase::debugDump(const std::vector<std::pair<std::string, std::string> > &) const
{
	return std::string("Variable Out Of Sync : ") + name();
}

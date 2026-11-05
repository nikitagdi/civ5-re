/// origin guess
#include "Civ5/Src/CvLocalization/CvLocalization.h"
#include <cstring>

/// 0x08c5cd04
Localization::String::String()
{
	m_pkLanguage = nullptr;
}

/// 0x08c5cd8c
Localization::String::String(const char* szString)
{
	m_pkLanguage = nullptr;
	size_t length = szString ? strlen(szString) : 0;
	if (length != 0)
	{
		m_BaseString = SharedString(szString, length);
		ParseBaseString();
	}
	else
	{
		m_BaseString = SharedString("", 0);
	}
}

/// 0x08c5d3d2
Localization::String::String(const String& other)
	: m_BaseString(other.m_BaseString),
	  m_ComposedNullTerminated(other.m_ComposedNullTerminated),
	  m_arguments(other.m_arguments),
	  m_Forms(other.m_Forms),
	  m_pkLanguage(other.m_pkLanguage)
{
}

/// 0x08c5d4ae
Localization::String::~String()
{
}

/// 0x08c5d636
bool Localization::String::IsEmpty() const
{
	size_t length = 0;
	m_BaseString.GetString(length);
	return length == 0;
}

/// 0x08c5d506
Localization::String& Localization::String::operator=(const String& other)
{
	m_BaseString = other.m_BaseString;
	m_ComposedNullTerminated = other.m_ComposedNullTerminated;
	if (this != &other)
	{
		m_arguments.assign(other.m_arguments.begin(), other.m_arguments.end());
		m_Forms.assign(other.m_Forms.begin(), other.m_Forms.end());
	}
	m_pkLanguage = other.m_pkLanguage;
	return *this;
}

/// 0x08c5d58a
Localization::String& Localization::String::operator=(const char* szBaseString)
{
	size_t length = strlen(szBaseString);
	if (length != 0)
	{
		m_BaseString = SharedString(szBaseString, length);
	}
	else
	{
		m_BaseString = SharedString();
	}
	ParseBaseString();
	return *this;
}

/// 0x08c5d676
const char* Localization::String::toUTF8()
{
	if (m_ComposedNullTerminated.GetLength() == 0)
	{
		size_t bytes = 0;
		const char* szUTF8 = toUTF8(bytes, 1);
		m_ComposedNullTerminated = SharedString(szUTF8, bytes);
	}
	size_t length = 0;
	return m_ComposedNullTerminated.GetString(length);
}

/// 0x08c63a9f
Localization::String Localization::Lookup(const char* textKey)
{
	return m_pkDictionary->Lookup(textKey);
}

/// 0x08c5e2f2
bool Localization::String::PushArgument(const char* szText)
{
	String text(szText);
	m_arguments.push_back(ParameterArgument(text));
	return true;
}

/// 0x08c5e47e
bool Localization::String::PushArgument(const int val)
{
	m_arguments.push_back(ParameterArgument(val));
	return true;
}

/// 0x08c5e5dc
bool Localization::String::PushArgument(const float val)
{
	m_arguments.push_back(ParameterArgument(val));
	return true;
}

/// 0x08c5e764
bool Localization::String::PushArgument(const String& strText)
{
	m_arguments.push_back(ParameterArgument(strText));
	return true;
}

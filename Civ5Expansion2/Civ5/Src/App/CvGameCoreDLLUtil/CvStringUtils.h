/// origin binary
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//!	 \file		CvstringUtils.h
//!  \brief     Public header common string functions
//!
//!		This header includes utility functions for changing string encoding.
//!
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#pragma once

#include <string>

namespace CvStringUtils
{
	/// 0x08b93ce4
	std::wstring FromACPToUTF16(const std::string& strCPString);
	/// 0x08b93df9
	std::wstring FromUTF8ToUTF16(const std::string& strUTF8String);

	/// 0x08b93f0e
	std::string FromUTF16ToACP(const std::wstring& strUTF16String);
	/// 0x08b9403e
	std::string FromUTF16ToUTF8(const std::wstring& strUTF16String);
}

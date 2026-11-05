/// origin binary
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FString.h"

#include <stdexcept>
#include <string>

// Multi-byte values travel in the stream's endianness; bytes are reversed when the host differs.

/// 0x08d7cb40
void FDataStream::Read(FStringA& szName)
{
	INT32 count;
	Read(count);
	szName.Copy(0, "", 0);
	const int sizeLeft = static_cast<int>(GetSizeLeft());
	if (count > 0 && count <= sizeLeft)
	{
		std::string buffer(count, '\0');
		ReadIt(count, &buffer[0]);
		szName.Copy(count, buffer.c_str());
	}
	else if (count < 0 || count > sizeLeft)
	{
		std::runtime_error error("FDataStream ReadString error");
		RaiseException(error);
	}
}

/// 0x08d7cbe4
void FDataStream::Read(std::string& szName)
{
	INT32 count;
	Read(count);
	szName.clear();
	const int sizeLeft = static_cast<int>(GetSizeLeft());
	if (count > 0 && count <= sizeLeft)
	{
		szName.resize(count);
		ReadIt(count, &szName[0]);
		return;
	}
	if (count >= 0 && count <= sizeLeft)
		return;
	std::runtime_error error("FDataStream ReadString error");
	RaiseException(error);
}

/// 0x08d7caaa
void FDataStream::Read(bool& value)
{
	ReadIt(1, &value);
}

/// 0x08d7cadc
void FDataStream::Read(unsigned char& value)
{
	ReadIt(1, &value);
}

/// 0x08d7cb0e
void FDataStream::Read(char& value)
{
	ReadIt(1, &value);
}

/// 0x08d7c248
void FDataStream::Read(INT32& value)
{
	ReadIt(4, &value);
	if (m_eEndianness != FEndian::GetEndianness())
	{
		unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
		unsigned char tmp = bytes[0]; bytes[0] = bytes[3]; bytes[3] = tmp;
		tmp = bytes[1]; bytes[1] = bytes[2]; bytes[2] = tmp;
	}
}

/// 0x08d7d4ce
void FDataStream::Read(UINT32& value)
{
	ReadIt(4, &value);
	if (m_eEndianness != FEndian::GetEndianness())
	{
		unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
		unsigned char tmp = bytes[0]; bytes[0] = bytes[3]; bytes[3] = tmp;
		tmp = bytes[1]; bytes[1] = bytes[2]; bytes[2] = tmp;
	}
}

/// 0x08d7d626
void FDataStream::Read(INT16& value)
{
	ReadIt(2, &value);
	if (m_eEndianness != FEndian::GetEndianness())
	{
		unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
		unsigned char tmp = bytes[0]; bytes[0] = bytes[1]; bytes[1] = tmp;
	}
}

/// 0x08d7d672
void FDataStream::Read(UINT16& value)
{
	ReadIt(2, &value);
	if (m_eEndianness != FEndian::GetEndianness())
	{
		unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
		unsigned char tmp = bytes[0]; bytes[0] = bytes[1]; bytes[1] = tmp;
	}
}

/// 0x08d7d70a
void FDataStream::Read(float& value)
{
	ReadIt(4, &value);
	if (m_eEndianness != FEndian::GetEndianness())
	{
		unsigned char* bytes = reinterpret_cast<unsigned char*>(&value);
		unsigned char tmp = bytes[0]; bytes[0] = bytes[3]; bytes[3] = tmp;
		tmp = bytes[1]; bytes[1] = bytes[2]; bytes[2] = tmp;
	}
}

/// 0x08d7bc3e
void FDataStream::Write(const FStringA& szName)
{
	INT32 length = szName.GetLength();
	Write(length);
	if (length > 0)
		WriteIt(length, szName.c_str());
}

/// 0x08d7be1a
void FDataStream::Write(const std::string& szName)
{
	INT32 length = static_cast<INT32>(szName.size());
	Write(length);
	if (length > 0)
		WriteIt(length, szName.data());
}

/// 0x08d7bba8
void FDataStream::Write(const bool& value)
{
	WriteIt(1, &value);
}

/// 0x08d7bbda
void FDataStream::Write(const unsigned char& value)
{
	WriteIt(1, &value);
}

/// 0x08d7bc0c
void FDataStream::Write(const char& value)
{
	WriteIt(1, &value);
}

/// 0x08d7c086
void FDataStream::Write(const INT32& value)
{
	if (m_eEndianness == FEndian::GetEndianness())
	{
		WriteIt(4, &value);
		return;
	}
	const unsigned char* src = reinterpret_cast<const unsigned char*>(&value);
	unsigned char swapped[4] = { src[3], src[2], src[1], src[0] };
	WriteIt(4, swapped);
}

/// 0x08d7cd5a
void FDataStream::Write(const UINT32& value)
{
	if (m_eEndianness == FEndian::GetEndianness())
	{
		WriteIt(4, &value);
		return;
	}
	const unsigned char* src = reinterpret_cast<const unsigned char*>(&value);
	unsigned char swapped[4] = { src[3], src[2], src[1], src[0] };
	WriteIt(4, swapped);
}

/// 0x08d7cf90
void FDataStream::Write(const INT16& value)
{
	if (m_eEndianness == FEndian::GetEndianness())
	{
		WriteIt(2, &value);
		return;
	}
	const unsigned char* src = reinterpret_cast<const unsigned char*>(&value);
	unsigned char swapped[2] = { src[1], src[0] };
	WriteIt(2, swapped);
}

/// 0x08d7d012
void FDataStream::Write(const UINT16& value)
{
	if (m_eEndianness == FEndian::GetEndianness())
	{
		WriteIt(2, &value);
		return;
	}
	const unsigned char* src = reinterpret_cast<const unsigned char*>(&value);
	unsigned char swapped[2] = { src[1], src[0] };
	WriteIt(2, swapped);
}

/// 0x08d7d114
void FDataStream::Write(const float& value)
{
	if (m_eEndianness == FEndian::GetEndianness())
	{
		WriteIt(4, &value);
		return;
	}
	const unsigned char* src = reinterpret_cast<const unsigned char*>(&value);
	unsigned char swapped[4] = { src[3], src[2], src[1], src[0] };
	WriteIt(4, swapped);
}

/// origin binary
#include "FirePlace/FireWorks/FString.h"

/// 0x08d8031a
void FStringA::Copy(int iLength, LPCSTR lpszSource, int iFirst)
{
	if (iLength < 0 || iFirst < 0)
		return;
	if (m_pszString == lpszSource)
		return;
	if (iLength == 0 || lpszSource == NULL)
	{
		Empty();
		return;
	}
	SetLength(iLength);
	memmove(m_pszString, lpszSource + iFirst, iLength);
	m_pszString[GetLength()] = '\0';
}

/// 0x08d8249e
void FStringA::Format(LPCSTR lpszFormat, ...)
{
	va_list vaArgList;
	va_start(vaArgList, lpszFormat);
	FormatV(lpszFormat, vaArgList);
	va_end(vaArgList);
}

/// 0x08d81e0e
int FStringA::Replace(char chOld, char chNew)
{
	int iCount = 0;
	if (chOld != chNew)
	{
		char * pch = m_pszString;
		char * pchEnd = m_pszString + GetLength();
		for (; pch < pchEnd; ++pch)
		{
			if (*pch == chOld)
			{
				*pch = chNew;
				++iCount;
			}
		}
	}
	return iCount;
}

/// 0x08d7fe88
void FStringA::SetAt(int iIndex, int iLength, LPCSTR lpszSource, int iFirst)
{
	if (iLength <= 0 || iFirst < 0)
		return;
	if (static_cast<unsigned int>(iIndex) > static_cast<unsigned int>(GetLength()) || lpszSource == NULL)
		return;
	if (GetLength() < iLength + iIndex)
	{
		SetLength(iLength + iIndex);
		m_pszString[GetLength()] = '\0';
	}
	memmove(m_pszString + iIndex, lpszSource + iFirst, iLength);
}

/// 0x08d841e8
void FStringW::Copy(int iLength, LPCWSTR lpszSource, int iFirst)
{
	if (iLength < 0 || iFirst < 0)
		return;
	if (m_pszString == lpszSource)
		return;
	if (iLength == 0 || lpszSource == NULL)
	{
		Empty();
		return;
	}
	SetLength(iLength);
	wmemmove(m_pszString, lpszSource + iFirst, iLength);
	m_pszString[GetLength()] = L'\0';
}

/// 0x08d8784c
void FStringW::CopyToUTF8(FStringA * pkOutput) const
{
	if (pkOutput == NULL)
		return;
	if (GetLength() > 0)
	{
		CopyToUTF8(m_pszString, pkOutput);
		return;
	}
	if (pkOutput->GetLength() != 0)
		pkOutput->Empty();
}

/// 0x08d85956
void FStringW::ExtractFileTitle(FStringW & szDest, wchar chPathDelimiter) const
{
	FStringW sFileName = ExtractFileName(chPathDelimiter);
	szDest.Copy(sFileName.GetLength(), sFileName);
	int iCount = szDest.ReverseFind(L'.', -1);
	if (iCount >= 0)
		szDest.Mid(szDest, 0, iCount);
}

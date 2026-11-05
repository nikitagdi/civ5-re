/// origin binary
#include "FirePlace/FireWorks/FMemoryStream.h"

/// 0x08d7f1b0
FMemoryStream::FMemoryStream()
{
	m_pStart = NULL;
	m_pData = NULL;
	m_pStop = NULL;
	m_pEndOfBuffer = NULL;
	m_bRealloc = true;
	m_bReallocMin = false;
	m_uMemoryPool = 2;
	m_uMemoryTag = 0;
}

/// 0x08d7f882
FMemoryStream::~FMemoryStream()
{
	if (m_bRealloc)
	{
		if (m_pStart != NULL)
			delete[] m_pStart;
		m_pStart = NULL;
	}
}

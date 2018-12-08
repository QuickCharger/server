#include "CBufferSend.h"
#include <stdlib.h>
#include <string.h>

CBufferSend::CBufferSend()
	: CBuffer()
{
}

CBufferSend::CBufferSend(int a_nBufferSize)
	: CBuffer(a_nBufferSize)
{
}

CBufferSend::~CBufferSend()
{
}

CBufferSend::EStatus CBufferSend::Append(const char* a_pData, int a_nSize)
{
	int packSize = a_nSize + m_skIntSize;
	char *p = new char[packSize];
	memset(p, 0, packSize);
	p[0] = packSize >> 24;
	p[1] = packSize >> 16;
	p[2] = packSize >> 8;
	p[3] = packSize >> 0;
	memcpy(p + m_skIntSize, a_pData, a_nSize);
	if (!CBuffer::Append(p, packSize))
	{
		return eOverPackage;
	}
	return eOK;
}

void CBufferSend::GetBuffer(char*& a_pData, int& a_nSize)
{
	a_pData = nullptr;
	a_nSize = 0;
	CBuffer::GetBuffer(a_pData, a_nSize);
}

void CBufferSend::Clear()
{
	CBuffer::Clear();
}

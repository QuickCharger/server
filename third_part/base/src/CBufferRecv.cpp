#include "CBufferRecv.h"
#include <stdlib.h>
#include <string.h>

CBufferRecv::CBufferRecv()
	: CBuffer()
{
}

CBufferRecv::CBufferRecv(int a_nBufferSize)
	: CBuffer(a_nBufferSize)
{
}

CBufferRecv::~CBufferRecv()
{
}

CBufferRecv::EStatus CBufferRecv::Append(const char* a_pData, unsigned int a_nSize)
{
	if(!CBuffer::Append(a_pData, a_nSize))
	{
		return eOverPackage;
	}
	return eOK;
}

CBufferRecv::EStatus CBufferRecv::GetPackage(char*& a_pData, unsigned int& a_nSize)
{
	a_pData = nullptr;
	a_nSize = 0;
	if (checkParse(a_pData, a_nSize) != eCanRead)
	{
		return eCannotRead;
	}
	return eCanRead;
}

CBufferRecv::EStatus CBufferRecv::GetPackage(int& a_nCode, char*& a_pData, unsigned int& a_nSize)
{
	a_pData = nullptr;
	a_nSize = 0;
	if (checkParse(a_pData, a_nSize) != eCanRead)
	{
		return eCannotRead;
	}
	a_nCode = *(int*)a_pData;
	a_pData += m_skIntSize;
	a_nSize -= m_skIntSize;
	return eCanRead;
}

void CBufferRecv::DropPackage()
{
	char *pData = nullptr;
	unsigned int size = 0;
	CBuffer::GetBuffer(pData, size);
	if (size < m_skIntSize)
		return;
	size = calHeadSize(pData);
	CBuffer::DropFront(size);
}

unsigned int CBufferRecv::calHeadSize(const char *p)
{
	unsigned int nSize = 0;
	nSize |= (p[0] << 24) & 0xFF000000;
	nSize |= (p[1] << 16) & 0xFF0000;
	nSize |= (p[2] << 8) & 0xFF00;
	nSize |= (p[3] << 0) & 0xFF;
	return nSize;
}

enum CBufferRecv::EStatus CBufferRecv::checkParse(char*& a_pData, unsigned int& a_nSize)
{
	char *pData = nullptr;
	unsigned int size = 0;
	this->GetBuffer(pData, size);
	if (size < m_skIntSize)
	{
		return eCannotRead;
	}
	unsigned int packSize = calHeadSize(pData);
	if (packSize > size)
	{
		return eCannotRead;
	}
	a_pData = pData + m_skIntSize;
	a_nSize = packSize - m_skIntSize;
	return eCanRead;
}

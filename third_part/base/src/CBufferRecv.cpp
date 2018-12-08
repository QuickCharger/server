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

CBufferRecv::EStatus CBufferRecv::Append(const char* a_pData, int a_nSize)
{
	if(!CBuffer::Append(a_pData, a_nSize))
	{
		return eOverPackage;
	}
	return CheckParse();
}

CBufferRecv::EStatus CBufferRecv::GetPackage(char*& a_pData, int& a_nSize)
{
	a_pData = nullptr;
	a_nSize = 0;
	if (CheckParse() != eCanRead)
	{
		return eCannotRead;
	}
	int n;
	this->GetBuffer(a_pData, n);
	a_nSize = calHeadSize(a_pData);
	a_pData += m_skIntSize;
	a_nSize -= m_skIntSize;
	return eCanRead;
}

void CBufferRecv::DropPackage()
{
	char *p = nullptr;
	int n = 0;
	CBuffer::GetBuffer(p, n);
	if (n != 0)
	{
		n = calHeadSize(p);
		CBuffer::DropFront(n);
	}
}

enum CBufferRecv::EStatus CBufferRecv::CheckParse()
{
	char *p = nullptr;
	int n = 0;
	this->GetBuffer(p, n);
	if (n < m_skIntSize)
	{
		return eCannotRead;
	}
	int packSize = calHeadSize(p);
	if (packSize <= n)
	{
		return eCanRead;
	}
	return eCannotRead;
}

int CBufferRecv::calHeadSize(const char *p)
{
	int nSize = 0;
	nSize |= (p[0] << 24) & 0xFF000000;
	nSize |= (p[1] << 16) & 0xFF0000;
	nSize |= (p[2] << 8) & 0xFF00;
	nSize |= (p[3] << 0) & 0xFF;
	return nSize;
}

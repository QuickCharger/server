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

CBufferSend::EStatus CBufferSend::Append(const char* a_pData, unsigned int a_nSize)
{
	unsigned int packSize = a_nSize + m_skIntSize;
	if (Available() < packSize)
		return eOverPackage;

	char *alloc = nullptr;
	if (!createPack(packSize, alloc))
	{
		return eAllocateFailed;
	}

	memcpy(alloc + m_skIntSize, a_pData, a_nSize);
	if (!CBuffer::Append(alloc, packSize))
	{
		delete alloc;
		return eOverPackage;
	}
	delete alloc;
	return eOK;
}

CBufferSend::EStatus CBufferSend::Append(int a_nCode, const char* a_pData, unsigned int a_nSize)
{
	unsigned int packSize = a_nSize + m_skIntSize + m_skIntSize;
	if (Available() < packSize)
		return eOverPackage;

	char *alloc = nullptr;
	if (!createPack(packSize, alloc))
	{
		return eAllocateFailed;
	}
	memcpy(alloc + m_skIntSize, &a_nCode, m_skIntSize);
	memcpy(alloc + m_skIntSize + m_skIntSize, a_pData, a_nSize);
	if (!CBuffer::Append(alloc, packSize))
	{
		delete alloc;
		return eOverPackage;
	}
	delete alloc;
	return eOK;
}

void CBufferSend::GetBuffer(char*& a_pData, unsigned int& a_nSize)
{
	a_pData = nullptr;
	a_nSize = 0;
	CBuffer::GetBuffer(a_pData, a_nSize);
}

char* CBufferSend::GetBuffer()
{
	return CBuffer::GetBuffer();
}

unsigned int CBufferSend::GetCurrentSize()
{
	return CBuffer::GetCurrentSize();
};

void CBufferSend::Clear()
{
	CBuffer::Clear();
}

bool CBufferSend::createPack(unsigned int a_nSize, char*& alloc)
{
	alloc = new char[a_nSize];
	if (alloc == nullptr)
		return false;

	memset(alloc, 0, a_nSize);
	alloc[0] = a_nSize >> 24;
	alloc[1] = a_nSize >> 16;
	alloc[2] = a_nSize >> 8;
	alloc[3] = a_nSize >> 0;

	return true;
}

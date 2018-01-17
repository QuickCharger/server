#include "buffer.h"
#include <stdlib.h>
#include <string.h>

CBuffer::CBuffer()
{
	InitBuffer();
}

CBuffer::CBuffer(int a_nBufferSize)
	: m_nBufferSize(a_nBufferSize)
{
	InitBuffer();
}

CBuffer::~CBuffer()
{
	if (m_pBuffer != nullptr)
	{
		delete[]m_pBuffer;
	}
}

enum CBuffer::EState CBuffer::Append(const char* a_pData, int a_uSize)
{
	if ((m_nCurrentSize + a_uSize) > m_nBufferSize)
	{
		return eBufferOverPackage;
	}
	memcpy(m_pBuffer + m_nCurrentSize, a_pData, a_uSize);
	m_nCurrentSize += a_uSize;
	return eBufferOK;
}

void CBuffer::DeleteBuffer(int a_nSize)
{
	int nLastSize = m_nCurrentSize - a_nSize;
	m_nCurrentSize -= a_nSize;
	char *pBuffer = new char[nLastSize];
	memset(pBuffer, 0, sizeof(nLastSize));
	memcpy(pBuffer, m_pBuffer + a_nSize, nLastSize);
	memset(m_pBuffer, 0, m_nBufferSize);
	memcpy(m_pBuffer, pBuffer, nLastSize);
}

void CBuffer::InitBuffer()
{
	if (m_pBuffer == nullptr)
	{
		m_pBuffer = new char[m_nBufferSize];
		if (m_pBuffer == nullptr)
		{
			// TODO
		}
		memset(m_pBuffer, 0, m_nBufferSize);
	}
	memset(m_pBuffer, 0, m_nCurrentSize);
	m_nCurrentSize = 0;
}

char* CBuffer::GetBuffer()
{
	return m_pBuffer;
}

int CBuffer::GetCurrentSize()
{
	return m_nCurrentSize;
}

CSendBuffer::CSendBuffer()
{

}

CSendBuffer::~CSendBuffer()
{

}

enum CSendBuffer::EState CSendBuffer::Append(const char* a_pData, int a_uSize)
{
	CBuffer::Append(a_pData, a_uSize);
}

CRecvBuffer::CRecvBuffer()
	: CBuffer()
{

}

CRecvBuffer::~CRecvBuffer()
{

}

enum CRecvBuffer::EState CRecvBuffer::Append(const char* a_pData, int a_uSize)
{
	CBuffer::EState state = CBuffer::Append(a_pData, a_uSize);
	if (state == eBufferOK)
	{
		return CheckParse();
	}
	return state;
}

enum CRecvBuffer::EState CRecvBuffer::CheckParse()
{
	unsigned int nSize = 0;
	nSize |= (m_pBuffer[0] << 24) & 0xFF000000;
	nSize |= (m_pBuffer[1] << 16) & 0xFF0000;
	nSize |= (m_pBuffer[2] << 8) & 0xFF00;
	nSize |= (m_pBuffer[3] << 0) & 0xFF;
	if (nSize < m_nCurrentSize) {
		return eBufferOK;
	}
	else if (nSize == m_nCurrentSize) {
		return eBufferCanRead;
	}
	else if (nSize > m_nCurrentSize) {
		InitBuffer();
		return eBufferBufHeadErr;
	}
	return eBufferOK;
}

void CRecvBuffer::Clear()
{
	InitBuffer();
}

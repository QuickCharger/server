#include "buffer.h"
#include <stdlib.h>
#include <string.h>

CBuffer::CBuffer(int a_nBufferSize)
	: m_nBufferSize(a_nBufferSize)
{
	if (m_nBufferSize > 0)
	{
		m_pBuffer = new char[m_nBufferSize];
		if (m_pBuffer == nullptr)
		{
			// TODO
		}
		memset(m_pBuffer, 0, m_nBufferSize);
	}
}

CBuffer::~CBuffer()
{
	if (m_pBuffer != nullptr)
	{
		delete[]m_pBuffer;
	}
}

int CBuffer::Append(const char* a_pData, int a_uSize)
{
	if ((m_nCurrentSize + a_uSize) > m_nBufferSize)
	{
		return eReadBufferStateOverPackage;
	}
	memcpy(m_pBuffer + m_nCurrentSize, a_pData, a_uSize);
	m_nCurrentSize += a_uSize;
	return eReadBufferStateOK;
}

int CBuffer::CheckParse()
{
	int nSize = *(int*)m_pBuffer;
	nSize |= (m_pBuffer[0] << 24) & 0xFF000000;
	nSize |= (m_pBuffer[1] << 16) & 0xFF0000;
	nSize |= (m_pBuffer[2] << 8) & 0xFF00;
	nSize |= (m_pBuffer[3] << 0) & 0xFF;
	if (nSize > m_nBufferSize)
	{
		return eReadBufferStateBufHeadErr;
	}
	else if (nSize < m_nCurrentSize)
	{
		return eReadBufferStateOK;
	}
	else
	{
		return eReadBufferStateCanRead;
	}
}

void CBuffer::DeleteBuffer(int a_nSize)
{
	//m_pBuffer
	int nLastSize = m_nCurrentSize - a_nSize;
	m_nCurrentSize -= a_nSize;
	char *pBuffer = new char[nLastSize];
	memset(pBuffer, 0, sizeof(nLastSize));
	memcpy(pBuffer, m_pBuffer + a_nSize, nLastSize);
	memset(m_pBuffer, 0, m_nBufferSize);
	memcpy(m_pBuffer, pBuffer, nLastSize);
}

void CBuffer::ClearBuffer()
{
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

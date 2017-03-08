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
	//char ch[4];
	//ch[0] = m_pRecvBuf[0];
	//ch[1] = m_pRecvBuf[1];
	//ch[2] = m_pRecvBuf[2];
	//ch[3] = m_pRecvBuf[3];
	//int nSize = atoi(ch);
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

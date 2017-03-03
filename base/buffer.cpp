#include "buffer.h"
#include <stdlib.h>
#include <string.h>

CBuffer::CBuffer(int a_nBufferSize)
	: m_nBufSize(a_nBufferSize)
	, m_nCurrentSize(0)
	, m_pRecvBuf(nullptr)
{
	if (m_nBufSize > 0)
	{
		m_pRecvBuf = new char[m_nBufSize];
		memset(m_pRecvBuf, 0, m_nBufSize);
		if (m_pRecvBuf == nullptr)
		{
			// TODO
		}
	}
}

CBuffer::~CBuffer()
{
	if (m_pRecvBuf != nullptr)
	{
		delete[]m_pRecvBuf;
	}
}

int CBuffer::Append(const char* a_pData, int a_uSize)
{
	if ((m_nCurrentSize + a_uSize) > m_nBufSize)
	{
		return eReadBufferStateOverPackage;
	}
	memcpy(m_pRecvBuf + m_nCurrentSize, a_pData, a_uSize);
	m_nCurrentSize += a_uSize;
	return checkParse();
}

int CBuffer::checkParse()
{
	int nSize = *(int*)m_pRecvBuf;
	//char ch[4];
	//ch[0] = m_pRecvBuf[0];
	//ch[1] = m_pRecvBuf[1];
	//ch[2] = m_pRecvBuf[2];
	//ch[3] = m_pRecvBuf[3];
	//int nSize = atoi(ch);
	if (nSize > m_nBufSize)
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

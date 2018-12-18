#include "CBuffer.h"
#include <stdlib.h>
#include <string.h>

CBuffer::CBuffer()
{
	init();
}

CBuffer::CBuffer(unsigned int a_nBufferSize)
	: m_nBufferSize(a_nBufferSize)
{
	init();
}

CBuffer::~CBuffer()
{
	if (m_pBuffer != nullptr)
	{
		delete[]m_pBuffer;
	}
}

void CBuffer::ReAllocate(int a_nBufferSize)
{
	m_nBufferSize = a_nBufferSize;
	init();
}

unsigned int CBuffer::Available()
{
	return m_nBufferSize - m_nCurrentSize;
}

bool CBuffer::Append(const char* a_pData, int a_size)
{
	if ((m_nCurrentSize + a_size) > m_nBufferSize)
	{
		SetErr("over Size.");
		return false;
	}
	if ((m_nCurrentPoint + m_nCurrentSize + a_size) > m_nBufferSize)
	{
		arrange();
	}
	memcpy(m_pBuffer + m_nCurrentPoint + m_nCurrentSize, a_pData, a_size);
	m_nCurrentSize += a_size;
	return true;
}

void CBuffer::DropFront(unsigned int a_nSize)
{
	if (a_nSize >= m_nCurrentSize)
	{
		m_nCurrentPoint = 0;
		m_nCurrentSize = 0;
		return;
	}
	m_nCurrentPoint += a_nSize;
	m_nCurrentSize -= a_nSize;
}

void CBuffer::DropBack(unsigned int a_nSize)
{
	if (a_nSize >= m_nCurrentSize)
	{
		m_nCurrentPoint = 0;
		m_nCurrentSize = 0;
	}
	m_nCurrentSize -= a_nSize;
}

void CBuffer::Clear()
{
	m_nCurrentSize = 0;
	m_nCurrentPoint = 0;
}

unsigned int CBuffer::GetCurrentSize()
{
	return m_nCurrentSize;
};

char* CBuffer::GetBuffer()
{
	return m_pBuffer + m_nCurrentPoint;
}

void CBuffer::GetBuffer(char*&p)
{
	p = m_pBuffer + m_nCurrentPoint;
}

void CBuffer::GetBuffer(char *&p, unsigned int& n)
{
	p = m_pBuffer + m_nCurrentPoint;
	n = m_nCurrentSize;
}

void CBuffer::Test()
{
	m_nBufferSize = 16;
	init();
	this->Append("1234567890", 10);
	this->Append("1234", 4);
	this->Append("1234567890", 10);
	char *p = nullptr;
	unsigned int n = 0;
	GetBuffer(p, n);
	DropFront(8);
	GetBuffer(p, n);
	this->Append("12345", 5);
	this->Append("12345", 5);
	this->Append("12345", 5);
	DropFront(20);

	init();
	this->Append("12345678901234567890",20);
	this->Append("1", 1);
	GetBuffer(p, n);
	DropFront(100);

	//DropBack(100);
}

void CBuffer::init()
{
	if (m_pBuffer)
	{
		delete m_pBuffer;
	}
	m_nCurrentSize = 0;
	m_nCurrentPoint = 0;

	if (m_nBufferSize <= 0)
	{
		return;
	}

	m_pBuffer = new char[m_nBufferSize];
	if (m_pBuffer == nullptr)
	{
		return;
	}
}

void CBuffer::arrange()
{
	char *p = new char[m_nCurrentSize];
	memcpy(p, m_pBuffer + m_nCurrentPoint,m_nCurrentSize);
	memcpy(m_pBuffer, p, m_nCurrentSize);
	m_nCurrentPoint = 0;
	delete p;
}

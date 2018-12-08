#pragma once

#include "CErrRecord.h"

/*
* no overflow judge
*/
class CBuffer : public CErrRecord
{
public:
	CBuffer();
	CBuffer(unsigned int a_nBufferSize);
	virtual ~CBuffer();
	void ReAllocate(int a_nBufferSize);

	bool Append(const char* a_pData, int a_size);
	void DropFront(unsigned int a_nSize);
	void DropBack(unsigned int a_nSize);
	void Clear();

	int  GetCurrentSize()	{ return m_nCurrentSize; };
	char* GetBuffer()		{ return m_pBuffer + m_nCurrentPoint; }
	void GetBuffer(char*&p)	{ p = m_pBuffer + m_nCurrentPoint; }
	void GetBuffer(char *&p, int& n);
	void Test();

private:
	void init();
	void arrange();

private:
	char *m_pBuffer = nullptr;
	unsigned int m_nBufferSize = 10240;
	unsigned int m_nCurrentSize = 0;
	unsigned int m_nCurrentPoint = 0;
};

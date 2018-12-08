#pragma once

#include "CBuffer.h"

class CBufferSend : CBuffer
{
public:
	enum EStatus {
		eOverPackage = -1,
		eOK = 0,
	};
public:
	CBufferSend();
	CBufferSend(int a_nBufferSize);
	virtual ~CBufferSend();
	EStatus Append(const char* a_pData, int a_nSize);
	void GetBuffer(char*& a_pData, int& a_nSize);
	void Clear();

private:
	const static int m_skIntSize = sizeof(int);
};

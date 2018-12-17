#pragma once

#include "CBuffer.h"

class CBufferSend : protected CBuffer
{
public:
	enum EStatus {
		eOK = 0,
		eOverPackage = -1,
		eAllocateFailed = -2,
	};
public:
	CBufferSend();
	CBufferSend(int a_nBufferSize);
	virtual ~CBufferSend();
	EStatus Append(const char* a_pData, unsigned int a_nSize);
	EStatus Append(int a_nCode, const char* a_pData, unsigned int a_nSize);
	void GetBuffer(char*& a_pData, unsigned int& a_nSize);
	char* GetBuffer();
	unsigned int GetCurrentSize();
	void Clear();

private:
	bool createPack(unsigned int a_nSize, char*& alloc);
};

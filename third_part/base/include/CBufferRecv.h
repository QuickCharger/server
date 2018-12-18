#pragma once

#include "CBuffer.h"

class CBufferRecv : private CBuffer
{
public:
	enum EStatus {
		eBufHeadErr = -2,
		eOverPackage = -1,
		eOK = 0,
		eCanRead = 1,
		eCannotRead = 2,
	};

public:
	CBufferRecv();
	CBufferRecv(int a_nBufferSize);
	virtual ~CBufferRecv();
	EStatus Append(const char* a_pData, unsigned int a_nSize);
	EStatus GetPackage(char*& a_pData, unsigned int& a_nSize);
	EStatus GetPackage(int& a_nCode, char*& a_pData, unsigned int& a_nSize);
	void DropPackage();

private:
	unsigned int calHeadSize(const char *p);
	enum EStatus checkParse(char*& a_pData, unsigned int& a_nSize);
};

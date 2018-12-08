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
	EStatus Append(const char* a_pData, int a_nSize);
	EStatus GetPackage(char*& a_pData, int& a_nSize);
	void DropPackage();
	enum EStatus CheckParse();

private:
	int calHeadSize(const char *p);

private:
	const static int m_skIntSize = sizeof(int);
};

#ifndef _BASE_BUFFER_H_
#define _BASE_BUFFER_H_

#include "macro.h"

class CBuffer
{
public:
	enum{
		eReadBufferStateBufHeadErr = -2,
		eReadBufferStateOverPackage = -1,
		eReadBufferStateOK = 0,
		//eReadBufferStateCanRead = 1,
	};
public:
	CBuffer(int a_nBufferSize);
	virtual ~CBuffer();
	int Append(const char* a_pData, int a_uSize);
	//int CheckParse();
	void ClearBuffer();

public:
	char* GetBuffer();
	int GetCurrentSize();

private:
	char *m_pBuffer = nullptr;
	int m_nBufferSize = 1024;
	int m_nCurrentSize = 0;
};

#endif

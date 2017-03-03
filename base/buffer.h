#ifndef _BASE_BUFFER_H_
#define _BASE_BUFFER_H_

class CBuffer
{
	enum{
		eReadBufferStateBufHeadErr = -2,
		eReadBufferStateOverPackage = -1,
		eReadBufferStateOK = 0,
		eReadBufferStateCanRead = 1,
	};
public:
	CBuffer(int a_nBufferSize);
	~CBuffer();
	int Append(const char* a_pData, int a_uSize);

private:
	int checkParse();

	int m_nBufSize;
	int m_nCurrentSize;
	char* m_pRecvBuf;
};

#endif

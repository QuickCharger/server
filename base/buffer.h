#ifndef _BASE_BUFFER_H_
#define _BASE_BUFFER_H_

class CBuffer
{
	enum{
		eOverPackage = -1,
		eOK = 0,
		eCanRead = 1,
	};
public:
	CBuffer(int a_nBufferSize);
	~CBuffer();
	int Append(const char* a_pData, int a_uSize);

private:
	bool checkParse();

	int m_nBufSize;
	int m_nCurrentSize;
	char* m_pRecvBuf;
};

#endif

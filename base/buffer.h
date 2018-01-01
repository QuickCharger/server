#ifndef _BASE_BUFFER_H_
#define _BASE_BUFFER_H_

#include "macro.h"

/*
* ��������
*  ��ʼ��
*  ���
*    ���buf �򷵻�overflow
*  ɾ��
*    ��ʽ��buf
*/
class CBuffer
{
public:
	enum EState{
		eBufferBufHeadErr = -2,
		eBufferOverPackage = -1,
		eBufferOK = 0,
		eBufferCanRead = 1,
	};
public:
	CBuffer();
	CBuffer(int a_nBufferSize);
	virtual ~CBuffer();
	enum CBuffer::EState Append(const char* a_pData, int a_uSize);
	void DeleteBuffer(int a_nSize);
	void InitBuffer();

public:
	char* GetBuffer();
	int GetCurrentSize();

public:
	char *m_pBuffer = nullptr;
	int m_nBufferSize = 10240;
	int m_nCurrentSize = 0;
};

/*
* ����buffer
* ����
*   ���Ͷ������ݾͼ�ȥ��������
*   ʱʱ����buffer���� ÿ�η��͵�buf����Ҫ���¼���
*/
class CSendBuffer : protected CBuffer
{
	CSendBuffer();
	~CSendBuffer();
	enum CBuffer::EState Append(const char *a_pData, int a_uSize);

};

/*
* ����buffer
* ����
*   ���ܵ����ݼ����Ƿ�ɶ�
*/
class CRecvBuffer : protected CBuffer
{
public:
	CRecvBuffer();
	~CRecvBuffer();

	enum CBuffer::EState Append(const char *a_pData, int a_uSize);
	enum CBuffer::EState CheckParse();
	void Clear();
};

#endif

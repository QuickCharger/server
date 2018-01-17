#ifndef _BASE_BUFFER_H_
#define _BASE_BUFFER_H_

#include "macro.h"

/*
* 基本功能
*  初始化
*  添加
*    添加buf 或返回overflow
*  删除
*    格式化buf
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
* 发送buffer
* 功能
*   发送多少数据就减去多少数据
*   时时计算buffer长度 每次发送的buf长度要重新计算
*/
class CSendBuffer : protected CBuffer
{
	CSendBuffer();
	~CSendBuffer();
	enum CBuffer::EState Append(const char *a_pData, int a_uSize);

};

/*
* 接受buffer
* 功能
*   接受的数据计算是否可读
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

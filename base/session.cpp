#include "session.h"
#include <assert.h>
#include <iostream>
#include <time.h>
#include "buffer.h"
#include "log.h"

CSession::CSession(event_base* a_pEventBase)
{
	m_pEventBase = a_pEventBase;
	m_pReadBuffer = new CBuffer(4000);	//4KB per buffer
	m_pSendBuffer = new CBuffer(4000);	
}

CSession::~CSession()
{
	m_pEventBase = nullptr;
}

void CSession::Connect()
{
	if (m_Socket == 0)
	{
		m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_strServerIP.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_nPort);
	if (connect(m_Socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == 0)
	{
		LOG(INFO) << "ConnectServer: " << m_strServerName << ":" << m_nPort << " success";
		evutil_make_socket_nonblocking(m_Socket);
		assert(m_pBufferEvent == nullptr);
		m_pBufferEvent = bufferevent_socket_new(m_pEventBase, m_Socket, BEV_OPT_CLOSE_ON_FREE);
		//bufferevent_setcb(pBufferEvent, OnReadCB, OnWriteCB, OnErrorCB, this);
		bufferevent_setcb(
			m_pBufferEvent,
			[](bufferevent *a_pBev, void *a_pArg){
			CSession* pSession = static_cast<CSession*>(a_pArg);
			pSession->OnReadCB(a_pBev, a_pArg);
		},
			[](bufferevent *a_pBev, void *a_pArg){
			CSession* pSession = static_cast<CSession*>(a_pArg);
			pSession->OnWriteCB(a_pBev, a_pArg);
		},
			[](bufferevent *a_pBen, short a_nEvent, void *a_pArg){
			CSession* pSession = static_cast<CSession*>(a_pArg);
			pSession->OnErrorCB(a_nEvent);
		},
			this);
		bufferevent_enable(m_pBufferEvent, EV_READ | EV_WRITE | EV_PERSIST);
	}
	else if (m_bAutoConnect)
	{
		event* evListen2 = evtimer_new(m_pEventBase, 
			[](int, short, void *a_pArg){
			CSession* pSession = static_cast<CSession*>(a_pArg);
			pSession->ReConnect();
		},
			this);

		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		int nResult = evtimer_add(evListen2, &tv);
	}
	LOG(INFO) << "fd:" << m_Socket << ". IP:" << m_strServerIP.c_str() << ". Port:" << m_nPort;
}

void CSession::CloseSocket()
{
	if (m_Socket != 0)
	{
		LOG(INFO) << "socket close. fd: " << m_Socket;
		closesocket(m_Socket);
		m_Socket = 0;
	}
}

void CSession::ReConnect()
{
	CloseSocket();
	if (m_bAutoConnect)
	{
		Connect();
	}
}

void CSession::OnReadCB(bufferevent *a_pBev, void *a_pArg)
{
#define MAXREADBUFFER 1024
	char *pBuf = new char[MAXREADBUFFER];
	int nReadSize;
	while (nReadSize = bufferevent_read(a_pBev, pBuf, MAXREADBUFFER), nReadSize > 0)
	{
		memset(pBuf, 0, MAXREADBUFFER);
		CSession* pSession = static_cast<CSession*>(a_pArg);
		m_pReadBuffer->Append(pBuf, nReadSize);
	}
	int nState = m_pReadBuffer->CheckParse();
	if (nState == CBuffer::eReadBufferStateCanRead)
	{
		m_funcReadCB((void*)m_pReadBuffer->GetBuffer());
	}
	else if (nState != CBuffer::eReadBufferStateOK)
	{
		// TODO
	}
	delete[]pBuf;
#undef MAXREADBUFFER
}

void CSession::OnWriteCB(bufferevent *a_pBev, void *a_pArg)
{
	LOG(INFO) << "OnWriteCB";
	if (m_pSendBuffer->GetCurrentSize() != 0)
	{
		if (bufferevent_write(a_pBev, m_pSendBuffer->GetBuffer(), m_pSendBuffer->GetCurrentSize()) == 0)
		{
			m_funcWriteCB((void*)m_pSendBuffer->GetBuffer());
			m_pSendBuffer->ClearBuffer();
		}
		else
		{
			m_funcWriteCB((void*)(1));
		}
	}
}

void CSession::OnErrorCB(short a_nEvent)
{
	LOG(INFO) << "OnErrorCB";
	if (m_pBufferEvent != nullptr)
	{
		bufferevent_free(m_pBufferEvent);
		m_pBufferEvent = nullptr;
	}
	if (a_nEvent & BEV_EVENT_TIMEOUT)
	{
		LOG(WARNING) << "socket Time out";
	}
	else if (a_nEvent & BEV_EVENT_EOF)
	{
		LOG(WARNING) << "connection closed";
		ReConnect();
	}
	else if (a_nEvent & BEV_EVENT_ERROR)
	{
		LOG(WARNING) << "Unknown error";
	}
	else
	{
		LOG(WARNING) << "unknown error";
	}
}

void CSession::Send(char* a_pBuffer, int a_nSize)
{
	m_pSendBuffer->Append(a_pBuffer, a_nSize);
	if (bufferevent_write(m_pBufferEvent, m_pSendBuffer->GetBuffer(), m_pSendBuffer->GetCurrentSize()) == 0)
	{
		m_funcWriteCB((void*)m_pSendBuffer->GetBuffer());
		m_pSendBuffer->ClearBuffer();
	}
	else
	{
		m_funcWriteCB((void*)(1));
	}
}

void CSession::SetSocket(SOCKET a_Socket)
{
	assert(m_Socket == 0);
	m_Socket = a_Socket;
}

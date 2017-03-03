#include "session.h"
#include <assert.h>
#include <iostream>
#include <time.h>
#include "buffer.h"
//#include "log.h"

CSession::CSession(event_base* a_pEventBase)
{
	m_pBuffer = new CBuffer(4000);	//4KB per buffer
	m_pReadBuffer = new char[m_nReadBufferSize];
	memset(m_pReadBuffer, 0, m_nReadBufferSize);
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
	int nResult = connect(m_Socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	std::cout << "ConnectServer: " << m_strServerName << ":" << m_nPort << ". Result:" << nResult << std::endl;
	if (nResult == 0)
	{
		evutil_make_socket_nonblocking(m_Socket);
		struct bufferevent *pBufferEvent = bufferevent_socket_new(m_pEventBase, m_Socket, BEV_OPT_CLOSE_ON_FREE);
		//bufferevent_setcb(pBufferEvent, OnReadCB, OnWriteCB, OnErrorCB, this);
		bufferevent_setcb(
			pBufferEvent,
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
			pSession->OnErrorCB(a_pBen, a_nEvent, a_pArg);
		},
			this);
		bufferevent_enable(pBufferEvent, EV_READ | EV_WRITE | EV_PERSIST);
	}
	else if (m_bAutoConnect)
	{
		event* evListen2 = evtimer_new(m_pEventBase, ReConnect, this);
		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		int nResult = evtimer_add(evListen2, &tv);
	}
	std::cout << "fd:" << m_Socket << ". IP:" << m_strServerIP.c_str() << ". Port:" << m_nPort << std::endl;
}

void CSession::CloseSocket()
{
	if (m_Socket != 0)
	{
		std::cout << "socket close. fd: " << m_Socket << std::endl;
		closesocket(m_Socket);
		m_Socket = 0;
	}
}

void CSession::ReConnect(int a_nClientFD, short a_nEvent, void *a_pArg)
{
	CSession* pSession = static_cast<CSession*>(a_pArg);
	if (pSession != nullptr)
	{
		pSession->CloseSocket();
		if (pSession->GetAutoConnect())
		{
			pSession->Connect();
		}
	}
	else
	{
		assert(true);
	}
}

void CSession::OnReadCB(bufferevent *a_pBev, void *a_pArg)
{
	//DLOG(INFO) << "OnReadCB";
	int nReadSize;
	while (nReadSize = bufferevent_read(a_pBev, m_pReadBuffer, m_nReadBufferSize), nReadSize > 0)
	{
		CSession* pSession = static_cast<CSession*>(a_pArg);
		m_pBuffer->Append(m_pReadBuffer, nReadSize);
		memset(m_pReadBuffer, 0, m_nReadBufferSize);
	}
}

void CSession::OnWriteCB(bufferevent *a_pBev, void *a_pArg)
{
	std::cout << "OnWriteCB" << std::endl;
}

void CSession::OnErrorCB(bufferevent *a_pBen, short a_nEvent, void *a_pArg)
{
	std::cout << "OnErrorCB" << std::endl;
	evutil_socket_t fd = bufferevent_getfd(a_pBen);
	printf("fd=%u ", fd);
	if (a_nEvent & BEV_EVENT_TIMEOUT)
	{
		std::cout << "socket Time out" << std::endl;
	}
	else if (a_nEvent & BEV_EVENT_EOF)
	{
		std::cout << "connection closed" << std::endl;
		CSession* pSession = static_cast<CSession*>(a_pArg);
		if (pSession != nullptr)
		{
			pSession->CloseSocket();
			ReConnect(0, 0, a_pArg);
		}
		else
		{
			assert(true);
		}
	}
	else if (a_nEvent & BEV_EVENT_ERROR)
	{
		std::cout << "Unknown error" << std::endl;
	}
	else
	{
		std::cout << "unknown error" << std::endl;
	}
	bufferevent_free(a_pBen);
}


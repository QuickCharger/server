#include "session.h"
#include <assert.h>
#include <iostream>
#include <time.h>

CSession::CSession(event_base* a_pEventBase)
	: m_EventBase(a_pEventBase)
	, m_Socket(0)
	, m_strName("")
	, m_strServerIP("")
	, m_nPort(0)
	, m_bAutoConnect(false)
{

}

CSession::~CSession()
{
	m_EventBase = nullptr;
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
	std::cout << "ConnectServer: " << m_strName << ":" << m_nPort << ". Result:" << nResult << std::endl;
	if (nResult == 0)
	{
		evutil_make_socket_nonblocking(m_Socket);
		struct bufferevent *pBufferEvent = bufferevent_socket_new(m_EventBase, m_Socket, BEV_OPT_CLOSE_ON_FREE);
		bufferevent_setcb(pBufferEvent, OnReadCB, OnWriteCB, OnErrorCB, this);
		bufferevent_enable(pBufferEvent, EV_READ | EV_WRITE | EV_PERSIST);
	}
	else if (m_bAutoConnect)
	{
		event* evListen2 = evtimer_new(m_EventBase, ReConnect, this);
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
	std::cout << "OnReadCB" << std::endl;
#define MAX_LINE 10240
	char line[MAX_LINE];
	memset(line, 0, MAX_LINE);
	static int nRecv = 0;
	static time_t tBegin = 0;
	static time_t tEnd = 0;
	int n;
	evutil_socket_t fd = bufferevent_getfd(a_pBev);
	while (n = bufferevent_read(a_pBev, line, MAX_LINE), n > 0)
	{
		if (tBegin == 0)
			tBegin = time(nullptr);
		tEnd = time(nullptr);
		nRecv += n;
		//line[n] = '\0';
		bufferevent_write(a_pBev, line, n);
		std::cout << line << std::endl;
	}
#undef MAX_LINE
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


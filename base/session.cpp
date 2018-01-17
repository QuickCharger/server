#include "session.h"
#include <assert.h>
#include <iostream>
#include <time.h>
#include "buffer.h"
#include "log.h"
#include "IServer.h"
#include "message_lite.h"
//#include "ISessionSelector.h"

CSession::CSession(IServer *a_pServer, event_base* a_pEventBase, SOCKET a_Socket)
{
	m_Server = a_pServer;
	m_pEventBase = a_pEventBase;
	m_pReadBuffer = new CBuffer(4000);	//4KB per buffer
	m_pSendBuffer = new CBuffer(4000);	
	m_Socket = a_Socket;

	initSocket();
}

CSession::CSession(IServer *a_pServer, event_base *a_pEventBase, const std::string& a_strName, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect)
{
	m_Server = a_pServer;
	m_pEventBase = a_pEventBase;
	m_pReadBuffer = new CBuffer(4000);
	m_pSendBuffer = new CBuffer(4000);
	m_strServerName = a_strName;
	m_strServerIP = a_strIP;
	m_nPort = a_nPort;
	m_bAutoConnect = a_bAutoConnect;
	//Connect();
	addConnectTimer();
}

CSession::~CSession()
{
	m_pEventBase = nullptr;
	if (m_Socket != 0)
	{
		closesocket(m_Socket);
	}
	if (m_pReadBuffer != nullptr)
	{
		delete m_pReadBuffer;
		m_pReadBuffer = nullptr;
	}
	if (m_pSendBuffer != nullptr)
	{
		delete m_pSendBuffer;
		m_pSendBuffer = nullptr;
	}
}

//void CSession::Connect()
//{
//	addConnectTimer();
//}

void CSession::CloseSocket()
{
	if (m_Socket != 0)
	{
		LOG(INFO) << "socket close. fd: " << m_Socket;
		closesocket(m_Socket);
		m_Socket = 0;
	}
}

void CSession::DoConnect()
{
	assert(m_Socket == 0);
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_strServerIP.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_nPort);
	if (connect(m_Socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == 0)
	{
		LOG(INFO) << "ConnectServer: " << m_strServerName << ":" << m_nPort << " success. Socket: " << m_Socket;
		initSocket();
		//m_funcConnectCB();
		m_Server->OnConnect(this);
	}
	else
	{
		CloseSocket();
		if (m_bAutoConnect)
		{
			addConnectTimer();
		}
		LOG(INFO) << "ConnectServer: " << m_strServerName << ":" << m_nPort << " failed.";
	}
}

void CSession::OnReadCB(bufferevent *a_pBev, void *a_pArg)
{

#define MAXREADBUFFER 1024
	char *pBuf = new char[MAXREADBUFFER];
	memset(pBuf, 0, MAXREADBUFFER);
	//pBuf可能溢出，unfinish
	//while中append可能溢出
	int nReadSize;
	while (nReadSize = bufferevent_read(a_pBev, pBuf, MAXREADBUFFER), nReadSize > 0)
	{
		CSession* pSession = static_cast<CSession*>(a_pArg);
		m_pReadBuffer->Append(pBuf, nReadSize);
		memset(pBuf, 0, MAXREADBUFFER);
	}
#undef MAXREADBUFFER

	char *pDest;
	int nCode = 0;
	//大于0表示成功收到一次可解析的数据，
	while ((nReadSize = m_Server->OnUnPackCB(m_pReadBuffer->GetBuffer(), m_pReadBuffer->GetCurrentSize(), nCode, &pDest)) > 0)
	{
		m_pReadBuffer->DeleteBuffer(nReadSize);
		m_Server->OnReadCB(nCode, pDest);
		//m_Server->OnReadCB(pDecodeBuf);
	}
	delete []pBuf;
	delete []pDest;
}

void CSession::OnWriteCB(bufferevent *a_pBev, void *a_pArg)
{
	//LOG(INFO) << "OnWriteCB";
	if (m_pSendBuffer->GetCurrentSize() != 0)
	{
		//if (bufferevent_write(a_pBev, m_pSendBuffer->GetBuffer(), m_pSendBuffer->GetCurrentSize()) == 0)
		//{
		//	m_Server->OnWriteCB((void*)m_pSendBuffer->GetBuffer());
		//	m_pSendBuffer->ClearBuffer();
		//}
		//else
		//{
		//	m_Server->OnWriteCB((void*)1);
		//}
	}
	//m_funcWriteCB((void*)(0));
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
		LOG(WARNING) << "BEV_EVENT_TIMEOUT";
	}
	else if (a_nEvent & BEV_EVENT_EOF)
	{
		LOG(WARNING) << "BEV_EVENT_EOF";
		CloseSocket();
		if (m_bAutoConnect)
		{
			addConnectTimer();
		}
	}
	else if (a_nEvent & BEV_EVENT_ERROR)
	{
		LOG(WARNING) << "BEV_EVENT_ERROR";
		CloseSocket();
		if (m_bAutoConnect)
		{
			addConnectTimer();
		}
	}
	else
	{
		LOG(WARNING) << "unknown error. Event Code: " << a_nEvent;
	}
}

void CSession::Send(int a_nMsgCode, ::google::protobuf::Message &a_Msg)
{
	std::string strSend = a_Msg.SerializeAsString();
	int nSize = strSend.size();
	char *pBuf = nullptr;
	m_Server->OnPackCB(strSend.c_str(), a_nMsgCode, nSize, &pBuf);
	m_pSendBuffer->Append(pBuf, nSize);
	if (bufferevent_write(m_pBufferEvent, m_pSendBuffer->GetBuffer(), m_pSendBuffer->GetCurrentSize()) == 0)
	{
		m_Server->OnWriteCB((void*)m_pSendBuffer->GetBuffer());
		m_pSendBuffer->ClearBuffer();
	}
	else
	{
		m_Server->OnWriteCB(nullptr);
	}
	delete[]pBuf;
}

void CSession::Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg)
{
	std::string strSend = a_pMsg == nullptr ? "" : a_pMsg->SerializeAsString();
	int nSize = strSend.size();
	char *pBuf = nullptr;
	m_Server->OnPackCB(strSend.c_str(), a_nMsgCode, nSize, &pBuf);
	m_pSendBuffer->Append(pBuf, nSize);
	if (bufferevent_write(m_pBufferEvent, m_pSendBuffer->GetBuffer(), m_pSendBuffer->GetCurrentSize()) == 0)
	{
		m_Server->OnWriteCB((void*)m_pSendBuffer->GetBuffer());
		m_pSendBuffer->ClearBuffer();
	}
	else
	{
		m_Server->OnWriteCB(nullptr);
	}
	delete[]pBuf;
}

void CSession::addConnectTimer()
{
	event* evListen2 = evtimer_new(m_pEventBase,
		[](int, short, void *a_pArg){
		CSession* pSession = static_cast<CSession*>(a_pArg);
		pSession->DoConnect();
	},
		this);

	timeval tv;
	tv.tv_sec = 2;
	tv.tv_usec = 0;
	int nResult = evtimer_add(evListen2, &tv);
}

void CSession::initSocket()
{
	evutil_make_socket_nonblocking(m_Socket);
	assert(m_pBufferEvent == nullptr);
	m_pBufferEvent = bufferevent_socket_new(m_pEventBase, m_Socket, BEV_OPT_CLOSE_ON_FREE);
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

	m_Server->SetCallBack();
}

#include "session.h"
#include <assert.h>
#include <iostream>
#include <time.h>

#include "google/protobuf/message_lite.h"
#include "base.h"

#include "IServer.h"
#include "Libevent.h"
#include "log.h"
//#include "ISessionSelector.h"

CSession::CSession(IServer *a_pServer, evutil_socket_t a_Socket)
{
	m_Server = a_pServer;
	m_pRecvBuffer = new char[m_skReadBufferSize];
	m_pReadBuffer = new CBufferRecv(m_skBufferSize);
	m_pSendBuffer = new CBufferSend(m_skBufferSize);
	m_Socket = a_Socket;

	initSocket();
}

CSession::CSession(IServer *a_pServer, const std::string& a_strName, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect)
{
	m_Server = a_pServer;
	m_pRecvBuffer = new char[m_skReadBufferSize];
	m_pReadBuffer = new CBufferRecv(m_skBufferSize);
	m_pSendBuffer = new CBufferSend(m_skBufferSize);
	m_strServerName = a_strName;
	m_strServerIP = a_strIP;
	m_nPort = a_nPort;
	m_bAutoConnect = a_bAutoConnect;
	//Connect();
	addConnectTimer();
}

CSession::~CSession()
{
	if (m_Socket != 0)
	{
		closesocket(m_Socket);
	}
	if (m_pReadBuffer)
	{
		delete m_pReadBuffer;
		m_pReadBuffer = nullptr;
	}
	if (m_pSendBuffer)
	{
		delete m_pSendBuffer;
		m_pSendBuffer = nullptr;
	}
	if (m_pRecvBuffer)
	{
		delete m_pRecvBuffer;
		m_pRecvBuffer = nullptr;
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
		//closesocket(m_Socket);
		EVUTIL_CLOSESOCKET(m_Socket);
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
	//bufferevent_socket_connect();
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

	memset(m_pRecvBuffer, 0, m_skReadBufferSize);
	//while中append可能溢出 todo
	unsigned int size;
	while (size = bufferevent_read(a_pBev, m_pRecvBuffer, m_skReadBufferSize) && size > 0)
	{
		CSession* pSession = static_cast<CSession*>(a_pArg);
		m_pReadBuffer->Append(m_pRecvBuffer, size);
		memset(m_pRecvBuffer, 0, m_skReadBufferSize);
	}

	int nCode = 0;
	char *pSrc = nullptr;
	//大于0表示成功收到一次可解析的数据，
	while (m_pReadBuffer->GetPackage(nCode, pSrc, size) == CBufferRecv::eCanRead)
	{
		std::string msg(pSrc, size);
		m_Server->OnReadCB(nCode, msg);
		m_pReadBuffer->DropPackage();
	}
}

void CSession::OnWriteCB(bufferevent *a_pBev, void *a_pArg)
{
	//LOG(INFO) << "OnWriteCB";
	//if (m_pSendBuffer->GetCurrentSize() != 0)
	//{
		//if (bufferevent_write(a_pBev, m_pSendBuffer->GetBuffer(), m_pSendBuffer->GetCurrentSize()) == 0)
		//{
		//	m_Server->OnWriteCB((void*)m_pSendBuffer->GetBuffer());
		//	m_pSendBuffer->ClearBuffer();
		//}
		//else
		//{
		//	m_Server->OnWriteCB((void*)1);
		//}
	//}
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
	send(a_nMsgCode, a_Msg.SerializeAsString());
}

void CSession::Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg)
{
	send(a_nMsgCode, a_pMsg == nullptr ? "" : a_pMsg->SerializeAsString());
}

void CSession::addConnectTimer()
{
	event* evListen2 = evtimer_new(CLibevent::GetInstance(),
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
	m_pBufferEvent = bufferevent_socket_new(CLibevent::GetInstance(), m_Socket, BEV_OPT_CLOSE_ON_FREE);
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

bool CSession::send(int a_nMsgCode, std::string& str)
{
	int result = m_pSendBuffer->Append(a_nMsgCode, str.c_str(), str.size());
	if (result != CBufferSend::eOK)
	{
		LOG(WARNING) << "over pack";
		return false;
	}

	if (bufferevent_write(m_pBufferEvent, m_pSendBuffer->GetBuffer(), m_pSendBuffer->GetCurrentSize()) == 0)
	{
		m_Server->OnWriteCB((void*)m_pSendBuffer->GetBuffer());
		m_pSendBuffer->Clear();
	}
	else
	{
		m_Server->OnWriteCB(nullptr);
	}

	return true;
}

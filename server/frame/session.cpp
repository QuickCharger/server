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
	m_Socket = a_Socket;

	initBuffer();
	initSocket();
}

CSession::~CSession()
{
	DLOG(INFO) << "~CSESSION";
	Close();
}

void CSession::Close()
{
	closeBuffer();
	if (m_Socket)
	{
		LOG(INFO) << "socket close. fd: " << m_Socket;
		//closesocket(m_Socket);
		EVUTIL_CLOSESOCKET(m_Socket);
		m_Socket = 0;
	}
	if (m_pEvent)
	{
		bufferevent_free(m_pEvent);
		m_pEvent = nullptr;
	}
}

void CSession::OnReadCB(bufferevent *a_pBev, void *a_pArg)
{

	memset(m_pRecvBuffer, 0, m_skReadBufferSize);
	//while中append可能溢出 todo
	unsigned int size;
	while (size = bufferevent_read(a_pBev, m_pRecvBuffer, m_skReadBufferSize), size > 0)
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

/*
* 暂不清楚各种错误的区别, 统一认为错误
*/
void CSession::OnErrorCB(short a_nEvent)
{
	DLOG(INFO) << "OnErrorCB, eventId " << a_nEvent;
	if (a_nEvent & BEV_EVENT_TIMEOUT)
	{
		DLOG(WARNING) << "BEV_EVENT_TIMEOUT";
	}
	else if (a_nEvent & BEV_EVENT_EOF)
	{
		DLOG(INFO) << "fd disconnected.";
	}
	else if (a_nEvent & BEV_EVENT_ERROR)
	{
		DLOG(WARNING) << "BEV_EVENT_ERROR";
	}
	else
	{
		LOG(WARNING) << "unknown error. Event Code: " << a_nEvent;
	}
	m_Server->OnErrorCB(this);
	delete this;
}

void CSession::Send(int a_nMsgCode, ::google::protobuf::Message &a_Msg)
{
	send(a_nMsgCode, a_Msg.SerializeAsString());
}

void CSession::Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg)
{
	send(a_nMsgCode, a_pMsg == nullptr ? "" : a_pMsg->SerializeAsString());
}

void CSession::initSocket()
{
	evutil_make_socket_nonblocking(m_Socket);
	assert(m_pEvent == nullptr);
	m_pEvent = bufferevent_socket_new(CLibevent::GetInstance(), m_Socket, BEV_OPT_CLOSE_ON_FREE);
	bufferevent_setcb(
		m_pEvent,
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
	bufferevent_enable(m_pEvent, EV_READ | EV_WRITE | EV_PERSIST);
}

void CSession::initBuffer()
{
	m_pRecvBuffer = new char[m_skReadBufferSize];
	m_pReadBuffer = new CBufferRecv(m_skBufferSize);
	m_pSendBuffer = new CBufferSend(m_skBufferSize);
	reinitBuffer();
}

void CSession::reinitBuffer()
{
	memset(m_pRecvBuffer, 0, m_skReadBufferSize);
	m_pReadBuffer->Clear();
	m_pSendBuffer->Clear();
}

void CSession::closeBuffer()
{
	if(m_pRecvBuffer)
		delete m_pRecvBuffer;
	if (m_pReadBuffer)
		delete m_pReadBuffer;
	if (m_pSendBuffer)
		delete m_pSendBuffer;
	m_pRecvBuffer = nullptr;
	m_pReadBuffer = nullptr;
	m_pSendBuffer = nullptr;
}

bool CSession::send(int a_nMsgCode, std::string& str)
{
	int result = m_pSendBuffer->Append(a_nMsgCode, str.c_str(), str.size());
	if (result != CBufferSend::eOK)
	{
		LOG(WARNING) << "over pack";
		return false;
	}

	if (bufferevent_write(m_pEvent, m_pSendBuffer->GetBuffer(), m_pSendBuffer->GetCurrentSize()) == 0)
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

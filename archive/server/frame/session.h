#pragma once

/*
* 保存socket
* 负责收发数据，如果收到一个完整的数据，回调上层
*/

#include <functional>
#include <string>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include "macro.h"
#include "google/protobuf/message.h"
#include "google/protobuf/message_lite.h"

class IServer;
class IServerManager;
class CBufferRecv;
class CBufferSend;

class CSession
{
public:
	CSession(IServer *a_pServer, evutil_socket_t a_Socket);
	virtual ~CSession();

	void Close();

	void OnReadCB(bufferevent *a_pBev, void *a_pArg);
	void OnWriteCB(bufferevent *a_pBev, void *a_pArg);
	void OnErrorCB(short a_nEvent);

	void SetReadTimeout(int a_nSec);
	void SetWriteTimeout(int a_nSec);

	void Send(int a_nMsgCode, ::google::protobuf::Message &a_Msg);
	void Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg);

private:
	void initSocket();
	void initBuffer();
	void reinitBuffer();
	void closeBuffer();
	bool send(int a_nMsgCode, std::string& str);

	bufferevent* m_pEvent = nullptr;

	DEFINE_TYPE_BASE(CBufferRecv*,	m_pReadBuffer, nullptr, GetReadBuffer, SetReadBuffer);
	DEFINE_TYPE_BASE(CBufferSend*,	m_pSendBuffer, nullptr, GetSendBuffer, SetSendBuffer);
	DEFINE_TYPE_BASE(int,		m_nPort, 1024, GetPort, SetPort);
	DEFINE_TYPE_REFER(std::string,	m_strServerIP, "", GetServerIP, SetServerIP);
	DEFINE_TYPE_BASE(evutil_socket_t,	m_Socket, 0, GetSocket, SetSocket);

	DEFINE_TYPE_BASE(IServer*, m_Server, nullptr, GetServer, SetServer);

	char* m_pRecvBuffer;
	static const unsigned int m_skReadBufferSize = 2000;
	static const unsigned int m_skBufferSize = 10000;	// 10KB
};

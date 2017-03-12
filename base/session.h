#ifndef _BASE_SESSION_H_
#define _BASE_SESSION_H_

#include <functional>
#include <string>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include "macro.h"

class CBuffer;

class CSession
{
//public:
//	enum ERead{
//
//	};
//	enum EWrite{
//
//	};
//	enum EERROR{
//
//	};
public:
	CSession(event_base* a_pEventBase);
	~CSession();

	void InitSock();
	void Connect();
	void CloseSocket();
	void ReConnect();

	void OnReadCB(bufferevent *a_pBev, void *a_pArg);
	void OnWriteCB(bufferevent *a_pBev, void *a_pArg);
	void OnErrorCB(short a_nEvent);

	void Send(char* a_pBuffer, int a_nBufferSize);

public:
	void SetSocket(SOCKET a_Socket);

private:
	event_base* m_pEventBase = nullptr;
	bufferevent* m_pBufferEvent = nullptr;

	DEFINE_TYPE_BASE(std::function< void(void*)>, funcReadCB, nullptr, GetReadCB, SetReadCB);
	DEFINE_TYPE_BASE(std::function< void(void*)>, funcWriteCB, nullptr, GetWriteCB, SetWriteCB);
	DEFINE_TYPE_BASE(std::function< void(void*)>, funcErrorCB, nullptr, GetErrorCB, SetErrorCB);

	DEFINE_BOOL(AutoConnect, false, GetAutoConnect, SetAutoConnect);
	DEFINE_TYPE_BASE(SOCKET, Socket, 0, GetSocket, ZZZSetSocket);
	DEFINE_PTR_BASE(CBuffer*, ReadBuffer, nullptr, GetReadBuffer, SetReadBuffer);
	DEFINE_PTR_BASE(CBuffer*, SendBuffer, nullptr, GetSendBuffer, SetSendBuffer);
	DEFINE_INT(Port, 1024, GetPort, SetPort);
	DEFINE_STRING(ServerName, "", GetServerName, SetServerName);
	DEFINE_STRING(ServerIP, "", GetServerIP, SetServerIP);
};
#endif

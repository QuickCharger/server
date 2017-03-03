#ifndef _BASE_SESSION_H_
#define _BASE_SESSION_H_

#include <string>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include "macro.h"

class CBuffer;

class CSession
{
public:
	CSession(event_base* a_pEventBase);
	~CSession();

	void Connect();
	void CloseSocket();
	static void ReConnect(int a_nClientFD, short a_nEvent, void *a_pArg);
	void OnReadCB(bufferevent *a_pBev, void *a_pArg);
	void OnWriteCB(bufferevent *a_pBev, void *a_pArg);
	void OnErrorCB(bufferevent *a_pBen, short a_nEvent, void *a_pArg);

private:
	DEFINE_TYPE_BASE(SOCKET, Socket, 0);
	DEFINE_BOOL(AutoConnect, false);
	DEFINE_PTR_BASE(CBuffer*, Buffer, nullptr);
	DEFINE_PTR_BASE(event_base*, EventBase, nullptr);
	DEFINE_PTR_BASE(char*, ReadBuffer, nullptr);
	DEFINE_INT(ReadBufferSize, 1024);
	DEFINE_INT(Port, 0);
	DEFINE_STRING(ServerName, "");
	DEFINE_STRING(ServerIP, "");

};
#endif

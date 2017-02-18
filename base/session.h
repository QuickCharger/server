#ifndef _BASE_SESSION_H_
#define _BASE_SESSION_H_

#include <string>
#include <event2/bufferevent.h>
#include <event2/event.h>

class CSession
{
public:
	CSession(event_base* a_pEventBase);
	~CSession();

	void SetServerName(const std::string a_strName)	{ m_strName = a_strName; };
	void SetServerIP(const std::string a_strIP)		{ m_strServerIP = a_strIP; };
	void SetPort(int a_nPort)						{ m_nPort = a_nPort; };
	void Connect(bool a_bAutoConnect);
	static void OnReadCB(bufferevent *a_pBev, void *a_pArg);
	static void OnWriteCB(bufferevent *a_pBev, void *a_pArg);
	static void OnErrorCB(bufferevent *a_pBen, short a_nEvent, void *a_pArg);

private:
	event_base *m_EventBase;
	SOCKET m_Socket;
	std::string m_strName;
	std::string m_strServerIP;
	int m_nPort;
	bool m_bAutoConnect;
};

#endif

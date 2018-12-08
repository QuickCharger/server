#pragma once

#include <functional>
#include <string>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include "macro.h"
#include "google/protobuf/message.h"
#include "google/protobuf/message_lite.h"

class IServer;
class IServerManager;
class CBuffer;

class CSession
{
public:
	CSession(IServer *a_pServer, event_base* a_pEventBase, SOCKET a_Socket);
	CSession(IServer *a_pServer, event_base *a_pEventBase, const std::string& a_strName, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect = false);
	virtual ~CSession();

	//void Connect();
	void CloseSocket();
	//void ReConnect();
	void DoConnect();

	void OnReadCB(bufferevent *a_pBev, void *a_pArg);
	void OnWriteCB(bufferevent *a_pBev, void *a_pArg);
	void OnErrorCB(short a_nEvent);

	//void Send(const char* a_pBuffer, int a_nBufferSize);
	//void Send(const std::string& a_strSrc);
	//void Send(int a_nMsgCode, const google::protobuf::MessageLite& a_Msg);
	void Send(int a_nMsgCode, ::google::protobuf::Message &a_Msg);
	void Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg);

private:
	void addConnectTimer();
	void initSocket();

	event_base* m_pEventBase = nullptr;
	bufferevent* m_pBufferEvent = nullptr;

	//DEFINE_TYPE_BASE(std::function< void(void*)>, m_funcReadCB, nullptr, GetReadCB, SetReadCB);
	//DEFINE_TYPE_BASE(std::function< void(void*)>, m_funcWriteCB, nullptr, GetWriteCB, SetWriteCB);
	//DEFINE_TYPE_BASE(std::function< void(void*)>, m_funcErrorCB, nullptr, GetErrorCB, SetErrorCB);

	/*
	* 发送数据之前的封装回调
	*/
//	DEFINE_TYPE_BASE(std::function< void(const char*, int&, char**)>, m_funcPackCB, nullptr, GetPackCB, SetPackCB);
	/*
	* 收到数据后的回调
	*/
//	DEFINE_TYPE_BASE(std::function< bool(const char*, int)>, m_funcUnPackCB, nullptr, GetUnPackCB, SetUnPackCB);
	///*
	//* connect之后的回调
	//*/
	//DEFINE_TYPE_BASE(std::function< void(void)>, m_funcConnectCB, nullptr, GetConnectCB, SetConnectCB);

	DEFINE_TYPE_BASE(bool,		m_bAutoConnect, false, GetAutoConnect, SetAutoConnect);
	DEFINE_TYPE_BASE(SOCKET,	m_Socket, 0, GetSocket, SetSocket);
	DEFINE_TYPE_BASE(CBuffer*,	m_pReadBuffer, nullptr, GetReadBuffer, SetReadBuffer);
	DEFINE_TYPE_BASE(CBuffer*,	m_pSendBuffer, nullptr, GetSendBuffer, SetSendBuffer);
	DEFINE_TYPE_BASE(int,		m_nPort, 1024, GetPort, SetPort);
	DEFINE_TYPE_REFER(std::string,	m_strServerName,	"", GetServerName, SetServerName);
	DEFINE_TYPE_REFER(std::string,	m_strServerIP,		"", GetServerIP, SetServerIP);

	DEFINE_TYPE_BASE(IServer*, m_Server, nullptr, GetServer, SetServer);
	//DEFINE_TYPE_BASE(IServerManager*, m_ServerManager, nullptr, GetServerManager, SetServerManager);
	//DEFINE_TYPE_BASE(ISessionSelector*, m_SessionSelector, nullptr, GetSessionSelector, SetSessionSelector);
};

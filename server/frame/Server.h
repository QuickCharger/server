#pragma once

//#include "netbase.h"
#include "google/protobuf/message.h"
#include "macro.h"
#include "IServer.h"
#include "base.h"

#include "IServerImpl.h"

class CServer : public IServer, public CErrRecord
{
public:
	CServer(IServerImpl *a_pServerImpl, evutil_socket_t a_Socket);
	CServer(IServerImpl *a_pServerImpl, const std::string& a_strName, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect = false);
	virtual ~CServer();

	virtual void OnReadCB(int a_nCode, const std::string& msg);
	virtual void OnWriteCB(void* a_pArg);
	virtual void OnErrorCB(void* a_pArg);
	/*
	* 成功连接其他服务器之后的回调
	*/
	virtual bool OnConnect(CSession* a_pSession);
	/*
	*
	*/
	bool OnConnected(const std::string& a_msg);

	void Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg);

	void Address(std::string& a_strIP, int& a_nPort);
	std::string Desc();

private:
	DEFINE_TYPE_BASE(CSession*, m_pSession, nullptr, GetSession, SetSession);
	DEFINE_TYPE_BASE(int, m_nPort, 1024, GetPort, SetPort);
	DEFINE_TYPE_REFER(std::string, m_strServerName, "", GetServerName, SetServerName);
	DEFINE_TYPE_REFER(std::string, m_strServerIP, "", GetServerIP, SetServerIP);

	IServerImpl *m_ServerImpl = nullptr;
	//DEFINE_TYPE_BASE(std::function< void(int, void*)>, m_funcMessageCB, nullptr, GetMessageCB, SetMessageCB);

	typedef void(IServerImpl::*Func)(CServer* m_pServer, int, const std::string&);
	Func m_funcOnMessageCB = nullptr;
};

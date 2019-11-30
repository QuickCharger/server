#pragma once

/*
* 保存session, 链接信息
* 重连机制
* 服务器的认证机制
* 收到session的回调后继续回调上层
* 被动链接的服务器设置超时读， 超时后主动关闭此链接

*/

//#include "netbase.h"
#include "google/protobuf/message.h"
#include "macro.h"
#include "utility.hpp"
#include "IServer.h"
#include "base.h"

#include "IServerImpl.h"
#include "timer.h"

class CServer : public IServer, public CErrRecord ,public CTimer<CServer>
{
public:
	CServer(IServerImpl *a_pServerImpl, evutil_socket_t a_Socket);
	CServer(IServerImpl *a_pServerImpl, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect = false);
	virtual ~CServer();

	void Connect();

	virtual void OnReadCB(int a_nCode, const std::string& msg);
	virtual void OnWriteCB(void* a_pArg);
	virtual void OnErrorCB(void* a_pArg);
	/*
	* 主动连接其他服务器 和 被动连接服务器的行为处理
	*/
	bool OnConnect();
	bool OnConnected(const std::string& a_msg);


	void Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg);

	void Address(std::string& a_strIP, int& a_nPort);
	std::string Desc();

	Property<std::string> m_strServerName;
	Property<std::string> m_strServerIP;
private:
	void addConnectTimer();
	void doConnect(void *a_pArg);
	void sendHeartBeat(void* a_pArg);

	DEFINE_TYPE_BASE(CSession*, m_pSession, nullptr, GetSession, SetSession);

	//DEFINE_TYPE_REFER(std::string, m_strServerName, "", GetServerName, SetServerName);
	//DEFINE_TYPE_REFER(std::string, m_strServerIP, "", GetServerIP, SetServerIP);
	DEFINE_TYPE_BASE(int, m_nPort, 0, GetSocket, SetSocket);
	DEFINE_TYPE_BASE(bool, m_bAutoConnect, false, GetAutoConnect, SetAutoConnect);
	DEFINE_TYPE_BASE(int, m_nAutoConnectTime, 2, GetAutoConnectTime, SetAutoConnectTime);

	IServerImpl *m_ServerImpl = nullptr;

	typedef void(IServerImpl::*Func)(CServer* m_pServer, int, const std::string&);
	Func m_funcOnMessageCB = nullptr;
};

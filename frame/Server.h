#ifndef _FRAME_SERVE_H_
#define _FRAME_SERVE_H_

//#include "netbase.h"
#include <functional>
#include "macro.h"
#include "IServer.h"
#include "ErrRecord.h"

#include "IServerImpl.h"

class CServer : public IServer, public CErrRecord<CServer>
{
public:
	CServer(IServerImpl *a_pServerImpl, event_base* a_pEventBase, SOCKET a_Socket = 0);
	CServer(IServerImpl *a_pServerImpl, event_base *a_pEventBase, const std::string& a_strName, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect = false);
	virtual ~CServer();

	//virtual void OnReadCB(const std::string& a_str);	//unfinish,a_pArg可能会有\0，导致转成string被截断。
	virtual void OnReadCB(CSession* a_pSession, int a_nCode, void* a_pArg);
	virtual void OnWriteCB(void* a_pArg);
	virtual void OnErrorCB(void* a_pArg);

	virtual void OnPackCB(const char *a_pSource, int a_nCode, int& a_nLength, char **a_pDest);
	virtual int OnUnPackCB(const char *a_pSource, int a_nLength, int &a_nCode, char **a_pDest);
	/*
	* 成功连接其他服务器之后的回调
	*/
	virtual bool OnConnect(CSession* a_pSession);

private:
	//DEFINE_TYPE_BASE(CSession*, m_pSession, nullptr, GetSession, SetSession);
	DEFINE_TYPE_BASE(int, m_nPort, 1024, GetPort, SetPort);
	DEFINE_TYPE_REFER(std::string, m_strServerName, "", GetServerName, SetServerName);
	DEFINE_TYPE_REFER(std::string, m_strServerIP, "", GetServerIP, SetServerIP);

	IServerImpl *m_ServerImpl = nullptr;
	//DEFINE_TYPE_BASE(std::function< void(int, void*)>, m_funcMessageCB, nullptr, GetMessageCB, SetMessageCB);
};

#endif

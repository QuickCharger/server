#pragma once

#include "CErrRecord.h"
#include "log.h"
#include "macro.h"
//#include "minidump.h"
#include "netbase.h"

#include "Config.h"
#include "Server.h"
#include "session.h"
#include "timer.h"

#include "IServerImpl.h"
#include "MessageDispatch.h"

enum EServerType
{
	eServerTypeMin,
	eDustbin,
	eDatabaseServer,
	eServerTypeMax
};

template<typename T>
class CServerImpl : public IServerImpl, public CMessageDispatch, public CErrRecord, public CTimer<T>
{
public:
	CServerImpl(char *argv[])
	{
		//InitMinDump();
		InitNet();
		InitLog(argv);
		initServerType();

		m_pConfig = CConfig::GetInstance();
		if (!m_pConfig->InitConfig(argv[0]))
		{
			//SetErr(m_pConfig->GetErr());
			LOG(ERROR) << m_pConfig->GetErr();
			exit(1);
		}
		if (!m_pConfig->GetValue("port", m_nPort))
		{
			SetErr(m_pConfig->GetErr());
			exit(1);
		}

		std::map<std::string, std::pair<std::string, int>> &mServer = m_pConfig->GetServer();

		m_Socket = socket(AF_INET, SOCK_STREAM, 0);
		evutil_make_socket_nonblocking(m_Socket);
#ifndef WIN32
		evutil_make_listen_socket_reuseable(sockSrv);	//设定接口可重用,Win上不可用
#endif

		SOCKADDR_IN addrSrv;
		std::memset(&addrSrv, 0, sizeof(SOCKADDR_IN));
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		addrSrv.sin_port = htons(m_nPort);

		if (bind(m_Socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == -1)
		{
			//perror("bind error");
			LOG(ERROR) << "bind error";
			exit(1);
		}

		if (listen(m_Socket, 10) == -1)
		{
			LOG(ERROR) << "listen failed";
			exit(1);
		}

		m_pEventBase = event_base_new();
		event *evListen = event_new(m_pEventBase, m_Socket, EV_READ | EV_PERSIST,
			[](evutil_socket_t a_nClientFD, short a_nEvent, void *a_pArg){
			CServerImpl *pServerImpl = static_cast<CServerImpl*>(a_pArg);
			pServerImpl->OnAccept(a_nClientFD, a_nEvent, a_pArg);
		}, this);
		event_add(evListen, nullptr);
		// EV_CLOSED 没有处理	unfinish

		// 1秒之后连接配置文件内的服务器
		event *evListen2 = event_new(m_pEventBase, -1, 0,
			[](evutil_socket_t a_Socket, short a_nEvent, void *a_pArg){
			CServerImpl *pServerImpl = static_cast<CServerImpl*>(a_pArg);
			pServerImpl->LinkToServer(a_Socket, a_nEvent, a_pArg);
		}, this);
		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		event_add(evListen2, &tv);

		//init timer
		InitTimer(m_pEventBase);

		//AddTimer(1, 1, [this](void* a_pArg){}, nullptr, 1);
	}
	void Run()
	{
		event_base_dispatch(m_pEventBase);
	};
	virtual ~CServerImpl()
	{
		LOG(INFO) << "PROGRAM FINISH\n" << "press any key to exit";
		getchar();

		if (m_pEventBase != nullptr)
		{
			m_pEventBase = nullptr;
		}

		CloseNet();
	};

	void OnAccept(int a_nClientFD, short a_nEvent, void *a_pArg)
	{
		SOCKADDR_IN addrIn;
		int len = sizeof(SOCKADDR);
		SOCKET socket = accept(a_nClientFD, (SOCKADDR*)&addrIn, &len);
		if (socket > 0)
		{
			LOG(INFO) << "New Connect Accept Success. sockID: " << socket;
		}
		else
		{
			LOG(WARNING) << "New Connect Accept Failed!!!";
			return;
		}
		// 有新的服务器连接到本服务器
		CServer *pNewServer = new CServer(this, m_pEventBase, socket);
	}

	void LinkToServer(evutil_socket_t a_Socket, short a_nEvent, void *a_pArg)
	{
		std::map<std::string, std::pair<std::string, int>> &serverCfg = m_pConfig->GetServer();
		for (auto it = serverCfg.begin(); it != serverCfg.end(); ++it)
		{
			//主动连接其他服务器
			CServer *pServer = new CServer(this, m_pEventBase, it->first, it->second.first, it->second.second, true);
		}
	}

	virtual void OnMessageCB(CServer* a_pServer, int a_nCode, const char *a_pCh)
	{
		if (!DoMessageCB(a_pServer, a_nCode, a_pCh))
		{
			LOG(WARNING) << "code: " << a_nCode << " dispatch failed !!!";
		}
	};

	virtual bool AcceptServer(CServer *a_pServer)
	{
		const std::string strServerName = a_pServer->GetServerName();
		for (auto it = m_ServerType.begin(); it != m_ServerType.end(); ++it)
		{
			if (it->second == strServerName)
			{
				m_Server[it->first].insert(a_pServer);
				return true;
			}
		}
		return false;
	}

private:
	void initServerType()
	{
		m_ServerType[EServerType::eDustbin] = "dustbinserver";
		m_ServerType[EServerType::eDatabaseServer] = "databaseserver";
	}

	void brocast(EServerType a_eServerType, ::google::protobuf::Message *a_pMsg)
	{

	}

private:
	CConfig *m_pConfig;
	DEFINE_TYPE_BASE(int, m_nPort, 0, GetPort, SetPort);
	evutil_socket_t m_Socket;
	event_base* m_pEventBase;

	std::map<EServerType, const char*> m_ServerType;
	std::map<EServerType, std::set<CServer*>> m_Server;
};

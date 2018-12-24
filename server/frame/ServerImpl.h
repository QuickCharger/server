#pragma once

#include "CErrRecord.h"
#include "macro.h"

#include <signal.h>

#include "Config.h"
#include "Libevent.h"
#include "net.h"
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
		InitNet();
		InitLog(argv);
		//InitTimer(CLibevent::GetInstance());
		CLibevent::GetInstance();
		initServerType();
		initConfig(argv[0]);
		initSignal();

		//		m_Socket = socket(AF_INET, SOCK_STREAM, 0);
		//evutil_make_socket_nonblocking(m_Socket);
		//		evutil_make_listen_socket_reuseable(m_Socket);	//设定接口可重用,Win上不可用

		startAccepting();

		AddTimer(1, this, &CServerImpl::LinkToServer, nullptr, 1);
	}
	void Run()
	{
		CLibevent::Run();
	};

	virtual ~CServerImpl()
	{
		LOG(INFO) << "PROGRAM FINISH";
		CloseLog();
		CloseNet();
		evconnlistener_free(m_pListener);
		CLibevent::Close();
	};

	//void OnAccept(int a_nClientFD, short a_nEvent, void *a_pArg)
	//{
	//	SOCKADDR_IN addrIn;
	//	int len = sizeof(SOCKADDR);
	//	SOCKET socket = accept(a_nClientFD, (SOCKADDR*)&addrIn, &len);
	//	if (socket > 0)
	//	{
	//		LOG(INFO) << "New Connect Accept Success. sockID: " << socket;
	//	}
	//	else
	//	{
	//		LOG(WARNING) << "New Connect Accept Failed!!!";
	//		return;
	//	}
	//	// 有新的服务器连接到本服务器
	//	CServer *pNewServer = new CServer(this, m_pEventBase, socket);
	//}

	void LinkToServer(void* a_pArg)
	{
		std::vector<std::pair<std::string, int>> serverCfg = CConfig::GetInstance()->GetServerList();
		for (auto it = serverCfg.begin(); it != serverCfg.end(); ++it)
		{
			//主动连接其他服务器
			CServer *pServer = new CServer(this, it->first, it->second, true);
			pServer->Connect();
		}

		//// test
		//static int counter = 1;
		//LOG(INFO) << "counter " << counter;
		//counter++;
	}

	virtual void OnMessageCB(CServer* a_pServer, int a_nCode, const char *a_pCh)
	{
		if (!DoMessageCB(a_pServer, a_nCode, a_pCh))
		{
			LOG(WARNING) << "code: " << a_nCode << " dispatch failed !!!";
		}
	};

	virtual bool AcceptClientCB(CServer *a_pServer)
	{
		return true;
	}

	virtual bool AcceptServerCB(CServer *a_pServer)
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

	virtual void LostServer(CServer* a_pServer)
	{

	}

private:
	void initServerType()
	{
		m_ServerType[EServerType::eDustbin] = "dustbinserver";
		m_ServerType[EServerType::eDatabaseServer] = "databaseserver";
	}

	void initConfig(char *pCh)
	{
		CConfig* config = CConfig::GetInstance();
		if (!config->InitConfig(pCh))
		{
			LOG(ERROR) << config->GetErr();
			exit(1);
		}
		config->GetValue("Port", m_nPort);
	}

	void initSignal()
	{
		if (!CLibevent::CaughtSignal(SIGINT, signal_cb, this))
		{
			LOG(WARNING) << "Could not create/add a signal event!";
			CLibevent::Close();
		}
	}

	void startAccepting()
	{
		if (m_nPort == 0)
			return;

		DLOG(INFO) << "open port " << m_nPort << " start accepting";

		SOCKADDR_IN sin;
		std::memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		sin.sin_port = htons(m_nPort);

		// listen() 第二个参数是链接处于ESTABLISH的最大数量 此处设置128个
		m_pListener = evconnlistener_new_bind(CLibevent::GetInstance(), listener_cb,
			this, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 128, (struct sockaddr*)&sin, sizeof(sin));
		if (!m_pListener)
		{
			LOG(ERROR) << "evconnlistener_new_bind error";
			exit(1);
		}
	}

	static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *sa, int socklen, void *param)
	{
		CServerImpl *pServerImpl = static_cast<CServerImpl*>(param);
		CServer *pNewServer = new CServer(pServerImpl, fd);

		DLOG(INFO) << "new socket income. fd " << fd << ". Desc: " << pNewServer->Desc().c_str();
	}

	static void signal_cb(evutil_socket_t fd, short sig, void * arg)
	{
		LOG(INFO) << "Caught an interrupt signal";
		CLibevent::Close();
	}

	void brocast(EServerType a_eServerType, ::google::protobuf::Message *a_pMsg)
	{

	}

private:
	int m_nPort = 0;
	evutil_socket_t m_Socket;
	evconnlistener* m_pListener = nullptr;
	event* signal_event = nullptr;

	std::map<EServerType, const char*> m_ServerType;
	std::map<EServerType, std::set<CServer*>> m_Server;
};

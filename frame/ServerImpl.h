#ifndef _FRAME_SERVERIMPL_H_
#define _FRAME_SERVERIMPL_H_

#include "ErrRecord.h"
#include "log.h"
#include "macro.h"
//#include "minidump.h"
#include "netbase.h"

#include "Config.h"
#include "Server.h"
#include "session.h"

#include "IServerImpl.h"
#include "MessageDispatch.h"

template<typename T>
class CServerImpl : public IServerImpl, public CMessageDispatch, public CErrRecord<T>
{
public:
	CServerImpl(char *argv[])
	{
		//InitMinDump();
		InitNet();
		InitLog(argv);

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
			perror("bind error");
			return;
		}

		if (listen(m_Socket, 10) == -1)
		{
			perror("listen error");
			return;
		}

		m_pEventBase = event_base_new();
		event *evListen = event_new(m_pEventBase, m_Socket, EV_READ | EV_PERSIST,
			[](evutil_socket_t a_nClientFD, short a_nEvent, void *a_pArg){
			CServerImpl *pServerImpl = static_cast<CServerImpl*>(a_pArg);
			pServerImpl->OnAccept(a_nClientFD, a_nEvent, a_pArg);
		}, this);
		event_add(evListen, nullptr);

		//新线程连接合理
		event *evListen2 = event_new(m_pEventBase, -1, 0,
			[](evutil_socket_t a_Socket, short a_nEvent, void *a_pArg){
			CServerImpl *pServerImpl = static_cast<CServerImpl*>(a_pArg);
			pServerImpl->LinkToServer(a_Socket, a_nEvent, a_pArg);
		}, this);
		timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		event_add(evListen2, &tv);
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

		CServer *pNewServer = new CServer(this, m_pEventBase, socket);
		//pNewServer->SetMessageCB([this](int a_nCode, void *a_Arg){ 
		//	this->OnMessageCB(a_nCode, a_Arg);
		//});
		//pNewServer->SetSocket(socket);
	}

	//virtual void OnMessageCB(int, void*) = 0;

	//void OnMessageCB(int a_nCode, void*)
	//{
	//	LOG(INFO) << "Template OnMessageCB";
	//}

	void LinkToServer(evutil_socket_t a_Socket, short a_nEvent, void *a_pArg)
	{
		//std::map<std::string, std::pair<std::string, int>> *serverCfg = m_pConfig->GetServer();
		std::map<std::string, std::pair<std::string, int>> &serverCfg = m_pConfig->GetServer();
		for (auto it = serverCfg.begin(); it != serverCfg.end(); ++it)
		{
			CServer *pServer = new CServer(this, m_pEventBase, it->first, it->second.first, it->second.second, true);
			//pServer->SetMessageCB([this](int a_nCode, void *a_Arg){ this->OnMessageCB(a_nCode, a_Arg); });
		}
	}

	virtual void OnMessageCB(CSession* a_pSession, int a_nCode, const char *a_pCh)
	{
		if (!DoMessageCB(a_pSession, a_nCode, a_pCh))
		{
			LOG(WARNING) << "code: " << a_nCode << " dispatch failed !!!";
		}
	};

private:
	CConfig *m_pConfig;
	DEFINE_TYPE_BASE(int, m_nPort, 0, GetPort, SetPort);
	evutil_socket_t m_Socket;
	//DEFINE_TYPE_REFER(event_base*, m_pEventBase, nullptr, GetEventBase, SetEventBase);
	event_base* m_pEventBase;
};

#endif

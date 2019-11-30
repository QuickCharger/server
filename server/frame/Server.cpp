#include "Server.h"
#include "config.h"
#include "Libevent.h"
#include "log.h"
#include "session.h"
#include "NetProtocol/common/test.pb.h"
#include "NetProtocol/common/messageCode.h"
#include "NetProtocol/server/messageCode.h"

/*
* 被动链接
* 直接创建session
* 设定读触发, 规定时间到后还没数据执行CSession::OnErrorCB
*/
CServer::CServer(IServerImpl *a_pServerImpl, evutil_socket_t a_Socket)
	//: IServer(/*a_pEventBase, new CSession(a_pEventBase)*/)
	: m_ServerImpl(a_pServerImpl)
{
	m_pSession = new CSession(this, a_Socket);
	LOG(INFO) << "Accept new socket. Desc " << Desc();
	m_pSession->SetReadTimeout(30);
}

// 主动链接， 依靠Connect()
CServer::CServer(IServerImpl *a_pServerImpl, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect)
	//: IServer()
	: m_ServerImpl(a_pServerImpl)
	, m_strServerIP(a_strIP)
	, m_nPort(a_nPort)
	, m_bAutoConnect(a_bAutoConnect)
{
}

CServer::~CServer()
{
	LOG(INFO) << "CServer::~CServer";
}

void CServer::Connect()
{
	addConnectTimer();
}

void CServer::OnReadCB(int a_nCode, const std::string& msg)
{
	DLOG(INFO) << "get msg, code " << a_nCode;

	if (a_nCode == ServerMessageCode::eRegistServer)
	{
		if (OnConnected(msg))
		{
			//if (certification.type() == Certification::eServer)
			//{
			//	LOG(INFO) << "New Server come in";
			//}
			//else if (certification.type() == Certification::eClient)
			//{
			//	LOG(INFO) << "New Client come in";
			//}
			//else
			//{
			//	LOG(WARNING) << "certification err.";
			//}
		}
	}
	else if (m_funcOnMessageCB != nullptr)
	{
		(m_ServerImpl->*m_funcOnMessageCB)(this, a_nCode, msg);
	}
	else
	{
		LOG(WARNING) << "Server do MSG before unCertificate";
	}
}

void CServer::OnWriteCB(void* a_pArg)
{
	LOG(INFO) << "Client. OnWriteCB";
}

/*
* 写错误是session回调， session自行执行销毁
*/
void CServer::OnErrorCB(void* a_pArg)
{
	DLOG(INFO) << "Client. OnErrorCB";
	m_ServerImpl->LostServer(this);
	m_pSession = nullptr;
	if (m_bAutoConnect)
	{
		addConnectTimer();
	}
	else
	{
		delete this;
	}
}

/*
* 主动连接
* 按时发心跳
* 发送本服务的配置给连接上的服务器
*/
bool CServer::OnConnect()
{
	LOG(INFO) << "Link to " << m_strServerIP().c_str() << ":" << m_nPort << " successful. Desc " << Desc();
	AddTimer(10, this, &CServer::sendHeartBeat, nullptr, -1);
	const CConfig *pConfig = CConfig::GetInstance();
	std::string strServerName;
	std::string strCode;
	if (pConfig->GetValue("ServerName", strServerName) && pConfig->GetValue("SecretKey", strCode))
	{
		Certification msgCertification;
		msgCertification.set_type(Certification_TYPE_eServer);
		msgCertification.set_name(strServerName);
		msgCertification.set_code(strCode);
		m_pSession->Send(ServerMessageCode::eRegistServer, msgCertification);

		return true;
	}
	SetErr(pConfig->GetErr());
	return false;
}

/*
* 接收其他服务器发来的消息并认证
* return true 包解析正确 并且 key正确
* return false 包解析失败 或 key不正确
*/
bool CServer::OnConnected(const std::string& a_msg)
{
	Certification certification;
	if (!certification.ParseFromString(a_msg))
	{
		LOG(WARNING) << "CServer::OnConnected. Certification parse failed !!!";
		return false;
	}
	if (certification.type() == Certification::eServer)
	{
		std::string strCode;
		if (CConfig::GetInstance()->GetValue("SecretKey", strCode) && strCode == certification.code())
		{
			m_strServerName = certification.name();
			if (!m_ServerImpl->AcceptServerCB(this))
			{
				// TODO
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	else if (certification.type() == Certification::eClient)
	{
		m_ServerImpl->AcceptClientCB(this);
		// todo
	}
	LOG(WARNING) << "CServer::OnConnected. Certification type err.";
	return true;
}

void CServer::Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg)
{
	m_pSession->Send(a_nMsgCode, *a_pMsg);
}

void CServer::Address(std::string& a_strIP, int& a_nPort)
{
	a_strIP = "";
	a_nPort = 0;

	if (!m_pSession)
		return;

	const evutil_socket_t &socket = m_pSession->GetSocket();
	struct sockaddr_in sa;
	int len = sizeof(sa);
	if (!getpeername(socket, (struct sockaddr *)&sa, &len))
	{
		a_strIP = inet_ntoa(sa.sin_addr);
		a_nPort = ntohs(sa.sin_port);
	}
}

std::string CServer::Desc()
{
	std::string ip;
	int port = 0;
	Address(ip, port);

	std::stringstream ss;
	ss << "ip: " << ip << ":" << port;
	return ss.str();
}

void CServer::addConnectTimer()
{
	AddTimer(m_nAutoConnectTime, this, &CServer::doConnect, nullptr, 1);
}

void CServer::doConnect(void *a_pArg)
{
	assert(m_pSession == 0);
	evutil_socket_t fd = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(m_strServerIP().c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(m_nPort);
	//bufferevent_socket_connect();		todo
	//DLOG(INFO) << "before connect";
	if (connect(fd, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == 0)
	{
		DLOG(INFO) << "ConnectServer " << m_strServerIP() << ":" << m_nPort << " success. Socket: " << fd;
		//m_funcConnectCB();
		m_pSession = new CSession(this, fd);
		OnConnect();
	}
	else
	{
		DLOG(INFO) << "ConnectServer " << m_strServerIP() << ":" << m_nPort << " failed. socket close. fd: " << fd;
		EVUTIL_CLOSESOCKET(fd);
		if (m_bAutoConnect)
		{
			addConnectTimer();
		}
	}
	//DLOG(INFO) << "after connect";
}

void CServer::sendHeartBeat(void* a_pArg)
{
	if(m_pSession)
		m_pSession->Send(CommonMessageCode::eHeartBeat, nullptr);
}

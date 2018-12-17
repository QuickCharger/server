#include "Server.h"
#include "config.h"
#include "log.h"
#include "session.h"
#include "NetProtocol/common/test.pb.h"
#include "NetProtocol/server/messageCode.h"

CServer::CServer(IServerImpl *a_pServerImpl, evutil_socket_t a_Socket)
	//: IServer(/*a_pEventBase, new CSession(a_pEventBase)*/)
	: m_ServerImpl(a_pServerImpl)
{
	m_pSession = new CSession(this, a_Socket);
}

CServer::CServer(IServerImpl *a_pServerImpl, const std::string& a_strName, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect)
	//: IServer()
	: m_ServerImpl(a_pServerImpl)
{
	m_pSession = new CSession(this, a_strName, a_strIP, a_nPort, a_bAutoConnect);
}

CServer::~CServer()
{
	LOG(INFO) << "CServer::~CServer";
}

void CServer::OnReadCB(int a_nCode, const std::string& msg)
{
	//LOG(INFO) << "Client. OnReadCB";

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

void CServer::OnErrorCB(void* a_pArg)
{
	LOG(INFO) << "Client. OnErrorCB";
}

//void CServer::OnPackCB(const char *a_pSource, int a_nCode, int& a_nLength, char **a_pDest)
//{
//	*a_pDest = (char*)a_pSource;
//	int nLength = sizeof(int) + sizeof(a_nCode) + a_nLength;	//包长度 + 消息号 + 消息体
//	*a_pDest = new char[nLength];		//int + buf
//	memset(*a_pDest, 0, nLength);
//	memcpy(*a_pDest, (void*)&nLength, sizeof(int));								// 包长度
//	memcpy(*a_pDest + sizeof(int), (void*)&a_nCode, sizeof(a_nCode));			// 消息号
//	memcpy(*a_pDest + sizeof(int) + sizeof(a_nCode), a_pSource, a_nLength);		// 消息体
//	a_nLength = nLength;
//}
//
//int CServer::OnUnPackCB(const char *a_pSource, int a_nLength, int &a_nCode, char **a_pDest)
//{
//	if (a_nLength == 0)
//	{
//		return 0;
//	}
//	//int nPackLength = *(int*)a_pSource;
//	int nPackLength = 0;
//	nPackLength |= (a_pSource[0] << 24) & 0xFF000000;
//	nPackLength |= (a_pSource[1] << 16) & 0xFF0000;
//	nPackLength |= (a_pSource[2] << 8) & 0xFF00;
//	nPackLength |= (a_pSource[3] << 0) & 0xFF;
//
//	if (nPackLength <= a_nLength)
//	{
//		a_nCode = *(int*)(a_pSource + sizeof(int));
//		*a_pDest = new char[nPackLength - sizeof(int) - sizeof(a_nCode) + 1];	//结尾补0
//		memset(*a_pDest, 0, nPackLength - sizeof(int) - sizeof(a_nCode) + 1);
//		memcpy(*a_pDest, a_pSource + sizeof(int) + sizeof(a_nCode), nPackLength - sizeof(int) - sizeof(a_nCode));
//		return nPackLength;
//	}
//	return 0;
//}

//发送本服务的配置给连接上的服务器
bool CServer::OnConnect(CSession* a_pSession)
{
	//CConfig *pConfig = CConfig::GetInstance();
	const CConfig *pConfig = CConfig::GetInstance();
	std::string strServerName;
	std::string strCode;
	if (pConfig->GetValue("ServerName", strServerName) && pConfig->GetValue("SecretKey", strCode))
	{
		Certification msgCertification;
		msgCertification.set_type(Certification_TYPE_eServer);
		msgCertification.set_name(strServerName);
		msgCertification.set_code(strCode);
		a_pSession->Send(ServerMessageCode::eRegistServer, msgCertification);
		return true;
	}
	SetErr(pConfig->GetErr());
	return false;
}

//接收其他服务器发来的消息并认证
bool CServer::OnConnected(const std::string& a_msg)
{
	Certification certification;
	if (certification.ParseFromString(a_msg))
	{
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
		LOG(WARNING) << "CServer::OnConnected. Certification type err.";
	}
	else
	{
		LOG(WARNING) << "CServer::OnConnected. Certification parse failed !!!";
	}
	return false;
}

void CServer::Send(int a_nMsgCode, ::google::protobuf::Message *a_pMsg)
{
	m_pSession->Send(a_nMsgCode, *a_pMsg);
}

void CServer::Address(std::string& a_strIP, int& a_nPort)
{
	a_strIP = "";
	a_nPort = 0;

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

#include "Server.h"
#include "config.h"
#include "log.h"
#include "session.h"
#include "common/test.pb.h"
#include "MessageCode.h"
CServer::CServer(IServerImpl *a_pServerImpl, event_base* a_pEventBase, SOCKET a_Socket)
	: IServer(/*a_pEventBase, new CSession(a_pEventBase)*/)
	, m_ServerImpl(a_pServerImpl)
{
	m_pSession = new CSession(this, a_pEventBase, a_Socket);
}

CServer::CServer(IServerImpl *a_pServerImpl, event_base *a_pEventBase, const std::string& a_strName, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect)
	: IServer()
	, m_ServerImpl(a_pServerImpl)
{
	m_pSession = new CSession(this, a_pEventBase, a_strName, a_strIP, a_nPort, a_bAutoConnect);
}

CServer::~CServer()
{

}

//void CServer::OnReadCB(const std::string& a_str)
//{
//	LOG(INFO) << "Client. OnReadCB";
//
//	Certification certification;
//	if (certification.ParseFromString(a_str))
//	{
//		if (certification.type() == Certification::eServer)
//		{
//
//		}
//		else if (certification.type() == Certification::eClient)
//		{
//
//		}
//	}
//	else
//	{
//		LOG(ERROR) << "ParseFromString failed";
//	}
//}

void CServer::OnReadCB(int a_nCode, void* a_pArg)
{
	LOG(INFO) << "Client. OnReadCB";

	if (a_nCode == kCertification)
	{
		Certification certification;
		if (certification.ParseFromString((char*)a_pArg))
		{
			if (certification.type() == Certification::eServer)
			{

			}
			else if (certification.type() == Certification::eClient)
			{

			}
			else
			{
				LOG(WARNING) << "certification err.";
			}
		}
		else
		{
			LOG(WARNING) << "Parse err. code: " << a_nCode;
		}
		//m_funcMessageCB(a_nCode, a_pArg);
		m_ServerImpl->OnMessageCB(a_nCode, (char*)a_pArg);
		return;
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

void CServer::OnPackCB(const char *a_pSource, int a_nCode, int& a_nLength, char **a_pDest)
{
	*a_pDest = (char*)a_pSource;
	int nLength = sizeof(int) + sizeof(a_nCode) + a_nLength;	//包长度 + 消息号 + 消息体
	*a_pDest = new char[nLength];		//int + buf
	memset(*a_pDest, 0, nLength);
	memcpy(*a_pDest, (void*)&nLength, sizeof(int));
	memcpy(*a_pDest + sizeof(int), (void*)&a_nCode, sizeof(a_nCode));
	memcpy(*a_pDest + sizeof(int) + sizeof(a_nCode), a_pSource, a_nLength);
	a_nLength = nLength;
}

bool CServer::OnUnPackCB(const char *a_pSource, int a_nLength, int &a_nCode, char **a_pDest)
{
	int nPackLength = *(int*)a_pSource;
	if (nPackLength <= a_nLength)
	{
		a_nCode = *(int*)(a_pSource + sizeof(int));
		*a_pDest = new char[nPackLength - sizeof(int) - sizeof(a_nCode) + 1];	//结尾补0
		memset(*a_pDest, 0, nPackLength - sizeof(int) - sizeof(a_nCode) + 1);
		memcpy(*a_pDest, a_pSource + sizeof(int) + sizeof(a_nCode), a_nLength - sizeof(int) - sizeof(a_nCode));
		return true;
	}
	return false;
}

//void CServer::OnPackCB(int a_nCode, const std::string& a_strSrc, std::string& a_strDest)
//{
//	int nPackSize = sizeof(int) + sizeof(a_nCode) + a_strSrc.size();
//	char chPackSize[4] = { 0 };
//	chPackSize[0] = nPackSize / 256 / 256 / 256 % 256;
//	chPackSize[1] = nPackSize / 256 / 256 % 256;
//	chPackSize[2] = nPackSize / 256 % 256;
//	chPackSize[3] = nPackSize % 256;
//	a_strDest.assign(chPackSize, 4);
//	a_strDest += a_nCode;
//	a_strDest += a_strSrc;
//}
//
//bool CServer::OnUnPackCB(const std::string& a_strSrc, int &a_nCode, std::string &a_strDest)
//{
//	std::string strCode;
//	char chPackSize[5] = { 0 };
//	chPackSize[0] = a_strSrc[0];
//	chPackSize[1] = a_strSrc[1];
//	chPackSize[2] = a_strSrc[2];
//	chPackSize[3] = a_strSrc[3];
//	int nPackLength = atoi(chPackSize);
//	if (nPackLength <= static_cast<int>(a_strSrc.size()))
//	{
//		strCode.assign(a_strSrc, sizeof(int), sizeof(a_nCode));
//		a_nCode = atoi(strCode.c_str());
//		a_strDest.assign(a_strSrc, sizeof(int) + sizeof(a_nCode), a_strSrc.size() - sizeof(int) - sizeof(a_nCode));
//		return true;
//	}
//	return false;
//}

bool CServer::OnConnect(CSession* a_pSession)
{
	//发送本服务的配置给连接上的服务器
	CConfig *pConfig = CConfig::GetInstance();
	std::string strServerName;
	std::string strCode;
	if (pConfig->GetValue("ServerName", strServerName) && pConfig->GetValue("SecretKey", strCode))
	{
		Certification msgCertification;
		msgCertification.set_type(Certification_TYPE_eServer);
		msgCertification.set_name(strServerName);
		msgCertification.set_code(strCode);
		a_pSession->Send(kCertification, msgCertification);
		return true;
	}
	SetErr(pConfig->GetErr());
	return false;
}

#include "Server.h"
#include "config.h"
#include "log.h"
#include "session.h"
#include "common/test.pb.h"
#include "MessageCode.h"
CServer::CServer(event_base* a_pEventBase, SOCKET a_Socket)
	: IServer(/*a_pEventBase, new CSession(a_pEventBase)*/)
{
	m_pSession = new CSession(this, a_pEventBase, a_Socket);
}

CServer::CServer(event_base *a_pEventBase, const std::string& a_strName, const std::string& a_strIP, int a_nPort, bool a_bAutoConnect)
	: IServer()
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

void CServer::OnReadCB(int a_nCode, const std::string& a_str)
{
	LOG(INFO) << "Client. OnReadCB";

	if (a_nCode == kCertification)
	{
		Certification certification;
		if (certification.ParseFromString(a_str))
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

//void CServer::OnPackCB(const std::string& a_strSrc, std::string& a_strDest)
//{
//	a_strDest = a_strSrc.size() + sizeof(int);
//	a_strDest += a_strSrc;
//}

void CServer::OnPackCB(int a_nCode, const std::string& a_strSrc, std::string& a_strDest)
{
	a_strDest = sizeof(int) + sizeof(a_nCode) + a_strSrc.size();
	a_strDest += a_nCode;
	a_strDest += a_strSrc;
}

//bool CServer::OnUnPackCB(const std::string& a_strSrc, std::string& a_strDest)
//{
//	std::string strPackLength;
//	strPackLength.assign(a_strSrc, 0, sizeof(int));
//	int nPackLength = atoi(strPackLength.c_str());
//	if (nPackLength <= static_cast<int>(a_strSrc.size()))
//	{
//		a_strDest.assign(a_strSrc, sizeof(int), a_strSrc.size() - sizeof(int));
//		return true;
//	}
//	return false;
//}

bool CServer::OnUnPackCB(const std::string& a_strSrc, int &a_nCode, std::string &a_strDest)
{
	std::string strPackLength;
	std::string strCode;
	strPackLength.assign(a_strSrc, 0, sizeof(int));
	int nPackLength = atoi(strPackLength.c_str());
	if (nPackLength <= static_cast<int>(a_strSrc.size()))
	{
		strCode.assign(a_strSrc, sizeof(int), sizeof(a_nCode));
		a_nCode = atoi(strCode.c_str());
		a_strDest.assign(a_strSrc, sizeof(int) + sizeof(a_nCode), a_strSrc.size() - sizeof(int) - sizeof(a_nCode));
		return true;
	}
	return false;
}

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

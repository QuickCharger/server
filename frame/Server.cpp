#include "Server.h"
#include "config.h"
#include "log.h"
#include "session.h"
#include "common/test.pb.h"
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

void CServer::OnReadCB(const std::string& a_str)
{
	LOG(INFO) << "Client. OnReadCB";

	Certification certification;
	if (certification.ParseFromString(a_str))
	{
		if (certification.type() == Certification::eServer)
		{

		}
		else if (certification.type() == Certification::eClient)
		{

		}
	}
	else
	{
		LOG(ERROR) << "ParseFromString failed";
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

void CServer::OnPackCB(const std::string& a_strSrc, std::string& a_strDest)
{
	a_strDest = a_strSrc.size() + sizeof(int);
	a_strDest += a_strSrc;
}

bool CServer::OnUnPackCB(const std::string& a_strSrc, std::string& a_strDest)
{
	std::string strPackLength;
	strPackLength.assign(a_strSrc, 0, sizeof(int));
	int nPackLength = atoi(strPackLength.c_str());
	if (nPackLength <= static_cast<int>(a_strSrc.size()))
	{
		a_strDest.assign(a_strSrc, sizeof(int), a_strSrc.size() - sizeof(int));
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
		Certification certification;
		certification.set_type(Certification_TYPE_eServer);
		certification.set_name(strServerName);
		certification.set_code(strCode);
		std::string str;
		certification.SerializeToString(&str);
		a_pSession->Send(str);
		return true;
	}
	SetErr(pConfig->GetErr());
	return false;
}

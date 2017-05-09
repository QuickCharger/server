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

void CServer::OnReadCB(void* a_pArg)
{
	LOG(INFO) << "Client. OnReadCB";

	Certification certification;
	if (certification.ParseFromString((char*)a_pArg))
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

void CServer::OnPackCB(const char *a_pSource, int& a_nLength, char **a_pDest)
{
	*a_pDest = (char*)a_pSource;
	int nLength = a_nLength + sizeof(int);
	*a_pDest = new char[nLength];		//int + buf
	memset(*a_pDest, 0, nLength);
	memcpy(*a_pDest, (void*)&nLength, sizeof(int));
	memcpy(*a_pDest + sizeof(int), a_pSource, a_nLength);
	a_nLength = nLength;
}

bool CServer::OnUnPackCB(const char *a_pSource, int a_nLength, char **a_pDest)
{
	int nPackLength = *(int*)a_pSource;
	if (nPackLength <= a_nLength)
	{
		*a_pDest = new char[nPackLength - sizeof(int) + 1];
		memset(*a_pDest, 0, nPackLength - sizeof(int) + 1);
		memcpy(*a_pDest, a_pSource + sizeof(int), a_nLength - sizeof(int));
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
		a_pSession->Send(str.c_str(), str.size());
		return true;
	}
	SetErr(pConfig->GetErr());
	return false;
}

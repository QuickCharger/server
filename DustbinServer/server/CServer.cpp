#include "CServer.h"
#include <map>
#include <set>
#include <utility>
#include "log.h"
#include "session.h"
#include "test.pb.h"

//std::map<int, std::function< void(void*)>> MessageCode = {
//	std::make_pair(1, [&](void*){})
//};

CServer::CServer(CSession* a_pSession)
{
	m_pSession = a_pSession;
	m_pSession->SetReadCB([this](void* a_pArg){ OnReadCB(a_pArg); });
	m_pSession->SetWriteCB([this](void* a_pArg){ OnWriteCB(a_pArg); });
	m_pSession->SetErrorCB([this](void* a_pArg){ OnErrorCB(a_pArg); });
	m_pSession->SetPackCB([this](const char *a_pSource, int& a_nLength, char **a_pDest) { OnPackCB(a_pSource, a_nLength, a_pDest); });
}

CServer::~CServer()
{

}

void CServer::OnReadCB(void* a_pArg)
{
	LOG(INFO) << "Client. OnReadCB";
	char *pBuf = ((char*)a_pArg) + sizeof(int);
	test msg;
	if (msg.ParseFromString(pBuf))
	{
		LOG(INFO) << "ParseFromString success";
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
	*a_pDest = (char*)a_pSource;	//unfinish
	int nLength = a_nLength + sizeof(int);
	*a_pDest = new char[nLength];		//int + buf
	memset(*a_pDest, 0, nLength);
	memcpy(*a_pDest, (void*)&nLength, sizeof(int));
	memcpy(*a_pDest + sizeof(int), a_pSource, a_nLength);
}

bool CServer::OnUnPackCB(const char *a_pSource, int a_nLength)
{
	return *(int*)a_pSource <= a_nLength;
}

void CServer::Send(const std::string& a_strContext)
{
	m_pSession->Send(a_strContext.c_str(), a_strContext.size());
}

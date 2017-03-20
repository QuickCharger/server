#include "CClient.h"
#include <map>
#include <set>
#include <utility>
#include "log.h"
#include "session.h"
#include "test.pb.h"

std::map<int, std::function< void(void*)>> MessageCode = {
	std::make_pair(1, [&](void*){})
};

CClient::CClient(CSession* a_pSession)
{
	m_pSession = a_pSession;
	m_pSession->SetReadCB([this](void* a_pArg){ OnReadCB(a_pArg); });
	m_pSession->SetWriteCB([this](void* a_pArg){ OnWriteCB(a_pArg); });
	m_pSession->SetErrorCB([this](void* a_pArg){ OnErrorCB(a_pArg); });
}

CClient::~CClient()
{

}

void CClient::OnReadCB(void* a_pArg)
{
	LOG(INFO) << "Client. OnReadCB";
	int nBufSize = *(int*)a_pArg;
	char *pBuf = ((char*)a_pArg) + nBufSize;
	test msg;
	if (msg.ParseFromString(pBuf))
	{
		LOG(INFO) << "ParseFromString success";
	}
}

void CClient::OnWriteCB(void* a_pArg)
{
	LOG(INFO) << "Client. OnWriteCB";
}

void CClient::OnErrorCB(void* a_pArg)
{
	LOG(INFO) << "Client. OnErrorCB";
}

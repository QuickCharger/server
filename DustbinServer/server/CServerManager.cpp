#include "CServerManager.h"
#include "assert.h"
#include "CServer.h"
#include "log.h"

CServerManager::CServerManager()
{
}

CServerManager::~CServerManager()
{
	for (auto it = m_mClient.begin(); it != m_mClient.end(); ++it)
	{
		delete it->second;
	}
	m_mClient.clear();
}

bool CServerManager::AddPlayer(int a_nPlayerID, CSession *a_pSession)
{
	return false;
	assert(m_mClient.find(a_nPlayerID) == m_mClient.end());
	m_mClient[a_nPlayerID] = new CServer(a_pSession);
}

void CServerManager::AddSession(CSession* a_pSession)
{
	CServer *pNewClient = new CServer(a_pSession);
}

bool CServerManager::DeletePlayer(int a_nPlayerID)
{
	auto it = m_mClient.find(a_nPlayerID);
	if (it == m_mClient.end())
	{
		return false;
	}
	m_mClient.erase(it);
	return true;
}

CServer* CServerManager::GetPlayer(int a_nPlayerID)
{
	auto it = m_mClient.find(a_nPlayerID);
	if (it == m_mClient.end())
		return nullptr;
	assert(it->second != nullptr);
	return it->second;
}

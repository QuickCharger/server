#include "CClientManager.h"
#include "assert.h"
#include "CClient.h"
#include "log.h"

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
	for (auto it = m_mClient.begin(); it != m_mClient.end(); ++it)
	{
		delete it->second;
	}
	m_mClient.clear();
}

bool CClientManager::AddPlayer(int a_nPlayerID, CSession *a_pSession)
{
	return false;
	assert(m_mClient.find(a_nPlayerID) == m_mClient.end());
	m_mClient[a_nPlayerID] = new CClient(a_pSession);
}

bool CClientManager::DeletePlayer(int a_nPlayerID)
{
	auto it = m_mClient.find(a_nPlayerID);
	if (it == m_mClient.end())
	{
		return false;
	}
	m_mClient.erase(it);
	return true;
}

CClient* CClientManager::GetPlayer(int a_nPlayerID)
{
	auto it = m_mClient.find(a_nPlayerID);
	if (it == m_mClient.end())
		return nullptr;
	assert(it->second != nullptr);
	return it->second;
}


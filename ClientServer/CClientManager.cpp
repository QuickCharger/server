#include "CClientManager.h"
#include "CClient.h"

CClientManager::CClientManager()
{
	for (int i = 0; i < s_kMaxLink; ++i)
	{
		m_vClient.push_back(make_pair(0, nullptr));
	}
}

CClientManager::~CClientManager()
{
	for (int i = 0; i < s_kMaxLink; ++i)
	{
		auto it = m_vClient.begin() + i;
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
}

bool CClientManager::AddPlayer(int a_pFd)
{
	if (a_pFd > 0 && a_pFd < s_kMaxLink)
	{
		auto it = m_vClient.begin() + a_pFd;
		it->second = new CClient;
		return true;
	}
	return false;
}

bool CClientManager::DeletePlayer(int a_pFd)
{
	if (a_pFd > 0 && a_pFd < s_kMaxLink)
	{
		auto it = m_vClient.begin() + a_pFd;
		delete it->second;
		it->second = nullptr;
		return true;
	}
	return false;
}

CClient* CClientManager::GetPlayer(int a_pFd)
{
	if (a_pFd > 0 && a_pFd < s_kMaxLink)
	{
		auto it = m_vClient.begin() + a_pFd;
		return it->second;
	}
	return nullptr;
}


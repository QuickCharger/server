#ifndef _CLIENTSERVER_CCLIENTMANAGER_H_
#define _CLIENTSERVER_CCLIENTMANAGER_H_

#include <utility>
#include <vector>

using namespace std;

class CClient;

class CClientManager
{
public:
	CClientManager();
	~CClientManager();

	bool AddPlayer(int a_pFd);
	bool DeletePlayer(int a_pFd);
	CClient* GetPlayer(int a_pFd);

private:
	const static int s_kMaxLink = 1024;
	std::vector<pair<int, CClient*>> m_vClient;
};

#endif

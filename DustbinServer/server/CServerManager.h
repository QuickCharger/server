#ifndef _CSERVERMANAGER_H_
#define _CSERVERMANAGER_H_

#include <map>
#include <set>
#include <utility>
#include <event2/bufferevent.h>

class CServer;
class CSession;

class CServerManager
{
public:
	CServerManager();
	~CServerManager();

	bool AddPlayer(int a_nPlayerID, CSession *a_pSession);
	void AddSession(CSession* a_pSession);
	bool DeletePlayer(int a_nPlayerID);
	CServer* GetPlayer(int a_nPlayerID);

private:
	std::map<int, CServer*> m_mClient;
};

#endif

#ifndef _CCLIENTMANAGER_H_
#define _CCLIENTMANAGER_H_

#include <map>
#include <set>
#include <utility>
#include <event2/bufferevent.h>

class CClient;
class CSession;

class CClientManager
{
public:
	CClientManager();
	~CClientManager();

	//bool AddPlayer(int a_nPlayerID, CSession *a_pSession);
	void AddSession(CSession* a_pSession);
	bool DeletePlayer(int a_nPlayerID);
	CClient* GetPlayer(int a_nPlayerID);

private:
	std::map<int, CClient*> m_mClient;		//<PlayerID, client*>
};

#endif

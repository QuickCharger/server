#ifndef _DATABASESERVER_H_
#define _DATABASESERVER_H_

#include "ServerImpl.h"

class CDatabaseServer : public CServerImpl<CDatabaseServer>
{
public:
	CDatabaseServer(int argc, char* argv[]);
	~CDatabaseServer();
	//virtual void OnMessageCB(int a_nCode, void*);

	//virtual void OnMessageCB(CSession* a_pSession, int, const char *);

	void OnMessageCB1(CServer* a_pSession, const char * a_pCh);
};

#endif

#ifndef _DUSTBINSERVER_H_
#define _DUSTBINSERVER_H_

#include <string>
#include "ServerImpl.h"

class CClientServer : public CServerImpl<CClientServer>
{
public:
	CClientServer(int argc, char* argv[]);
	~CClientServer();

	//virtual void OnMessageCB(int a_nCode, void *a_Arg);

	//virtual void OnMessageCB(CSession* a_pSession, int, const  char *);
	void OnTimer(void*);
};

#endif

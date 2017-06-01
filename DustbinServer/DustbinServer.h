#ifndef _DUSTBINSERVER_H_
#define _DUSTBINSERVER_H_

#include <string>
#include "ServerImpl.h"

class CDustbinServer : public CServerImpl<CDustbinServer>
{
public:
	CDustbinServer(int argc, char* argv[]);
	~CDustbinServer();

	//virtual void OnMessageCB(int a_nCode, void *a_Arg);

	virtual void OnMessageCB(int, /*const*/ char *) {
		LOG(INFO) << "DustbinServer::OnMessageCB";
	};
};

#endif

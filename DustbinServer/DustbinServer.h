#ifndef _DUSTBINSERVER_H_
#define _DUSTBINSERVER_H_

#include "ServerImpl.h"

class CDustbinServer : public CServerImpl<CDustbinServer>
{
public:
	CDustbinServer(int argc, char* argv[]);
	~CDustbinServer();
};

#endif

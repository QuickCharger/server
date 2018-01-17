#ifndef _FRAME_ISERVERIMPL_H_
#define _FRAME_ISERVERIMPL_H_

#include "log.h"

class CServer;

class IServerImpl
{
public:
	IServerImpl() { LOG(INFO) << "IServerImpl"; };
	virtual ~IServerImpl() { LOG(INFO) << "~IServerImpl"; };
public:
	virtual void OnMessageCB(CServer* m_pServer, int, const char *) = 0;
	virtual bool AcceptServer(CServer *a_pServer) = 0;;
};

#endif

#pragma once

#include "log.h"

class CServer;

class IServerImpl
{
public:
	IServerImpl() { LOG(INFO) << "IServerImpl"; };
	virtual ~IServerImpl() { LOG(INFO) << "~IServerImpl"; };
public:
	virtual void OnMessageCB(CServer* m_pServer, int, const char *) = 0;
	virtual bool AcceptClientCB(CServer *a_pServer) = 0;
	virtual bool AcceptServerCB(CServer *a_pServer) = 0;
	virtual void LostServer(CServer* a_pServer) = 0;
};

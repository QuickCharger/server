#ifndef _FRAME_ISERVERIMPL_H_
#define _FRAME_ISERVERIMPL_H_

#include "log.h"

class IServerImpl
{
public:
	IServerImpl() { LOG(INFO) << "IServerImpl"; };
	virtual ~IServerImpl() { LOG(INFO) << "~IServerImpl"; };
public:
	virtual void OnMessageCB(CSession* a_pSession, int, const char *) = 0;
};

#endif

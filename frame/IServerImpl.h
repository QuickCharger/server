#ifndef _FRAME_ISERVERIMPL_H_
#define _FRAME_ISERVERIMPL_H_

#include "log.h"

class IServerImpl
{
public:
	IServerImpl() { LOG(INFO) << "IServerImpl"; };
	~IServerImpl() { LOG(INFO) << "~IServerImpl"; };
public:
	virtual void OnMessageCB(int, char *) = 0;
};

#endif

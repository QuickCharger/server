#pragma once

#include <string>
#include <event2/event.h>
#include "macro.h"

class CSession;

class IServer
{
public:
	IServer() {};
	virtual ~IServer() {};

	virtual void OnReadCB(int a_nCode, const std::string& msg) = 0;
	virtual void OnWriteCB(void* a_pArg) = 0;
	virtual void OnErrorCB(void* a_pArg) = 0;

	//CSession *m_pSession = nullptr;

	//event_base* m_pEventBase = nullptr;

	//DEFINE_TYPE_BASE(CSession*, m_pSession, nullptr, GetSession, SetSession);
};

#ifndef _BASE_ISERVER_H_
#define _BASE_ISERVER_H_

#include <event2/event.h>
#include "macro.h"

class CSession;

class IServer
{
public:
	//IServer(event_base* a_pEventBase, CSession *a_pSession);
	IServer();
	virtual ~IServer();

	void SetCallBack();

	virtual void OnReadCB(void *a_pArg)  = 0;
	virtual void OnWriteCB(void* a_pArg) = 0;
	virtual void OnErrorCB(void* a_pArg) = 0;

	/*
	* ·â°ü
	*/
	virtual void OnPackCB(const char *a_pSource, int& a_nLength, char **a_pDest) = 0;
	
	/*
	* ²ð°ü
	*/
	virtual bool OnUnPackCB(const char *a_pSource, int a_nLength, char **a_pDest) = 0;

	virtual bool OnConnect(CSession* a_pSession) = 0;

	CSession *m_pSession = nullptr;


	//event_base* m_pEventBase = nullptr;

	//DEFINE_TYPE_BASE(CSession*, m_pSession, nullptr, GetSession, SetSession);
};

#endif

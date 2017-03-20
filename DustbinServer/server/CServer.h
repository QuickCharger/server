#ifndef _CSERVER_H_
#define _CSERVER_H_

#include <string>
#include "macro.h"

class CSession;

class CServer
{
public:
	CServer(CSession* a_pSession);
	~CServer();

	void OnReadCB(void* a_pArg);
	void OnWriteCB(void* a_pArg);
	void OnErrorCB(void* a_pArg);

	void OnPackCB(const char *a_pSource, int& a_nLength, char **a_pDest);
	bool OnUnPackCB(const char *a_pSource, int a_nLength);

	void Send(const std::string& a_strContext);

private:
	DEFINE_PTR_BASE(CSession*, Session, nullptr, GetSession, SetSession);
};

#endif

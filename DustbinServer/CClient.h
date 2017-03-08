#ifndef _CCLIENT_H_
#define _CCLIENT_H_

#include "macro.h"

class CSession;

class CClient
{
public:
	CClient(CSession* a_pSession);
	~CClient();

	void OnReadCB(void* a_pArg);
	void OnWriteCB(void* a_pArg);
	void OnErrorCB(void* a_pArg);

private:
	DEFINE_PTR_BASE(CSession*, Session, nullptr, GetSession, SetSession);
};

#endif

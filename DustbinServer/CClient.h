#ifndef _CCLIENT_H_
#define _CCLIENT_H_

#include "macro.h"

class CSession;

class CClient
{
public:
	CClient(CSession* a_pSession);
	~CClient();

private:
	DEFINE_PTR_BASE(CSession*, Session, nullptr);
};

#endif

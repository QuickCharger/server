#include "DustbinServer.h"

/*
* map
* (��Ϣ�ţ���Ϣ�壬���պ���)
*/

CDustbinServer::CDustbinServer(int argc, char *argv[])
	: CServerImpl(argv)
{

	//AddTimer(1, this, &CDustbinServer::OnTimer, nullptr, 10);
}

CDustbinServer::~CDustbinServer()
{
	LOG(INFO) << "CDustbinServer::~CDustbinServer";
}

void CDustbinServer::OnTimer(void*)
{
	static int counter = 1;
	LOG(INFO) << "counter " << counter;
	counter++;
}

//void CDustbinServer::OnMessageCB(CSession* a_pSession, int a_nCode, const char *a_Arg)
//{
//	LOG(INFO) << "CDustbinServer::OnMessageCB";
//}

#include "DustbinServer.h"

/*
* map
* (��Ϣ�ţ���Ϣ�壬���պ���)
*/

CDustbinServer::CDustbinServer(int argc, char *argv[])
	: CServerImpl(argv)
{

}

CDustbinServer::~CDustbinServer()
{
	LOG(INFO) << "CDustbinServer::~CDustbinServer";
}

//void CDustbinServer::OnMessageCB(CSession* a_pSession, int a_nCode, const char *a_Arg)
//{
//	LOG(INFO) << "CDustbinServer::OnMessageCB";
//}

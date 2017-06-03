#include "DustbinServer.h"

CDustbinServer::CDustbinServer(int argc, char *argv[])
	: CServerImpl(argv)
{

}

CDustbinServer::~CDustbinServer()
{
	LOG(INFO) << "CDustbinServer::~CDustbinServer";
}

void CDustbinServer::OnMessageCB(int a_nCode, const char *a_Arg)
{
	LOG(INFO) << "CDustbinServer::OnMessageCB";
}

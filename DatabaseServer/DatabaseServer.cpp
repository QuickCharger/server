#include "DatabaseServer.h"

CDatabaseServer::CDatabaseServer(int argc, char *argv[])
	: CServerImpl(argv)
{

}

CDatabaseServer::~CDatabaseServer()
{
	LOG(INFO) << "CDatabaseServer::~CDatabaseServer";
}

void CDatabaseServer::OnMessageCB(int, const char *)
{
	LOG(INFO) << "DatabaseServer::OnMessageCB";
};

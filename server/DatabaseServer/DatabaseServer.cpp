#include "DatabaseServer.h"
#include "NetProtocol/common/test.pb.h"
#include "NetProtocol/server/messageCode.h"

CDatabaseServer::CDatabaseServer(int argc, char *argv[])
	: CServerImpl(argv)
{
	//RegistMessage(ServerMessageCode::eTest2, [&](CServer* a_pSession, const char * a_pCh){OnMessageCB1(a_pServer, a_pCh); });
	//AddTimer(1, [](void *a_pArg)
	//{
	//},
	//	nullptr, -1);
}

CDatabaseServer::~CDatabaseServer()
{
	LOG(INFO) << "CDatabaseServer::~CDatabaseServer";
}

void CDatabaseServer::OnMessageCB1(CServer* a_pServer, const char * a_pCh)
{
	Certification certification;
	if (certification.ParseFromString(a_pCh))
	{
		if (certification.type() == Certification::eServer)
		{

		}
		else if (certification.type() == Certification::eClient)
		{

		}
		else
		{
			LOG(WARNING) << "certification err.";
		}
	}
	else
	{
		LOG(WARNING) << "CDatabaseServer::OnMessageCB1. Parse Failed !!!";
	}
}

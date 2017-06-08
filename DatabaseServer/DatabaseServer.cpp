#include "DatabaseServer.h"
#include "NetProtocol/common/test.pb.h"
#include "NetProtocol/server/messageCode.h"

CDatabaseServer::CDatabaseServer(int argc, char *argv[])
	: CServerImpl(argv)
{
	RegistMessage(ServerMessageCode::eTest2, [&](CSession* a_pSession, const char * a_pCh){OnMessageCB1(a_pSession, a_pCh); });
}

CDatabaseServer::~CDatabaseServer()
{
	LOG(INFO) << "CDatabaseServer::~CDatabaseServer";
}

//void CDatabaseServer::OnMessageCB(CSession* a_pSession, int a_nCode, const char * a_pArg)
//{
//	LOG(INFO) << "DatabaseServer::OnMessageCB";
//
//	//if (a_nCode == )
//	//Certification certification;
//	//if (certification.ParseFromString(a_pArg))
//	//{
//	//}
//};

void CDatabaseServer::OnMessageCB1(CSession* a_pSession, const char * a_pCh)
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

#include "Client.h"
#include "Work.h"

std::map<long long, Client*> gClients;

Client::Client()
{
}

Client::~Client()
{
}

void Client::CreateClient(long long uid)
{
	Client* c = new Client;
	CLibevent::cRobot++;
	c->uid = uid;
	assert(gClients.find(uid) == gClients.end());
	gClients[uid] = c;
}

void Client::DestroyClient(long long uid)
{
	auto it = gClients.find(uid);
	assert(it != gClients.end());
	if (it != gClients.end())
	{
		delete it->second;
		CLibevent::cRobot--;
		gClients.erase(it);
	}
}

void Client::OnMsg(char *p, int len)
{
	// fake news
	char *p2 = po->Acquire(len);
	memcpy(p2, p, len);
	po->Release(p);
	CLibevent::cRecvBuf--;
	CLibevent::cSendBuf++;
	gWork->ProductMsg(uid, Event::Type::Send, p2, nullptr, len);
}

void Client::Send(char* ch, int len)
{

}

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
	CLibevent::cRecvBuf--;
	CLibevent::cSendBuf++;
	// 此处不做重新赋值了
	gWork->ProductMsg(uid, Event::Type::Send, p, nullptr, len);

	//std::cout << (char*)p << std::endl;

	//evutil_socket_t fd = bufferevent_getfd(m_bev);
	//std::cout << fd << " send " << len << std::endl;
	//bufferevent_write(m_bev, p, len);
}

void Client::Send(char* ch, int len)
{

}



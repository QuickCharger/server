#include "Client.h"

std::map<int, Client*> gClients;

Client::Client()
{
}

Client::~Client()
{
	//struct bufferevent_private2 *locking = BEV_UPCAST2(m_bev);
	int ret = bufferevent_decref(m_bev);
	m_bev = m_bev;
}

void Client::CreateClient(bufferevent* e)
{
	Client* c = new Client;
	c->m_bev = e;
	int uid = ((SocketInfo*)e->cbarg)->uid;
	gClients[uid] = c;
}

void Client::DestroyClient(bufferevent *e)
{
	int uid = ((SocketInfo*)e->cbarg)->uid;
	auto it = gClients.find(uid);
	if (it != gClients.end()) {
		delete it->second;
		gClients.erase(it);
	}
}

void Client::OnMsg(void *p, int len)
{
	//std::cout << (char*)p << std::endl;

	evutil_socket_t fd = bufferevent_getfd(m_bev);
	//std::cout << fd << " send " << len << std::endl;
	bufferevent_write(m_bev, p, len);
}

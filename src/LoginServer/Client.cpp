#include "Client.h"

std::map<int, Client*> gClients;

Client::Client()
{
}

Client::~Client()
{
	//struct bufferevent_private2 *locking = BEV_UPCAST2(m_bev);
	//int ret = bufferevent_decref(m_bev);
	//m_bev = m_bev;
	if (m_session) {
		delete m_session;
		m_session = nullptr;
	}
}

void Client::CreateClient(bufferevent* e)
{
	Client* c = new Client;
	c->InitSession(e);
	int uid = ((BufferEventArg*)e->cbarg)->uid;
	assert(gClients.find(uid) == gClients.end());
	gClients[uid] = c;
}

void Client::DestroyClient(bufferevent *e)
{
	int uid = ((BufferEventArg*)e->cbarg)->uid;
	auto it = gClients.find(uid);
	if (it != gClients.end()) {
		delete it->second;
		gClients.erase(it);
	}
	if (e->cbarg) {
		delete (BufferEventArg*)e->cbarg;
		e->cbarg = nullptr;
	}

}

void Client::InitSession(bufferevent* e) {
	this->m_session = new Session(e);
}

void Client::OnMsg(void *p, int len)
{
	if (m_session->m_ch) {
		m_session->FakeNews();
	}
	m_session->m_ch = (char*)p;
	m_session->m_len = len;
	//std::cout << (char*)p << std::endl;

	//evutil_socket_t fd = bufferevent_getfd(m_bev);
	//std::cout << fd << " send " << len << std::endl;
	//bufferevent_write(m_bev, p, len);
}

void Client::FakeNews() {
	//this->
	m_session->FakeNews();
}

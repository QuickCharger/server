#include "Session.h"
#include <iostream>
#include <utility>
#include <assert.h>
#include "common.h"

int ClientCounter = 0;

std::map<int, Session*> Sessions = {};

void Session_New(int fd, bufferevent* ev) {
	Session *s = new Session(fd, ev);
	assert(Sessions.find(fd) == Sessions.end());
	Sessions[fd] = s;
}

void Session_Destroy(int fd) {
	auto it = Sessions.find(fd);
	if (it != Sessions.end()) {
		delete it->second;
		Sessions.erase(it);
	}
}

void Session_Append(int fd, char*ch, int len) {
	auto it = Sessions.find(fd);
	if (it != Sessions.end()) {
		it->second->Append(ch, len);
	}
}

//void Client_New(int fd) {
//	Client* c = new Client();
//	c->m_fd = fd;
//	c->m_Uid = ClientCounter;
//	FD2Clients[fd] = c;
//	UID2Clients[ClientCounter] = c;
//	ClientCounter++;
//}
//
//void client_Destroy(int a_fd, int a_clientId, int a_uid) {
//	if (a_fd > 0)
//		FD2Clients.erase(a_fd);
//	if (a_clientId > 0)
//		ClientId2Clients.erase(a_clientId);
//	if (a_uid > 0)
//		UID2Clients.erase(a_uid);
//}
//
//void Client_Destroy_ClientId(int a_clientId) {
//	int fd = 0;
//	int clientId = 0;
//	int uid = 0;
//	auto it = ClientId2Clients.find(a_clientId);
//	if (it != ClientId2Clients.end()) {
//		Client *c = it->second;
//		fd = c->m_fd;
//		clientId = c->m_clientId;
//		delete c;
//	}
//	client_Destroy(fd, clientId, uid);
//}

Session::Session(int fd, bufferevent* ev) {
	//{
	//	std::unique_lock<std::mutex> lck(ioMtx);
	//	std::cout << "Session fd " << fd << std::endl;
	//}
	m_fd = fd;
	m_ev = ev;
	//bufferevent_incref(ev);
}

Session::~Session() {
	//{
	//	std::unique_lock<std::mutex> lck(ioMtx);
	//	std::cout << "~Session destroy fd " << m_fd << std::endl;
	//}
	if (m_ch) {
		delete m_ch;
		m_ch = nullptr;
	}
	if (m_ev) {
		struct bufferevent_private2 *locking = BEV_UPCAST2(m_ev);
		bufferevent_decref(m_ev);
		m_ev = nullptr;
	}
}

void Session::Append(char* ch, int len) {
	if (m_ch) {
		delete m_ch;
	}
	m_ch = ch;
	m_len = len;
}


void Session::Send(char* ch, int len) {
	struct bufferevent_private2 *locking = BEV_UPCAST2(m_ev);
	bufferevent_write(m_ev, ch, len);
}

void Session::Desc() {
	{
		std::unique_lock<std::mutex> lck(ioMtx);
		std::cout << "fd " << m_fd << " data " << (m_ch == nullptr ? "" : m_ch) << " datalen " << m_len << std::endl;
	}
}

void Session::FakeNews() {
	if (m_ch) {
		Send(m_ch, m_len);
		delete m_ch;
		m_ch = nullptr;
		m_len = 0;
	}
}

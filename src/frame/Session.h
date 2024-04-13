#pragma once

#include <map>
#include "Libevent.h"

class Session;
extern std::map<int, Session*> Sessions;

void Session_New(int fd, bufferevent*);
void Session_Destroy(int fd);
void Session_Append(int fd, char* p, int len);

class Session {
public:
	Session(int fd, bufferevent* ev);
	virtual ~Session();
	void Append(char* ch, int len);
	void Send(char* ch, int len);
	void Desc();
	void FakeNews();

public:
	int m_fd = 0;
	char *m_ch = nullptr;
	int m_len = 0;
	bufferevent* m_ev = nullptr;
};

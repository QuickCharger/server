#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <mutex>
#include <deque>
#include <mutex>
#include <list>
#include <vector>
#include <assert.h>

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/bufferevent_struct.h>

#include "IRunnable.cc"
#include "ProductConsume.cc"

/*
  网络事件库
    events是libevent生成的事件 需要锁上读取
  AUTHOR
    2024/04/07 BDG INIT
*/

class CLibevent;

struct BufferEventArg {
	CLibevent* that = nullptr;
	int uid = 0;
};

class CLibevent : public IRunnable {
public:
	CLibevent() {};
	~CLibevent() {};
	int Init();
	int Listen(int port);
	int Connect(char* ip, int port);
	int Run();

	//int AddEvent(Event);

	int Stop();
	void Close();

	void Consume(std::vector<EventStruct>** p);
	void Product(std::vector<EventStruct>** p);

public:
	static void log(int severity, const char *msg);

	void onTimer1ms(evutil_socket_t, short, void*);
	void onTimer1s(evutil_socket_t, short, void*);
	void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx);
	void accept_error_cb(struct evconnlistener *listener, void *ctx);


	void socket_read_cb(struct bufferevent *bev, void *ctx);
	void socket_write_cb(struct bufferevent *bev, void *ctx);
	void socket_event_cb(struct bufferevent *bev, short a_events, void *ctx);

	static void accept_conn_cb_static(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx);
	static void accept_error_cb_static(struct evconnlistener *listener, void *ctx);
	static void socket_read_cb_static(struct bufferevent *bev, void *ctx);
	static void socket_write_cb_static(struct bufferevent *bev, void *ctx);
	static void socket_event_cb_static(struct bufferevent *bev, short a_events, void *ctx);

private:
	struct event_base *base = nullptr;

	SafeBuffer<EventStruct> eventsFromNet;
	SafeBuffer<EventStruct> eventsFromUser;

	std::vector<EventStruct>* pEventP = nullptr;
	std::vector<EventStruct>* pEventC = nullptr;
};

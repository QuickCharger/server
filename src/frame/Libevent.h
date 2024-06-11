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
#include <atomic>
#include <map>

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/bufferevent_struct.h>

#include "IRunnable.h"
#include "ProductConsume.h"

/*
  �����¼���
    events��libevent���ɵ��¼� ��Ҫ���϶�ȡ
  AUTHOR
    2024/04/07 BDG INIT
*/

enum class SocketState {
	BLANK = 0,	// ռλ
	Connecting,	// ��������ing
	Connected,	// ����
	Err,		// ������
	Closeing,	// �����ر�ing Ҳ���Ǵ����Ĵ�����
	Closed,		// 
};

struct bufferevent;
class CLibevent;

// ȷ��plain
// ���������Ӧ��ֻ��libevent�߳����޸� ��һ���߳�Ӧֻ��
// ȷ�����ݾ�����ֻ�� �����Ҫ�޸� ��Ӧ���޸�������¼�����һ���߳�
struct BevInfo {
	long long uid = 0;
	bufferevent* bev = nullptr;
	CLibevent* that = nullptr;
	//SocketState state = SocketState::BLANK;
};


class CLibevent : public IRunnable {
public:
	CLibevent() {};
	~CLibevent() {};
	int Init();
	int Listen(int port);
	int Run();

	//int AddEvent(Event);

	int Stop();
	void Close();

	void Consume(std::vector<Event>** p);
	void Product(std::vector<Event>** p);
	void ProductMsg(long long uid, Event::Type t, void* p1 = nullptr, void *p2 = nullptr, long long l1 = 0, long long l2 = 0, const std::string& str1 = "", const std::string& str2 = "");

public:
	static void log(int severity, const char *msg);
	static long long GenUid();

	// bufferevent �Ĳ���
	struct bufferevent* genBEV(event_base* base, evutil_socket_t fd, int options, long long uid=0);
	int  delBEV(event_base* base, bufferevent* bev);
	int  connectTo(struct bufferevent *bev, const std::string& ip, int port);
	//void updateFdState(struct bufferevent* bev, SocketState state);

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

	SafeBuffer<Event> eventsFromNet;
	SafeBuffer<Event> eventsFromUser;

	std::vector<Event>* pEventP = nullptr;
	std::vector<Event>* pEventC = nullptr;

	std::map<long long, bufferevent*> m;

	static std::atomic<long long> cUid;

public:
	static std::atomic<int> cBevInfo;
	static std::atomic<int> cRecvBuf;
	static std::atomic<int> cSendBuf;
	static std::atomic<int> cSession;
	static std::atomic<int> cRobot;
	static std::atomic<int> cbufferevent_incref;
};

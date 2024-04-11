#pragma once

#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/bufferevent_struct.h>


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <mutex>
#include <deque>
#include <mutex>
#include <list>

/*
  网络事件库
    events是libevent生成的事件 需要锁上读取
    Event
      DataIn 有数据输入
	  SocketErr socket需要关闭
  AUTHOR
    2024/04/07 BDG INIT
*/

namespace LIBEVENT {
	enum class Event {
		SocketCreate = 1,
		DataIn,
		SocketErr,
		Close,
	};
	extern std::mutex mtxEventsOUT;
	extern std::list<std::tuple<int, Event, void*, int>> eventsOUT;

	//extern std::mutex mtxEventsIN;
	//extern std::list<std::tuple<Event>> eventsIN;

	int Init();
	int Listen(int port);
	int Connect(char* ip, int port);
	void Run();

	//int AddEvent(Event);

	void Close();

	extern struct event_base *base;
}


typedef ev_uint16_t bufferevent_suspend_flags;

struct bufferevent_private2 {
	/** The underlying bufferevent structure. */
	struct bufferevent bev;

	/** Evbuffer callback to enforce watermarks on input. */
	struct evbuffer_cb_entry *read_watermarks_cb;

	/** If set, we should free the lock when we free the bufferevent. */
	unsigned own_lock : 1;

	/** Flag: set if we have deferred callbacks and a read callback is
	* pending. */
	unsigned readcb_pending : 1;
	/** Flag: set if we have deferred callbacks and a write callback is
	* pending. */
	unsigned writecb_pending : 1;
	/** Flag: set if we are currently busy connecting. */
	unsigned connecting : 1;
	/** Flag: set if a connect failed prematurely; this is a hack for
	* getting around the bufferevent abstraction. */
	unsigned connection_refused : 1;
	/** Set to the events pending if we have deferred callbacks and
	* an events callback is pending. */
	short eventcb_pending;

	/** If set, read is suspended until one or more conditions are over.
	* The actual value here is a bitfield of those conditions; see the
	* BEV_SUSPEND_* flags above. */
	bufferevent_suspend_flags read_suspended;

	/** If set, writing is suspended until one or more conditions are over.
	* The actual value here is a bitfield of those conditions; see the
	* BEV_SUSPEND_* flags above. */
	bufferevent_suspend_flags write_suspended;

	/** Set to the current socket errno if we have deferred callbacks and
	* an events callback is pending. */
	int errno_pending;

	/** The DNS error code for bufferevent_socket_connect_hostname */
	int dns_error;

	/** Used to implement deferred callbacks */
	struct event_callback deferred;

	/** The options this bufferevent was constructed with */
	enum bufferevent_options options;

	/** Current reference count for this bufferevent. */
	int refcnt;

	/** Lock for this bufferevent.  Shared by the inbuf and the outbuf.
	* If NULL, locking is disabled. */
	void *lock;

	/** No matter how big our bucket gets, don't try to read more than this
	* much in a single read operation. */
	ev_ssize_t max_single_read;

	/** No matter how big our bucket gets, don't try to write more than this
	* much in a single write operation. */
	ev_ssize_t max_single_write;

	/** Rate-limiting information for this bufferevent */
	struct bufferevent_rate_limit *rate_limiting;

	/* Saved conn_addr, to extract IP address from it.
	*
	* Because some servers may reset/close connection without waiting clients,
	* in that case we can't extract IP address even in close_cb.
	* So we need to save it, just after we connected to remote server, or
	* after resolving (to avoid extra dns requests during retrying, since UDP
	* is slow) */
	union {
		struct sockaddr_in6 in6;
		struct sockaddr_in in;
	} conn_address;

	struct evdns_getaddrinfo_request *dns_request;
};


#define offsetof2(s,m) ((size_t)&(((s*)0)->m))

#define evutil_offsetof2(type, field) offsetof2(type, field)

#define EVUTIL_UPCAST2(ptr, type, field)				\
	((type *)(((char*)(ptr)) - evutil_offsetof2(type, field)))

#define BEV_UPCAST2(b) EVUTIL_UPCAST2((b), struct bufferevent_private2, bev)


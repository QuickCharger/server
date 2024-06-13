#include "Libevent.h"
#include <atomic>
#include <functional>
#include <mutex>
#include "common.h"
#include "event2/thread.h"
#include "map"

std::atomic<long long> CLibevent::cUid{ 1 };

std::atomic<int> CLibevent::cBevInfo{ 0 };
std::atomic<int> CLibevent::cRecvBuf{ 0 };
std::atomic<int> CLibevent::cSendBuf{ 0 };
std::atomic<int> CLibevent::cSession{ 0 };
std::atomic<int> CLibevent::cRobot{ 0 };
std::atomic<int> CLibevent::cbufferevent_incref{ 0 };

CMemPool<char> *po = new CMemPool<char>;

int CLibevent::Init() {
#ifdef WIN32
	WSADATA wsaData;
	int wsaData_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (wsaData_result != NO_ERROR) {
		printf("WSAStartup failed: %d\n", wsaData_result);
		return -1;
	}
#endif

	this->pEventP = eventsFromNet.Productor(this->pEventP);
	this->pEventC = eventsFromUser.Comsumer(this->pEventC);

#ifdef WIN32
	evthread_use_windows_threads();
#else
	evthread_use_pthreads();
#endif

	event_set_log_callback(CLibevent::log);

	this->base = event_base_new();
	if (!this->base)
	{
		puts("Couldn't open event base");
		return -2;
	}

	{
		struct timeval one_ms_delay = { 0, 1 };
		struct event *timeout_event = event_new(this->base, -1, EV_PERSIST, [](evutil_socket_t fd, short event, void *arg) { ((CLibevent*)arg)->onTimer1ms(fd, event, nullptr); }, this);
		event_add(timeout_event, &one_ms_delay);
	}
	{
		struct timeval one_sec_delay = { 1, 0 };
		struct event *timeout_event = event_new(this->base, -1, EV_PERSIST, [](evutil_socket_t fd, short event, void *arg) { ((CLibevent*)arg)->onTimer1s(fd, event, nullptr); }, this);
		event_add(timeout_event, &one_sec_delay);
	}

	if (this->config.port > 0)
	{
		Listen(this->config.port);
	}

	return 0;
}

int CLibevent::Listen(int port)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0);
	sin.sin_port = htons(port);

	struct evconnlistener *listener = evconnlistener_new_bind(base, CLibevent::accept_conn_cb_static, this, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&sin, sizeof(sin));
	if (!listener)
	{
		perror("Couldn't create listener");
		return 1;
	}
	evconnlistener_set_error_cb(listener, CLibevent::accept_error_cb_static);

	return 0;
}

int CLibevent::Run()
{
	{
		std::unique_lock<std::mutex> lck(ioMtx);
		std::cout << __FUNCTION__ << std::endl;
	}
	int result = event_base_loop(base, EVLOOP_NO_EXIT_ON_EMPTY);
	perror("LIBEVENT msg");
	return 0;
}

int CLibevent::Stop() {
	return 0;
}

void CLibevent::Close() {
	{
		std::unique_lock<std::mutex> lck(ioMtx);
		std::cout << "LIBEVENT " << __FUNCTION__ << std::endl;
	}
#ifdef WIN32
	WSACleanup();
#endif
	if (base) {
		event_base_loopexit(base, NULL);
		base = nullptr;
	}
}

// 消费消息
// 通常是 网络消息 供 使用者 读取
void CLibevent::Consume(std::vector<Event>** p) {
	*p = eventsFromNet.Comsumer(*p);
}

// 产生消息
// 通常是 使用者 产生消息给 libevent
void CLibevent::Product(std::vector<Event>** p) {
	*p = eventsFromUser.Productor(*p);
}

void CLibevent::ProductMsg(long long uid, Event::Type t, void* p1, void *p2, long long l1, long long l2, const std::string& str1, const std::string& str2)
{
	Event e;
	e.uid = uid;
	e.e = t;
	e.p1 = p1;
	e.p2 = p2;
	e.l1 = l1;
	e.l2 = l2;
	e.str1 = str1;
	e.str2 = str2;
	pEventP->push_back(std::move(e));
}

void CLibevent::log(int severity, const char *msg)
{
	std::unique_lock<std::mutex> lck(ioMtx);
	std::cout << msg << std::endl;
}

long long CLibevent::GenUid()
{
	return ++CLibevent::cUid;
}

struct bufferevent* CLibevent::genBEV(event_base* base, evutil_socket_t fd, int options, long long uid)
{
	uid = uid == 0 ? CLibevent::GenUid() : uid;
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	BevInfo *info = new BevInfo;
	CLibevent::cBevInfo++;
	info->bev = bev;
	info->that = this;
	info->uid = uid;
	bufferevent_setcb(bev, CLibevent::socket_read_cb_static, CLibevent::socket_write_cb_static, CLibevent::socket_event_cb_static, info);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
	m[uid] = bev;
	// 可以在此处发Event 与delBEV中的Event对应 但此处不能确定是主动链接还是被动链接 也许libevent线程不需要考虑这个？？？ todo
	return bev;
}

int CLibevent::delBEV(event_base* base, bufferevent* bev)
{
	long long uid = ((BevInfo*)(bev->cbarg))->uid;
	if (m.find(uid) == m.end())
	{
		std::cout << __FUNCTION__ << " " << uid << " should not happend" << std::endl;
	}
	assert(m.find(uid) != m.end());
	bufferevent_disable(bev, EV_READ | EV_WRITE);
	delete (BevInfo*)(bev->cbarg);
	CLibevent::cBevInfo--;
	bev->cbarg = nullptr;
	bufferevent_free(bev);
	m.erase(uid);
	ProductMsg(uid, Event::Type::Close);
	return 0;
}

int CLibevent::connectTo(struct bufferevent *bev, const std::string& ip, int port)
{
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &sin.sin_addr);
	return bufferevent_socket_connect(bev, (struct sockaddr *)&sin, sizeof(sin));
}

void CLibevent::onTimer1ms(evutil_socket_t, short, void*)
{
	this->pEventP = this->eventsFromNet.Productor(this->pEventP);
	this->pEventC = this->eventsFromUser.Comsumer(this->pEventC);

	long long uid = 0;
	for (auto it = pEventC->begin(); it != pEventC->end(); ++it)
	{
		uid = it->uid;
		auto itBev = m.find(uid);
		if (it->e == Event::Type::Send)
		{
			if (itBev != m.end())
			{
				bufferevent_write(itBev->second, it->p1, (int)it->l1);
			}
			//delete []it->p1;
			po->Release((char*)it->p1);
			CLibevent::cSendBuf--;
		}
		else if (it->e == Event::Type::ConnectTo)
		{
			struct bufferevent *bev = this->genBEV(base, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE, uid);
			if (this->connectTo(bev, it->str1, (int)it->l1) != 0)
			{
				ProductMsg(uid, Event::Type::Err);
				delBEV(base, bev);
			}
		}
		else if (it->e == Event::Type::TryClose)
		{
			if (itBev != m.end())
			{
				delBEV(base, itBev->second);
			}
		}
	}
	pEventC->clear();
}

void CLibevent::onTimer1s(evutil_socket_t, short, void*) {
	//std::unique_lock<std::mutex> lck(ioMtx);
	//std::cout << "OnTimer1s socket total " << cTotal << " living " << cLiving << std::endl;
	//std::cout << "BevInfo " << cBevInfo << " RecvBuf " << cRecvBuf << " cSendBuf " << cSendBuf << " Session " << cSession << " cbufferevent_incref " << cbufferevent_incref << std::endl;
}

void CLibevent::accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = this->genBEV(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	ProductMsg(((BevInfo*)(bev->cbarg))->uid, Event::Type::SocketAccept);
}

void CLibevent::accept_error_cb(struct evconnlistener *listener, void *ctx)
{
	struct event_base *base = evconnlistener_get_base(listener);
	int err = EVUTIL_SOCKET_ERROR();
	{
		std::unique_lock<std::mutex> lck(ioMtx);
		std::cout << "Got an error " << err << " (" << evutil_socket_error_to_string(err) << ") on the listener. " "Shutting down." << std::endl;
	}
	Close();
}


void CLibevent::socket_read_cb(struct bufferevent *bev, void *)
{
	evutil_socket_t fd = bufferevent_getfd(bev);
	struct evbuffer *input = bufferevent_get_input(bev);
	struct evbuffer *output = bufferevent_get_output(bev);
	//evbuffer_add_buffer(output, input);

	// runnable 或 libevent 可能不会正确delete 需要进一步处理 todo
	size_t len = evbuffer_get_length(input);
	//char *ch = new char[len] { 0 };
	char* ch = po->Acquire(len);
	CLibevent::cRecvBuf++;

	evbuffer_remove(input, ch, len);

	ProductMsg(((BevInfo*)(bev->cbarg))->uid, Event::Type::DataIn, ch, nullptr, len);
}

void CLibevent::socket_write_cb(struct bufferevent *bev, void *)
{}

void CLibevent::socket_event_cb(struct bufferevent *bev, short a_events, void *)
{
	struct evbuffer *input = bufferevent_get_input(bev);
	int finished = 0;

	if (a_events & BEV_EVENT_EOF)
	{
		size_t len = evbuffer_get_length(input);
		if (len > 0)
		{
			std::cout << __FUNCTION__ << " BEV_EVENT_EOF evBuf left size " << len << std::endl;
		}
		finished = 1;
	}
	else if (a_events & BEV_EVENT_ERROR)
	{
		size_t len = evbuffer_get_length(input);
		if(len > 0)
		{
			std::cout << __FUNCTION__ << " BEV_EVENT_ERROR evBuf left size " << len << std::endl;
		}
		finished = 1;
		ProductMsg(((BevInfo*)(bev->cbarg))->uid, Event::Type::Err);
	}
	// 主动链接成功后的消息
	else if (a_events & BEV_EVENT_CONNECTED)
	{
		ProductMsg(((BevInfo*)(bev->cbarg))->uid, Event::Type::ConnectSuccess);
	}

	if (finished == 1) {
		delBEV(base, bev);
	}
}

void CLibevent::accept_conn_cb_static(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
	CLibevent* that = (CLibevent*)ctx;
	that->accept_conn_cb(listener, fd, address, socklen, nullptr);
}

void CLibevent::accept_error_cb_static(struct evconnlistener *listener, void *ctx)
{
	// todo
}

void CLibevent::socket_read_cb_static(struct bufferevent *bev, void *arg)
{
	CLibevent* that = ((BevInfo*)arg)->that;
	that->socket_read_cb(bev, arg);
}

void CLibevent::socket_write_cb_static(struct bufferevent *bev, void *arg)
{
	CLibevent* that = ((BevInfo*)arg)->that;
	that->socket_write_cb(bev, arg);
}

void CLibevent::socket_event_cb_static(struct bufferevent *bev, short a_events, void *arg)
{
	CLibevent* that = ((BevInfo*)arg)->that;
	that->socket_event_cb(bev, a_events, arg);
}

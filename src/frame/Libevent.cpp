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
std::atomic<int> CLibevent::cSession{ 0 };
std::atomic<int> CLibevent::cbufferevent_incref{ 0 };

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
	if (!listener) {
		perror("Couldn't create listener");
		return 1;
	}
	evconnlistener_set_error_cb(listener, CLibevent::accept_error_cb_static);

	return 0;
}

int CLibevent::Run() {
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
// 通常是 使用者 产生消息给 client 或 libevent
void CLibevent::Product(std::vector<Event>** p) {
	// todo
	*p = eventsFromUser.Productor(*p);
}

void CLibevent::log(int severity, const char *msg)
{
	{
		std::unique_lock<std::mutex> lck(ioMtx);
		std::cout << msg << std::endl;
	}
	//switch (severity) {
	//case _EVENT_LOG_DEBUG: s = "debug"; break;
	//case _EVENT_LOG_MSG:   s = "msg";   break;
	//case _EVENT_LOG_WARN:  s = "warn";  break;
	//case _EVENT_LOG_ERR:   s = "error"; break;
	//default:               s = "?";     break; /* never reached */
	//}
	//fprintf(logfile, "[%s] %s\n", s, msg);
}

long long CLibevent::GenUid() {
	return ++CLibevent::cUid;
}

struct bufferevent* CLibevent::genBEV(event_base* base, evutil_socket_t fd, int options)
{
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	BevInfo *info = new BevInfo;
	cBevInfo++;
	info->bev = bev;
	info->that = this;
	info->uid = CLibevent::GenUid();
	bufferevent_setcb(bev, CLibevent::socket_read_cb_static, CLibevent::socket_write_cb_static, CLibevent::socket_event_cb_static, info);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
	return bev;
}

int CLibevent::delBEV(event_base* base, bufferevent* bev)
{
	bufferevent_disable(bev, EV_READ | EV_WRITE);
	delete (BevInfo*)(bev->cbarg);
	cBevInfo--;
	bev->cbarg = nullptr;
	//{
	//	int c = 0;
	//	int r = 0;
	//	do {
	//		c++;
	//		r = bufferevent_decref(bev);
	//	} while (r != 1);
	//	std::cout << "delBev c " << c << std::endl;
	//}
	bufferevent_decref(bev);
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

void CLibevent::updateFdState(struct bufferevent* bev, SocketState state)
{
	((BevInfo *)bev->cbarg)->state = state;
}

void CLibevent::onTimer1ms(evutil_socket_t, short, void*) {
	this->pEventP = this->eventsFromNet.Productor(this->pEventP);
	this->pEventC = this->eventsFromUser.Comsumer(this->pEventC);

	for (auto it = pEventC->begin(); it != pEventC->end(); ++it)
	{
		if (it->e == Event::Type::SocketConnectTo)
		{
			std::string ip = it->str1;
			int port = it->i1;
			long long uid = it->uid;

			struct bufferevent *bev = this->genBEV(base, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
			((BevInfo *)bev->cbarg)->uid = uid;
			//bufferevent_incref(bev);
			//cbufferevent_incref++;

			updateFdState(bev, SocketState::Connecting);

			Event e;
			e.e = Event::Type::RegBufferEvent;
			e.uid = uid;
			e.p1 = bev;
			pEventP->push_back(std::move(e));

			if (this->connectTo(bev, ip, port) != 0)
			{
				updateFdState(bev, SocketState::Err);

				Event e;
				e.p1 = bev;
				e.e = Event::SocketConnectErr;
				e.uid = uid;
				pEventP->push_back(std::move(e));
			}
		}
		else if (it->e == Event::Type::SocketTryClose)
		{
			struct bufferevent *bev = (bufferevent *)it->p1;
			delBEV(base, bev);
		}
	}
	pEventC->clear();
}

void CLibevent::onTimer1s(evutil_socket_t, short, void*) {
	std::unique_lock<std::mutex> lck(ioMtx);
	//std::cout << "OnTimer1s socket total " << cTotal << " living " << cLiving << std::endl;
	//std::cout << "BevInfo " << cBevInfo << " RecvBuf " << cRecvBuf << " Session " << cSession << " cbufferevent_incref " << cbufferevent_incref << std::endl;
}

void CLibevent::accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
	struct event_base *base = evconnlistener_get_base(listener);

	struct bufferevent *bev = this->genBEV(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);
	// bev在session有引用
	// 如果不提前incref 可能会出现session创建前bev就销毁 会崩溃
	//bufferevent_incref(bev);
	//cbufferevent_incref++;

	{
		Event e;
		e.e = Event::Type::SocketAccept;
		e.p1 = bev;
		pEventP->push_back(std::move(e));
	}
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

	size_t len = evbuffer_get_length(input);
	char *ch = new char[len] { 0 };
	cRecvBuf++;

	evbuffer_remove(input, ch, len);

	Event e;
	e.uid = ((BevInfo*)(bev->cbarg))->uid;
	e.e = Event::DataIn;
	e.p1 = ch;
	e.i1 = len;
	pEventP->push_back(std::move(e));
}

void CLibevent::socket_write_cb(struct bufferevent *bev, void *)
{}

void CLibevent::socket_event_cb(struct bufferevent *bev, short a_events, void *)
{
	Event e;
	e.uid = ((BevInfo *)bev->cbarg)->uid;

	struct evbuffer *input = bufferevent_get_input(bev);
	int finished = 0;

	if (a_events & BEV_EVENT_EOF)
	{
		size_t len = evbuffer_get_length(input);
		if (len > 0)
		{
			std::cout << __FUNCTION__ << " evBuf left size " << len << std::endl;
		}
		finished = 1;
	}
	else if (a_events & BEV_EVENT_ERROR)
	{
		size_t len = evbuffer_get_length(input);
		if(len > 0)
		{
			std::cout << __FUNCTION__ << " evBuf left size " << len << std::endl;
		}
		finished = 1;
	}
	// 主动链接成功后的消息
	else if (a_events & BEV_EVENT_CONNECTED)
	{
		e.e = Event::Type::SocketConnectSuccess;
		updateFdState(bev, SocketState::Connected);
	}

	if (finished == 1) {
		delBEV(base, bev);
		e.e = Event::Type::SocketErr;
	}

	pEventP->push_back(std::move(e));

	if (finished) {
		//bufferevent_decref(bev);
		//bufferevent_free(bev);
	}
}

void CLibevent::accept_conn_cb_static(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
	CLibevent* that = (CLibevent*)ctx;
	that->accept_conn_cb(listener, fd, address, socklen, nullptr);
}

void CLibevent::accept_error_cb_static(struct evconnlistener *listener, void *ctx) {
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

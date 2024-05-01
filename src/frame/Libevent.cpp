#include "Libevent.h"
#include <functional>
#include "mutex"
#include "common.h"
#include "event2/thread.h"
#include "map"

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
	if (!this->base) {
		puts("Couldn't open event base");
		return -2;
	}

	{
		struct timeval one_ms_delay = { 0, 1 };
		struct event *timeout_event = event_new(this->base, -1, EV_PERSIST, [](evutil_socket_t fd, short event, void *arg) { ((CLibevent*)arg)->onTimer1ms(fd, event, nullptr); }, this);
		event_add(timeout_event, &one_ms_delay);
	}
	{
		struct timeval one_ms_delay = { 1, 0 };
		struct event *timeout_event = event_new(this->base, -1, EV_PERSIST, [](evutil_socket_t fd, short event, void *arg) { ((CLibevent*)arg)->onTimer1s(fd, event, nullptr); }, nullptr);
		event_add(timeout_event, &one_ms_delay);
	}

	if (this->config.port) {
		Listen(this->config.port);
	}

	return 0;
}

int CLibevent::Listen(int port) {
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(0);
	sin.sin_port = htons(port);

	struct evconnlistener *listener = evconnlistener_new_bind(base, 
		[](struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx) {
			((CLibevent*)ctx)->accept_conn_cb(listener, fd, address, socklen, nullptr); 
		},
		NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&sin, sizeof(sin));
	if (!listener) {
		perror("Couldn't create listener");
		return 1;
	}
	evconnlistener_set_error_cb(listener, [](struct evconnlistener *listener, void *ctx)->void {
		((CLibevent*)ctx)->accept_error_cb(listener, nullptr);
	});

	return 0;
}

// todo
int CLibevent::Connect(char* ip, int port) {
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

//int AddEvent(Event);

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
void CLibevent::Consume(std::vector<EventStruct>** p) {
	*p = eventsFromNet.Comsumer(*p);
}

// 产生消息
// 通产是 使用者 产生消息给 client 或 libevent
void CLibevent::Product(std::vector<EventStruct>** p) {
	// todo
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

void CLibevent::onTimer1ms(evutil_socket_t, short, void*) {
	this->pEventP = this->eventsFromNet.Productor(this->pEventP);
	this->pEventC = this->eventsFromUser.Comsumer(this->pEventC);
}

void CLibevent::onTimer1s(evutil_socket_t, short, void*) {
	std::unique_lock<std::mutex> lck(ioMtx);
	//std::cout << "OnTimer1s socket total " << cTotal << " living " << cLiving << std::endl;
}


void CLibevent::accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
{
	struct event_base *base = evconnlistener_get_base(listener);
	struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);

	// bev在session有引用
	// 如果不提前incref 可能会出现session创建前bev就销毁 会崩溃
	//bufferevent_incref(bev);

	SocketInfo *info = new SocketInfo;
	//info->uid = ++uid;

	//bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, info);
	bufferevent_enable(bev, EV_READ | EV_WRITE);


	{
		EventStruct e;
		e.p1 = bev;
		e.e = Event::SocketCreate;
		pEventP->push_back(std::move(e));
	}

	//cTotal++;
	//cLiving++;
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


void CLibevent::socket_read_cb(struct bufferevent *bev, void *ctx)
{
	evutil_socket_t fd = bufferevent_getfd(bev);
	struct evbuffer *input = bufferevent_get_input(bev);
	struct evbuffer *output = bufferevent_get_output(bev);
	//evbuffer_add_buffer(output, input);

	size_t len = evbuffer_get_length(input);
	char *ch = new char[len] { 0 };

	//SocketInfo *info = (SocketInfo*)ctx;
	//info->total_drained += len;

	{
		//std::unique_lock<std::mutex> lck(ioMtx);
		//std::cout << "libevent fd " << fd << " data in len" << len << std::endl;
	}
	//evbuffer_copyout(input, ch, len);
	evbuffer_remove(input, ch, len);

	EventStruct e;
	e.p1 = bev;
	e.e = Event::DataIn;
	e.p2 = ch;
	e.i2 = len;
	pEventP->push_back(std::move(e));
}

void CLibevent::socket_event_cb(struct bufferevent *bev, short a_events, void *ctx)
{
	// todo
	// a_events 处理的不完善 没有处理所有的情况

	//evutil_socket_t fd = bufferevent_getfd(bev);
	//{
	//	std::unique_lock<std::mutex> lck(mtxEventsOUT);
	//	eventsOUT.push_back(std::make_tuple(fd, Event::SocketErr, nullptr, 0));
	//}


	EventStruct e;
	e.e = Event::SocketErr;
	e.p1 = bev;
	pEventP->push_back(std::move(e));

	struct SocketInfo *inf = (SocketInfo*)ctx;
	struct evbuffer *input = bufferevent_get_input(bev);
	int finished = 0;

	if (a_events & BEV_EVENT_EOF) {
		size_t len = evbuffer_get_length(input);
		{
			//std::unique_lock<std::mutex> lck(ioMtx); 
			//std::cout << "Got a close from " << inf->name << ". We drained " << inf->total_drained << " bytes from it, and have " << len << " left." << std::endl;
		}
		finished = 1;
	}
	if (a_events & BEV_EVENT_ERROR) {
		{
			std::unique_lock<std::mutex> lck(ioMtx);
			//std::cout << "Got " << a_events<< " error from " << inf->name << " : " << evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()) << std::endl;
		}
		finished = 1;
	}
	finished = 1;

	bufferevent_disable(bev, EV_READ | EV_WRITE);
	//if (finished) {
	//	bufferevent_free(bev);
	//}

	//cLiving--;
	//{
	//	std::unique_lock<std::mutex> lck(ioMtx);
	//	std::cout << "income socket total " << cTotal << " living " << cLiving << std::endl;
	//}

}

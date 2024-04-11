#include "Libevent.h"
#include "mutex"
#include "common.h"
#include "event2/thread.h"

struct SocketInfo {
	SocketInfo(int a_name) {
		name = new char[10];
		memset(name, 0, sizeof(name));
		std::sprintf(name, "%d", a_name);
	}
	~SocketInfo() {
		delete name;
	}
	char *name = nullptr;
	size_t total_drained = 0;
};

int cTotal = 0;
int cLiving = 0;
namespace LIBEVENT {
	struct event_base *base = nullptr;
	std::mutex mtxEventsOUT;
	std::list<std::tuple<int, Event, void*, int>> eventsOUT;

	void onTimer1s(evutil_socket_t, short, void *) {
		{
			std::unique_lock<std::mutex> lck(ioMtx);
			std::cout << "OnTimer1s socket total " << cTotal << " living " << cLiving << std::endl;
		}
	}

	static void log(int severity, const char *msg)
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

	int Init() {
#ifdef WIN32
		WSADATA wsaData;
		int wsaData_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (wsaData_result != NO_ERROR) {
			printf("WSAStartup failed: %d\n", wsaData_result);
			return -1;
		}
#endif

#ifdef WIN32
		evthread_use_windows_threads();
#else
		evthread_use_pthreads();
#endif

		event_set_log_callback(log);

		LIBEVENT::base = event_base_new();
		if (!base) {
			puts("Couldn't open event base");
			return -2;
		}

		struct timeval one_ms_delay = { 1, 0 };
		struct event *timeout_event = event_new(base, -1, EV_PERSIST, onTimer1s, nullptr);
		event_add(timeout_event, &one_ms_delay);

		return 0;
	}


	static void socket_read_cb(struct bufferevent *bev, void *ctx)
	{
		evutil_socket_t fd = bufferevent_getfd(bev);
		struct evbuffer *input = bufferevent_get_input(bev);
		struct evbuffer *output = bufferevent_get_output(bev);

		//evbuffer_add_buffer(output, input);
		//return;

		size_t len = evbuffer_get_length(input);
		char *ch = new char[len + 1]{ 0 };

		SocketInfo *info = (SocketInfo*)ctx;
		info->total_drained += len;

		{
			//std::unique_lock<std::mutex> lck(ioMtx);
			//std::cout << "libevent fd " << fd << " data in len" << len << std::endl;
		}
		evbuffer_copyout(input, ch, len);

		{
			std::unique_lock<std::mutex> lck(mtxEventsOUT);
			eventsOUT.push_back(std::make_tuple(fd, Event::DataIn, ch, len+1));
		}
	}

	static void socket_event_cb(struct bufferevent *bev, short a_events, void *ctx)
	{
		// todo
		// a_events 处理的不完善 没有处理所有的情况

		evutil_socket_t fd = bufferevent_getfd(bev);
		{
			std::unique_lock<std::mutex> lck(mtxEventsOUT);
			eventsOUT.push_back(std::make_tuple(fd, Event::SocketErr, nullptr, 0));
		}

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
				std::cout << "Got " << a_events<< " error from " << inf->name << " : " << evutil_socket_error_to_string(EVUTIL_SOCKET_ERROR()) << std::endl;
			}
			finished = 1;
		}
		finished = 1;

		if (finished) {

			delete inf;
			bufferevent_free(bev);
		}

		cLiving--;
		//{
		//	std::unique_lock<std::mutex> lck(ioMtx);
		//	std::cout << "income socket total " << cTotal << " living " << cLiving << std::endl;
		//}

	}

	static void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *address, int socklen, void *ctx)
	{
		//{
		//	std::unique_lock<std::mutex> lck(ioMtx); 
		//	std::cout << "libevent fd " << fd << " accept" << std::endl;
		//}
		struct event_base *base = evconnlistener_get_base(listener);
		struct bufferevent *bev = bufferevent_socket_new( base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);

		// bev在session有引用
		// 如果不提前incref 可能会出现session创建前bev就销毁 会崩溃
		bufferevent_incref(bev);

		SocketInfo *info1 = nullptr;
		info1 = new SocketInfo(cTotal);

		bufferevent_setcb(bev, socket_read_cb, NULL, socket_event_cb, info1);
		bufferevent_enable(bev, EV_READ | EV_WRITE);

		{
			std::unique_lock<std::mutex> lck(mtxEventsOUT);
			eventsOUT.push_back(std::make_tuple(fd, Event::SocketCreate, bev, fd));
		}

		cTotal++;
		cLiving++;
		//{
		//	std::unique_lock<std::mutex> lck(ioMtx); 
		//	std::cout << "income socket total " << cTotal << " living " << cLiving<< std::endl;
		//}
	}

	static void accept_error_cb(struct evconnlistener *listener, void *ctx)
	{
		struct event_base *base = evconnlistener_get_base(listener);
		int err = EVUTIL_SOCKET_ERROR();
		{
			std::unique_lock<std::mutex> lck(ioMtx);
			std::cout << "Got an error " << err << " (" << evutil_socket_error_to_string(err) <<") on the listener. " "Shutting down." << std::endl;
		}
		Close();
	}


	int Listen(int port) {
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = htonl(0);
		sin.sin_port = htons(port);

		struct evconnlistener *listener = evconnlistener_new_bind(base, accept_conn_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&sin, sizeof(sin));
		if (!listener) {
			perror("Couldn't create listener");
			return 1;
		}
		evconnlistener_set_error_cb(listener, accept_error_cb);

		return 0;
	}

	int Connect(char * ip, int port)
	{
		return 0;
	}

	void Run() {
		int result = event_base_loop(base, EVLOOP_NO_EXIT_ON_EMPTY);
		{
			std::unique_lock<std::mutex> lck(ioMtx); 
			std::cout << "LIBEVENT " << "event_base_loop exit. result " << result << std::endl;
		}
		perror("LIBEVENT msg");
	}

	void Close() {

		{
			std::unique_lock<std::mutex> lck(ioMtx);
			std::cout << "LIBEVENT " << __FUNCTION__ << std::endl;
			std::cout << __FUNCTION__ << std::endl;
		}
#ifdef WIN32
		WSACleanup();
#endif
		if (base) {
			event_base_loopexit(base, NULL);
			base = nullptr;
		}
	}
}
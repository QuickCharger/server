//#include <stdio.h>
//#include "base.h"
//#include "event2/event.h"  // libevent core
//#include "minidump.h"
//
//int main(int argc, char* argv[])
//{
//	InitMinDump();
//	InitLog(argv);
//	InitNet();
//
//
//
//	CloseLog();
//	CloseNet();
//
//	return 0;
//}


////定时器示例
//#include <iostream>
//#include <event.h>
//#include <ctime>
//
//void callback(evutil_socket_t fd, short what, void *arg)
//{
//	printf("event ocurrence every 2 seconds.\n");
//	timeval *pTime = (timeval*)&arg;
//	printf("sec: %d. mil:%d\n", pTime->tv_sec, pTime->tv_usec);
//	printf("%s\n", (char*)arg + sizeof(timeval));
//	printf("\n");
//}
//
//int main()
//{
//	WSADATA wsa_data;
//	WSAStartup(MAKEWORD(2, 2), &wsa_data);
//
//	timeval two_sec = { 2, 0 };
//	event_base *base = event_base_new();
//
//	char *pCh = "this is a test message";
//	char* pVoid = new char[sizeof(timeval) + strlen(pCh) + 1];
//	memset(pVoid, 0, sizeof(timeval) + strlen(pCh) + 1);
//	memcpy(pVoid, (void*)&two_sec, sizeof(timeval));
//	memcpy(pVoid + sizeof(timeval), pCh, strlen(pCh) + 1);
//
//	//EV_PERSIST | EV_TIMEOUT 定时重复
//	//event_new()出来的event生命期Libevent处理
//	event *timeout = event_new(base, -1, EV_PERSIST | EV_TIMEOUT, callback, pVoid);
//	event_add(timeout, &two_sec);
//	event_base_dispatch(base);
//
//	WSACleanup();
//	event_base_free(base);
//
//	getchar();
//	return 0;
//}

//
////第二个定时器示例，使用多个定时器
//#include <ctime>
//#include <iostream>
//#include <event.h>
//#include <event_struct.h>
//
//#define TIMECOUNT 10
//#define BUFLEN 100
//
//void callback(evutil_socket_t fd, short what, void *arg)
//{
//	printf("%s\n", (char*)arg);
//}
//
//int main(int argc, char **argv)
//{
//	event_base *pBase = event_base_new();
//	event evTimeout[TIMECOUNT];
//	timeval tTime;
//	evutil_timerclear(&tTime);
//	for (int i = 0; i < TIMECOUNT; ++i)
//	{
//		char bufTmp[BUFLEN] = { 0 };
//		char *buf = new char[BUFLEN];
//		memset(buf, 0, BUFLEN);
//		sprintf(bufTmp, "taskID %d", i + 1);
//		memcpy(buf, bufTmp, BUFLEN);
//		//event_assign()对应的event生命期用户处理
//		event_assign(evTimeout + i, pBase, -1, 0, callback, (void*)buf);
//		tTime.tv_sec = i;
//		event_add(evTimeout + i, &tTime);
//	}
//	event_base_dispatch(pBase);
//
//	printf("dispatch OVER\n");
//	event_base_free(pBase);
//
//	getchar();
//	return 0;
//}



////第三个定时器示例，简化使用方法，函数展开后近似于第二种写法，event_base使用模式方式
//#include <iostream>
//#include "event.h"
//#include "event2/event_compat.h"
//#include "event2/event_struct.h"
//
//using namespace std;
//
//void CallBack(int sock, short event, void* arg)
//{
//	cout << "OVER" << endl;
//	struct timeval tv;
//	tv.tv_sec = 1;
//	tv.tv_usec = 0;
//	//重新添加定时器时间，原本定时器触发后默认自动删除
//	event_add((struct event*)arg, &tv);
//}
//
//int main()
//{
//	event_init();
//	struct event evTime;
//	evtimer_set(&evTime, CallBack, &evTime);
//
//	struct timeval tv;
//	tv.tv_sec = 1;
//	tv.tv_usec = 0;
//	event_add(&evTime, &tv);
//	event_dispatch();
//
//	return 0;
//}







////网络示例
//#include <assert.h>
//#include <ctime>
//#include <iostream>
//#include <fcntl.h>
//#include <event.h>
//#include <event_struct.h>
//#include "event2/event_compat.h"
//
//#pragma comment(lib, "ws2_32.lib")
//
//using namespace std;
//
//unsigned int sockSrv;
//struct event_base *g_pEventBase;
//
//void onRead(int a_nClientFD, short a_nEvent, void* a_pArg)
//{
//	int nLen;
//	char buf[1500];
//	nLen = recv(a_nClientFD, buf, 1500, 0);
//	if (nLen <= 0)
//	{
//		cout << "Client Close" << endl;
//		event* pEvRead = (event*)a_pArg;
//		event_del(pEvRead);
//		delete pEvRead;
//		closesocket(a_nClientFD);
//		return;
//	}
//
//	buf[nLen] = 0;
//	cout << "Client Info: " << buf << endl;
//}
//
//void onAccept(int a_nClientFD, short a_nEvent, void *a_pArg)
//{
//	SOCKADDR_IN ClientAddr;
//	int len = sizeof(SOCKADDR);
//	SOCKET sockConn = accept(a_nClientFD, (SOCKADDR*)&ClientAddr, &len);
//	cout << "New Connect" << endl;
//
//	//连接注册为新事件(EV_PERSIST)为事件触发后不默认删除
//	event *pEvRead = new event;
//	event_set(pEvRead, sockConn, EV_READ | EV_PERSIST, onRead, pEvRead);
//	event_base_set(g_pEventBase, pEvRead);
//	event_add(pEvRead, nullptr);
//}
//
//int main()
//{
//	WSADATA wsa_data;
//	WSAStartup(MAKEWORD(2, 2), &wsa_data);
//
//	sockSrv = socket(AF_INET, SOCK_STREAM, 0);
//
//	SOCKADDR_IN addrSrv;
//	memset(&addrSrv, 0, sizeof(SOCKADDR_IN));
//	addrSrv.sin_family = AF_INET;
//	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//	addrSrv.sin_port = htons(8888);
//
//	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == -1)
//		assert(0);
//
//	if (listen(sockSrv, 10) == -1)
//		assert(0);
//	
//	g_pEventBase = event_base_new();
//
//	//EV_TIMEOUT	超时
//	//EV_READ		只要网络缓冲中还有数据，回调就会被触发
//	//EV_WRITE		只要塞给网络缓冲的数据被写完，回调就会被触发
//	//EV_SIGNAL		POSIX SIGNAL
//	//EV_PRESIST	如果不加，回调触发后会被删除
//	//EV_ET			边缘触发，参考EPOLL_ET
//	event *evListen = event_new(g_pEventBase, sockSrv, EV_READ | EV_PERSIST, onAccept, nullptr);
//	//上面一行和下面三行相同结果，不同方式在于上面的方法最后一个参数不能写event_new的返回值，下面的方法可以达到目的。
//	//struct event evListen;
//	//event_set(&evListen, sockSrv, EV_READ | EV_PERSIST, onAccept, nullptr);
//	//event_base_set(g_pEventBase, &evListen);
//
//	if (event_add(evListen, nullptr) == -1)
//		assert(0);
//
//	event_base_dispatch(g_pEventBase);
//
//	return 0;
//}




//网络示例2
#include <assert.h>
#include <ctime>
#include <iostream>
#include <fcntl.h>
#include "commonInclude.h"
#include "CClientManager.h"

using namespace std;

#define PORT 8880

std::vector<pair<int, void*>> g_vPORT;

unsigned int sockSrv;
struct event_base *g_pEventBase;
CClientManager *g_pClientManager;

void onAccept(int a_nClientFD, short a_nEvent, void *a_pArg);
void onReadCB(bufferevent *a_pBev, void *a_pArg);
void onWriteCB(bufferevent *a_pBen, void *a_pArg);
void onErrorCB(bufferevent *a_pBen, short a_nEvent, void *a_pArg);

int main(int argc, char *argv[])
{
	InitMinDump();
	InitNet();

	//for (int i = 0; i < MAXLINK; ++i)
	//{
	//	g_vPORT.push_back(make_pair(i, nullptr));
	//}

	g_pClientManager = new CClientManager;

	sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	evutil_make_listen_socket_reuseable(sockSrv);

	SOCKADDR_IN addrSrv;
	memset(&addrSrv, 0, sizeof(SOCKADDR_IN));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_port = htons(PORT);

	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == -1)
	{
		cout << "bind error";
		getchar();
		return -1;
	}

	if (listen(sockSrv, 10) == -1)
	{
		cout << "listen error";
		getchar();
		return -1;
	}

	evutil_make_socket_nonblocking(sockSrv);

	event_base *pEventBase = event_base_new();

	event *evListen = event_new(pEventBase, sockSrv, EV_READ | EV_PERSIST, onAccept, (void*)pEventBase);

	event_add(evListen, nullptr);

	event_base_dispatch(pEventBase);

	cout << "PROGRAM FINISH" << endl << "press any key to exit" << endl;
	getchar();

	CloseNet();

	return 0;
}

void onAccept(int a_nClientFD, short a_nEvent, void *a_pArg)
{
	SOCKADDR_IN ClientAddr;
	int len = sizeof(SOCKADDR);
	SOCKET sockConn = accept(a_nClientFD, (SOCKADDR*)&ClientAddr, &len);
	if (sockConn > 0)
	{
		cout << "New Connect Accept Success" << endl;
	}
	else
	{
		cout << "New Connect Accept Failed!!!" << endl;
		perror("");
		return;
	}

	if (g_pClientManager->AddPlayer(sockConn))
	{
		struct bufferevent *pBufferEvent = bufferevent_socket_new((event_base*)a_pArg, sockConn, BEV_OPT_CLOSE_ON_FREE);;
		bufferevent_setcb(pBufferEvent, onReadCB, onWriteCB, onErrorCB, a_pArg);
		bufferevent_enable(pBufferEvent, EV_READ | EV_WRITE | EV_PERSIST);
	}
	else
	{
		//close socket, unfinish
	}
}

void onReadCB(bufferevent *a_pBev, void *a_pArg)
{
#define MAX_LINE 256
	char line[MAX_LINE];
	int n;
	evutil_socket_t fd = bufferevent_getfd(a_pBev);
	while (n = bufferevent_read(a_pBev, line, MAX_LINE), n > 0)
	{
		line[n] = '\0';
		printf("fd=%u, read line: %s\n", fd, line);
		bufferevent_write(a_pBev, line, n);
	}
}

void onWriteCB(bufferevent *a_pBen, void *a_pArg)
{
	printf("write over\n");
}

void onErrorCB(bufferevent *a_pBen, short a_nEvent, void *a_pArg)
{
	evutil_socket_t fd = bufferevent_getfd(a_pBen);
	printf("fd=%u ", fd);
	if (a_nEvent & BEV_EVENT_TIMEOUT)
	{
		printf("Time out\n");
	}
	else if (a_nEvent & BEV_EVENT_EOF)
	{
		printf("connection closed\n");
	}
	else if (a_nEvent & BEV_EVENT_ERROR)
	{
		printf("Unknown error\n");
	}
	bufferevent_free(a_pBen);
}

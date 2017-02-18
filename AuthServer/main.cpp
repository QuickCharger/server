#include <assert.h>
#include <ctime>
#include <iostream>
#include <fcntl.h>
#include "commonInclude.h"

using namespace std;

unsigned int sockSrv;
//struct  *g_pEventBase;

#define PORT 8880
#define MAXLINK 1024

std::vector<pair<int, void*>> g_vPORT;

void onAccept(int a_nClientFD, short a_nEvent, void *a_pArg);
void onReadCB(bufferevent *a_pBev, void *a_pArg);
void onWriteCB(bufferevent *a_pBen, void *a_pArg);
void onErrorCB(bufferevent *a_pBen, short a_nEvent, void *a_pArg);

int main(int argc, char *argv[])
{
	InitMinDump();
	InitNet();

	for (int i = 0; i < MAXLINK; ++i)
	{
		g_vPORT.push_back(make_pair(i, nullptr));
	}

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
		cout << "New Connect Accept Success";
	}
	else
	{
		cout << "New Connect Accept Failed!!!";
		return;
	}

	struct bufferevent *pBufferEvent = bufferevent_socket_new((event_base*)a_pArg, sockConn, BEV_OPT_CLOSE_ON_FREE);;
	bufferevent_setcb(pBufferEvent, onReadCB, onWriteCB, onErrorCB, a_pArg);
	bufferevent_enable(pBufferEvent, EV_READ | EV_WRITE | EV_PERSIST);
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
//		bufferevent_write(a_pBev, line, n);
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
		cout << "socket Time out";
	}
	else if (a_nEvent & BEV_EVENT_EOF)
	{
		cout << "connection closed";
	}
	else if (a_nEvent & BEV_EVENT_ERROR)
	{
		cout << "Unknown error\n";
	}
	bufferevent_free(a_pBen);
}

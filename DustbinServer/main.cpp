#include <assert.h>
#include <ctime>
#include <iostream>
#include <fcntl.h>
#include "base.h"
#include "commonInclude.h"

using namespace std;

#define PORT 8880
#define MAXLINK 5

map<std::string, std::string> mDefaultValue;	//cfg文件中的默认值
map<std::string, std::pair<std::string, int>> mServer;	//<serverName, <serverIP, Port>>	cfg文件中的要连接的服务器

void OneSecCB(int a_nClientFD, short a_nEvent, void *a_pArg);
void onAccept(int a_nClientFD, short a_nEvent, void *a_pArg);
void onReadCB(bufferevent *a_pBev, void *a_pArg);
void onWriteCB(bufferevent *a_pBen, void *a_pArg);
void onErrorCB(bufferevent *a_pBen, short a_nEvent, void *a_pArg);
void ConnectServer(std::pair<std::string, int>& serverConfig, SOCKET& socket);

int main(int argc, char *argv[])
{
	InitMinDump();
	InitNet();

	std::string strConfigName = Func::Truncate(argv[0], '.') + ".cfg";
	if (strConfigName.size() < 4)
	{
		std::cout << "Can not parse file name." << endl << "exit" << endl;
		getchar();
		exit(2);
	}
	std::string strContext;
	ReadFile(strConfigName.c_str(), strContext);
	if (strContext.size() == 0)
	{
		std::cout << GetLastErr();
		exit(3);
	}

	std::vector<std::string > vSplit;
	Func::SplitString(strContext, "\n", vSplit);

	for (int i = 0; i < vSplit.size(); ++i)
	{
		std::cout << "Line: " << i << " " << vSplit[i] << endl;
	}

	InitConfig(vSplit, mDefaultValue, mServer);

	for (auto it = mDefaultValue.begin(); it != mDefaultValue.end(); ++it)
	{
		std::cout << "Type: " << it->first << "\tValue: " << it->second << endl;
	}
	for (auto it = mServer.begin(); it != mServer.end(); ++it)
	{
		std::cout << "ServerName: " << it->first << "\t IP: " << it->second.first << "\t PORT: " << it->second.second << endl;
	}

	//bufferevent_socket_new();

	evutil_socket_t sockSrv = socket(AF_INET, SOCK_STREAM, 0);
	evutil_make_socket_nonblocking(sockSrv);
#ifndef WIN32
	evutil_make_listen_socket_reuseable(sockSrv);	//设定接口可重用,Win上不可用
#endif

	SOCKADDR_IN addrSrv;
	std::memset(&addrSrv, 0, sizeof(SOCKADDR_IN));
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_port = htons(PORT);

	if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == -1)
	{
		std::cout << "bind error" << endl;
		getchar();
		return -1;
	}

	if (listen(sockSrv, 10) == -1)
	{
		std::cout << "listen error" << endl;
		getchar();
		return -1;
	}

	//evutil_make_socket_nonblocking(sockSrv);	//设定接口非阻塞

	event_base *pEventBase = event_base_new();

	//evtimer_new();
	//evtimer_add();
	event *evListen = event_new(pEventBase, sockSrv, EV_READ | EV_PERSIST, onAccept, (void*)pEventBase);
	event *evListen2 = event_new(pEventBase, -1, 0, OneSecCB, (void*)pEventBase);

	event_add(evListen, nullptr);

	timeval tv;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	event_add(evListen2, &tv);

	event_base_dispatch(pEventBase);

	std::cout << "PROGRAM FINISH" << endl << "press any key to exit" << endl;
	getchar();

	CloseNet();

	return 0;
}

void OneSecCB(int a_nClientFD, short a_nEvent, void *a_pArg)
{
	event_base* pEventBase = (event_base*)a_pArg;
	std::list<SOCKET> lSockServer;
	for (auto it = mServer.begin(); it != mServer.end(); ++it)
	{
		SOCKET sockClient;
		lSockServer.push_back(sockClient);
		//evutil_make_socket_nonblocking(sockClient);
		ConnectServer(it->second, sockClient);
		struct bufferevent *pBufferEvent = bufferevent_socket_new(pEventBase, sockClient, BEV_OPT_CLOSE_ON_FREE);
		bufferevent_setcb(pBufferEvent, onReadCB, onWriteCB, onErrorCB, nullptr);
		bufferevent_enable(pBufferEvent, EV_READ | EV_WRITE | EV_PERSIST);
		std::cout << "fd:" << sockClient << ". IP:" << it->second.first << ". Port:" << it->second.second << endl;
	}
}

void onAccept(int a_nClientFD, short a_nEvent, void *a_pArg)
{
	SOCKADDR_IN ClientAddr;
	int len = sizeof(SOCKADDR);
	SOCKET sockConn = accept(a_nClientFD, (SOCKADDR*)&ClientAddr, &len);
	if (sockConn > 0)
	{
		std::cout << "New Connect Accept Success. socketID: " << sockConn << endl;
	}
	else
	{
		std::cout << "New Connect Accept Failed!!!" << endl;
		return;
	}

	//g_mPORT[sockConn] = ;

	struct bufferevent *pBufferEvent = bufferevent_socket_new((event_base*)a_pArg, sockConn, BEV_OPT_CLOSE_ON_FREE);;
	bufferevent_setcb(pBufferEvent, onReadCB, onWriteCB, onErrorCB, a_pArg);
	bufferevent_enable(pBufferEvent, EV_READ | EV_WRITE | EV_PERSIST);
}

void onReadCB(bufferevent *a_pBev, void *a_pArg)
{
#define MAX_LINE 10240
	char line[MAX_LINE];
	static int nRecv = 0;
	static time_t tBegin = 0;
	static time_t tEnd = 0;
	int n;
	evutil_socket_t fd = bufferevent_getfd(a_pBev);
	while (n = bufferevent_read(a_pBev, line, MAX_LINE), n > 0)
	{
		if (tBegin == 0)
			tBegin = time(nullptr);
		tEnd = time(nullptr);
		nRecv += n;
		line[n] = '\0';
		//printf("fd=%u, read line: %s\n", fd, line);
		//printf("Recv: %d\n", nRecv);
		bufferevent_write(a_pBev, line, n);
	}
}

void onWriteCB(bufferevent *a_pBen, void *a_pArg)
{
	//printf("fd: %d. write over\n", bufferevent_getfd(a_pBen));
	//bufferevent_write(a_pBen, "qwer", 4);
}

void onErrorCB(bufferevent *a_pBen, short a_nEvent, void *a_pArg)
{
	evutil_socket_t fd = bufferevent_getfd(a_pBen);
	printf("fd=%u ", fd);
	if (a_nEvent & BEV_EVENT_TIMEOUT)
	{
		std::cout << "socket Time out" << endl;
	}
	else if (a_nEvent & BEV_EVENT_EOF)
	{
		std::cout << "connection closed" << endl;
	}
	else if (a_nEvent & BEV_EVENT_ERROR)
	{
		std::cout << "Unknown error\n" << endl;
	}
	bufferevent_free(a_pBen);
}

void ConnectServer(std::pair<std::string, int>& serverConfig, SOCKET& a_Socket)
{
	a_Socket = socket(AF_INET, SOCK_STREAM, 0);
	evutil_make_socket_nonblocking(a_Socket);
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(serverConfig.first.c_str());
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(serverConfig.second);
	int nResult = connect(a_Socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	cout << "ConnectServer: " << serverConfig.first << ":" << serverConfig.second << ". Result:" << nResult << endl;
}

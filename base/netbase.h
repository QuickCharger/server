#ifndef _NETBASE_H_
#define _NETBASE_H_

#ifdef WIN32
#include <WinSock2.h>

void InitNet()
{
	WSADATA wsa_data;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
}

void CloseNet()
{
	WSACleanup();
}
#endif

#endif

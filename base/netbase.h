#ifndef _NETBASE_H_
#define _NETBASE_H_

#ifdef WIN32
#include <WinSock2.h>

void InitNet()
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(MAKEWORD(2, 2), &wsa_data);
#endif
}

void CloseNet()
{
#ifdef WIN32
	WSACleanup();
#endif
}
#endif

#endif

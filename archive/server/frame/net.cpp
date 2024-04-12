#include "net.h"

#ifdef WIN32
#include <WinSock2.h>
#endif

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
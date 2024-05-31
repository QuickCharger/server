#include "Work.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <csignal>
#include "./frame/Libevent.h"
#include "Client.h"

Work *gWork = new Work;
int port = 12345;

Work::Work()
{
}

Work::~Work()
{
	// do nothing
}

int Work::OnInit()
{
	this->config.port = port;
	return 0;
}

int Work::OnRun()
{
	std::cout << __FUNCTION__ << std::endl;
	return 0;
}

int Work::OnStop()
{
	std::cout << __FUNCTION__ << std::endl;
	return 0;
}

int Work::OnNet_SocketAccept(long long uid)
{
	Client::CreateClient(uid);
	return 0;
}

int Work::OnNet_DataIn(long long uid, char* ch, int len)
{
	auto c = gClients.find(uid);
	assert(c != gClients.end());
	if (c != gClients.end())
	{
		gClients[uid]->OnMsg(ch, len);
	}
	else
	{
		delete[]ch;
	}
	return 0;
}

int Work::OnNet_Err(long long uid)
{
	return 0;
}

int Work::OnNet_Close(long long uid)
{
	Client::DestroyClient(uid);
	return 0;
}











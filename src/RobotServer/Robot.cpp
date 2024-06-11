#include "Robot.h"
#include "Work.h"

std::map<long long, Robot*> gRobots;
Robot::Robot()
{
}

Robot::~Robot()
{
}

void Robot::OnConnectSuccess()
{
	// 链接成功了， 什么都不用做， 也许输出个日志？
}

void Robot::OnDataIn(char*p, int len)
{
	// 机器人收到消息什么也不做 目前机器人只发收消息 并统计字数
}

void Robot::OnErr()
{
	// 链接出错了， 什么都不用做， 也许输出个日志？
	// 肯定会调用OnClose
}

void Robot::OnClose()
{
	if (this->reConnect)
	{
		gWork->AddTimer(this->reConnectIntervalSec * 1000, this, nullptr, TimerType::eReconnect, 0, 1);
	}
}

int Robot::Send(char *p, int len)
{
	gWork->ProductMsg(uid, Event::Send, p, nullptr, len);
	return 1;
}

void Robot::DoReconnect(const std::string& ip, int port)
{
	gWork->ProductMsg(uid, Event::ConnectTo, nullptr, nullptr, port, 0, ip);

	//this->Desc(__FUNCTION__);
}

int Robot::DoLogout()
{
	gWork->ProductMsg(uid, Event::TryClose);
	return 0;
}

void Robot::Desc(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::vfprintf(stdout, format, args);
	va_end(args);
	std::cout << "; uid " << this->uid;
	std::cout << std::endl;
}

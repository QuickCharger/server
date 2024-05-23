#include "Robot.h"
#include "Work.h"

std::map<long long, Robot*> gRobots;
Robot::Robot()
{
}

Robot::~Robot()
{
	if (m_session)
	{
		delete m_session;
		m_session = nullptr;
	}
}

void Robot::OnEvent(Event &e)
{
	if (e.e == Event::RegBufferEvent)
	{
		if (this->m_session)
		{
			delete this->m_session;
			this->m_session = nullptr;
		}
		if (e.p1)
		{
			bufferevent* ev = (bufferevent*)e.p1;
			this->m_session = new Session(ev);
		}
	}
	else if (e.e == Event::SocketConnectErr || e.e == Event::SocketErr)
	{
		bufferevent* ev = (bufferevent*)e.p1;
		if (this->reConnect)
		{
			gWork->AddTimer(this->reConnectIntervalSec * 1000, this, nullptr, TimerType::eReconnect, 0, 1);
		}
	}
	else if (e.e == Event::SocketConnectSuccess)
	{
		e.uid;
		// 链接成功了， 什么都不用做， 也许输出个日志？
	}
}

void Robot::OnMsg(void *p, int len)
{
	// 机器人收到消息什么也不做 目前机器人只发收消息 并统计字数
	char *pch = (char*)p;
	delete pch;
}

int Robot::Send(char *p, int len)
{
	if (m_session == nullptr || !m_session->Working())
		return 0;
	m_session->Send(p, len);
	return 1;
}

void Robot::DoReconnect(const std::string& ip, int port)
{
	Event e;
	e.e = Event::SocketConnectTo;
	e.uid = uid;
	e.str1 = ip;
	e.i1 = port;
	gWork->AddEvent(std::move(e));

	//this->Desc(__FUNCTION__);
}

int Robot::DoLogout()
{
	if (m_session && m_session->m_ev)
	{
		Event e;
		e.e = Event::SocketTryClose;
		e.p1 = m_session->m_ev;
		gWork->AddEvent(std::move(e));
	}
	return 0;
}

void Robot::Desc(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::vfprintf(stdout, format, args);
	va_end(args);
	std::cout << "; uid " << this->uid << ". session " << this->m_session;
	std::cout << std::endl;
}

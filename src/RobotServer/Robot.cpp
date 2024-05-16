#include "Robot.h"
#include "Work.h"

std::map<long long, Robot*> gRobots;
Robot::Robot()
{
}

Robot::~Robot()
{
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
		// ���ӳɹ��ˣ� ʲô���������� Ҳ���������־��
	}
}

void Robot::OnMsg(void *p, int len)
{
	// �������յ���ϢʲôҲ���� Ŀǰ������ֻ������Ϣ ��ͳ������
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

void Robot::Desc(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	std::vfprintf(stdout, format, args);
	va_end(args);
	std::cout << "; uid " << this->uid << ". session " << this->m_session;
	std::cout << std::endl;
}

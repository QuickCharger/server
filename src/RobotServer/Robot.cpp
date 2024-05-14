#include "Robot.h"

std::map<long long, Robot*> gRobots;
Robot::Robot()
{
}

Robot::~Robot()
{
}

void Robot::OnSession(Event &e)
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
	else if (e.e == Event::SocketConnectErr)
	{
		bufferevent* ev = (bufferevent*)e.p1;
		// ���� todo
	}
	else if (e.e == Event::SocketConnectSuccess)
	{
		e.uid;
		// ���ӳɹ��ˣ� ʲô���������� Ҳ���������־��
	}
}

void Robot::Desc()
{
	std::cout << "uid " << this->uid << ". session " << this->m_session << std::endl;
}

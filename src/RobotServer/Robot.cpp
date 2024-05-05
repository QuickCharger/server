#include "Robot.h"

std::map<int, Robot*> gRobots;
Robot::Robot() {

}

Robot::~Robot() {
}

void Robot::OnSession(EventStruct &e)
{
	if (e.e == Event::RegBufferEvent) {
		if (this->m_session)
		{
			delete this->m_session;
			this->m_session = nullptr;
		}
		if (e.bev)
		{
			this->m_session = new Session(e.bev);
		}
	}
	else if (e.e == Event::SocketConnectErr)
	{
		// ÖØÁ¬ todo
	}
	else if (e.e == Event::SocketConnectSuccess)
	{
		//
	}
}

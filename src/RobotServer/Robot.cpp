#include "Robot.h"

std::map<long long, Robot*> gRobots;
Robot::Robot() {

}

Robot::~Robot() {
}

void Robot::OnSession(Event &e)
{
	//if (e.e == Event::RegBufferEvent) {
	//	if (this->m_session)
	//	{
	//		delete this->m_session;
	//		this->m_session = nullptr;
	//	}
	//	if (e.bev)
	//	{
	//		this->m_session = new Session(e.bev);
	//	}
	//}
	//else if (e.e == Event::SocketConnectErr)
	//{
	//	// ÖØÁ¬ todo
	//}
	if (false)
	{
	}
	else if (e.e == Event::SocketConnectSuccess)
	{
		e.uid;
	}
}

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
	// ���ӳɹ��ˣ� ʲô���������� Ҳ���������־��
}

void Robot::OnDataIn(char*p, int len)
{
	// �������յ���ϢʲôҲ���� Ŀǰ������ֻ������Ϣ ��ͳ������
}

void Robot::OnErr()
{
	// ���ӳ����ˣ� ʲô���������� Ҳ���������־��
	// �϶������OnClose
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

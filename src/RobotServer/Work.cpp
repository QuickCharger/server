#include "Work.h"
#include "thread"
#include "mutex"
#include <csignal>
#include "./frame/Libevent.h"
#include "./frame/Session.h"
#include "Robot.h"

std::string remoteIp = "127.0.0.1";
int remotePort = 12346;

int cMaxRobot = 10;
int cCurRobot = 0;

enum TimerType {
	eAddRobot = 1,
};

Work *gWork = new Work;

Work::Work() {
	pEventTodo = new std::vector<Event>;
	RegThread(this);
}

Work::~Work() {
	// do nothing
}

int Work::Init() {
	net = new CLibevent;
	RegThread(net);

	CTimer::Init();

	AddTimer(1, nullptr, nullptr, eAddRobot, 0, -1);


	//auto now = std::chrono::system_clock::now();
	//long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	//TimerCBArg arg;
	//arg.i1 = 1;
	//arg.i2 = 2;
	//arg.p1 = (void*)1234;
	//arg.p2 = (void*)2345;
	//CTimer::AddTimer(milliseconds, 1000, std::bind(&Work::OnTimer, this, std::placeholders::_1), std::move(arg), 5);
	return 0;
}

int Work::Run() {
	while (true) {

		net->Consume(&pEventC);
		net->Product(&pEventP);

		auto now = std::chrono::system_clock::now();
		long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

		CTimer::Check(milliseconds);

		// 处理net
		for (auto it = pEventC->begin(); it != pEventC->end(); ++it)
		{
			long long uid = it->uid;
			Robot* r = gRobots.at(uid);
			if (it->e == Event::RegBufferEvent
				|| it->e == Event::SocketConnectErr
				|| it->e == Event::SocketConnectSuccess)
			{
				r->OnSession(*it);
			}
		}
		pEventC->clear();

		// 处理该线程逻辑
		//{
		//	// 处理个虚假的逻辑
		//	for (auto &it : gClients) {
		//		it.second->FakeNews();
		//	}
		//}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

int Work::Stop() {
	// todo
	return 0;
}

void Work::OnTimer(const TimerCBArg& arg) {
	//std::cout << __FUNCTION__ << " cur " << arg.cur << " i1 " << arg.i1 << " i2 " << arg.i2 << " p1 " << (int)arg.p1<< " p2 " << (int)arg.p2<< std::endl;
	int timerType = arg.i1;
	if (timerType == TimerType::eAddRobot) {
		addRobot();
	}
}

void Work::OnTimer1ms(const TimerCBArg& arg) {
	net->Consume(&pEventC);
	net->Product(&pEventP);
}

void Work::OnTimer1s(const TimerCBArg& arg) {
	std::cout << __FUNCTION__ << " cur " << arg.cur << std::endl;
}


// 上机器人
void Work::addRobot() {
	if (cCurRobot < cMaxRobot) {
		cCurRobot++;
		Robot* r = new Robot();
		r->uid = CLibevent::GenUid();
		gRobots[r->uid] = r;
		r->Desc();

		Event e;
		e.e = Event::SocketConnectTo;
		e.uid = r->uid;
		e.str1 = remoteIp;
		e.i1 = remotePort;
		pEventP->push_back(std::move(e));
	}

	// 重连 todo


}

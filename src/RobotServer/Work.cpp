#include "Work.h"
#include "thread"
#include "mutex"
#include <csignal>
#include "./frame/Libevent.h"
#include "./frame/Session.h"
#include "Robot.h"

std::string remoteIp = "127.0.0.1";
int remotePort = 12345;

Work *gWork = new Work;


// 目标客户量 / 实际客户量
int targetClientCount = 1000;
int clientCount = 0;

// 每次发送包的大小
int packLen = 1000 * 1;
int packInterval = 10;
//int sendTimes = -1;		// 发送次数 如果达到此值则socket销毁 默认-1不销毁
//std::string chunk;
char *chunk = nullptr;

// 统计
int cSend = 0;
int cRecv = 0;

// 运行时间 / 休息时间 秒
int runTime = 10;
int sleepTime = 3;
bool bSleepLogout = true;

bool working = false;


Work::Work() {

	chunk = new char[packLen];
	char *ch = "1234567890";
	for (int i = 0; i < packLen / 10; ++i)
	{
		memcpy(chunk + i * 10, ch, strlen(ch));
	}
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
	AddTimer(1000, nullptr, nullptr, eStateSend, 0, 1);
	AddTimer(packInterval, nullptr, nullptr, eDoSend, 0, -1);


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
			auto itRbots = gRobots.find(uid);
			if (itRbots == gRobots.end())
			{
				continue;
			}
			Robot* r = itRbots->second;
			if (it->e == Event::RegBufferEvent
				|| it->e == Event::SocketConnectErr
				|| it->e == Event::SocketConnectSuccess
				|| it->e == Event::SocketErr)
			{
				r->OnEvent(*it);
			}
			else if (it->e == Event::DataIn)
			{
				cRecv += it->i1;
				r->OnMsg(it->p1, it->i1);
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

void Work::AddEvent(Event&& e)
{
	pEventP->push_back(e);
}

void Work::OnTimer(const TimerCBArg& arg) {
	//std::cout << __FUNCTION__ << " cur " << arg.cur << " i1 " << arg.i1 << " i2 " << arg.i2 << " p1 " << (int)arg.p1<< " p2 " << (int)arg.p2<< std::endl;
	int timerType = arg.i1;
	if (timerType == TimerType::eAddRobot)
	{
		addRobot();
	}
	else if (timerType == TimerType::eReconnect)
	{
		Robot* r = (Robot*)arg.p1;
		r->DoReconnect(remoteIp, remotePort);
	}
	else if (timerType == TimerType::eStateSend)
	{
		working = true;
		cSend = 0;
		cRecv = 0;
		AddTimer(1000 * runTime, nullptr, nullptr, eStateStop, 0, 1);
	}
	else if (timerType == TimerType::eStateStop)
	{
		working = false;
		AddTimer(1000 * sleepTime, nullptr, nullptr, eStateSend, 0, 1);
		if (bSleepLogout)
		{
			delRobotAll();
		}
	}
	else if (timerType == TimerType::eDoSend)
	{
		//auto tnow = std::chrono::system_clock::now();
		//long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tnow.time_since_epoch()).count();
		//std::cout << milliseconds << std::endl;
		if (!working)
			return;
		for (auto it : gRobots)
		{
			int l = it.second->Send(chunk, packLen);
			if (l > 0)
			{
				cSend += packLen;
			}
		}
	}
	else if (timerType == TimerType::eDoLogout)
	{
		int rid = arg.i2;
		delRobot(rid);
	}
}

void Work::OnTimer1ms(const TimerCBArg& arg) {
	net->Consume(&pEventC);
	net->Product(&pEventP);
}

void Work::OnTimer1s(const TimerCBArg& arg) {
	//std::cout << __FUNCTION__ << " cur " << arg.cur << std::endl;
	int maxId = 0;
	for (auto it : gRobots)
	{
		if (it.second->uid > maxId)
		{
			maxId = (int)it.second->uid;
		}
	}
	std::cout << "diff " << cSend - cRecv << " send " << cSend << " recv " << cRecv << " robot size " << gRobots.size() << " robot max id " << maxId <<  std::endl;
}


// 上机器人
void Work::addRobot()
{
	if (clientCount < targetClientCount)
	{
		clientCount++;
		Robot* r = new Robot();
		r->uid = CLibevent::GenUid();
		r->reConnect = true;
		r->reConnectIntervalSec = 1;
		gRobots[r->uid] = r;
		//r->Desc(__FUNCTION__);
		r->DoReconnect(remoteIp, remotePort);
	}
}

void Work::delRobot(long long rid)
{
	gRobots[rid]->DoLogout();
	delete gRobots[rid];
	gRobots.erase(rid);
	clientCount--;
}

void Work::delRobotAll()
{
	for (auto it : gRobots)
	{
		// 此处robotid可能截断 假设暂时不会发生
		AddTimer(1, nullptr, nullptr, TimerType::eDoLogout, (int)it.first);
	}
}

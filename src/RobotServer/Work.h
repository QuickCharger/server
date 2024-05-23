#pragma  once

#include "./frame/common.h"
#include "./frame/IRunnable.h"
#include "./frame/Libevent.h"
#include "./frame/ProductConsume.h"

#include "./frame/Timer.h"

enum TimerType {
	eAddRobot = 1,
	eConnect,
	eReconnect,
	eStateSend,		// 发送数据
	eStateStop,		// 停止发送数据
	eDoSend,		// 尝试发送数据
	eDoLogout,
};

class Work : public IRunnable, public CTimer
{
public:
	Work();
	~Work();
	int Init();
	int Run();
	int Stop();

	void AddEvent(Event&& e);

	//void AddTimer(int waitms, void* p1, void* p2, int i1, int i2, int loopTimes = 1);
	void OnTimer(const TimerCBArg&);
	void OnTimer1ms(const TimerCBArg&);
	void OnTimer1s(const TimerCBArg&);

	void addRobot();
	void delRobot(long long);
	void delRobotAll();

private:
	std::vector<Event>* pEventC = nullptr;
	std::vector<Event>* pEventP = nullptr;

	CLibevent *net = nullptr;
};

extern Work *gWork;

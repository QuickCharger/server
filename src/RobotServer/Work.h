#pragma  once
#pragma  once

#include "./frame/common.h"
#include "./frame/Runnable.h"
#include "./frame/ProductConsume.h"

enum TimerType {
	eAddRobot = 1,
	eConnect,
	eReconnect,
	eStateSend,		// 发送数据
	eStateStop,		// 停止发送数据
	eDoSend,		// 尝试发送数据
	eDoLogout,
};

class Work : public Runnable
{
public:
	Work();
	~Work();

	void addRobot();
	void delRobot(long long);
	void delRobotAll();





	////////////////////////////////////////

	virtual int OnInit();
	virtual int OnRun();
	virtual int OnStop();

	virtual void OnTimer(const TimerCBArg&);
	virtual void OnTimer1s(const TimerCBArg&);
	virtual void OnTimer10s(const TimerCBArg&);

	virtual int OnNet_ConnectSuccess(long long uid);
	virtual int OnNet_DataIn(long long uid, char* ch, int len);
	virtual int OnNet_Err(long long uid);
	virtual int OnNet_Close(long long uid);

private:
	std::vector<Event>* pEventC = nullptr;
	std::vector<Event>* pEventP = nullptr;

	CLibevent *net = nullptr;
};

extern Work *gWork;

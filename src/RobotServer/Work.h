#pragma  once

#include "./frame/common.h"
#include "./frame/IRunnable.h"
#include "./frame/ProductConsume.h"

#include "./frame/Timer.h"

class Work : public IRunnable, public CTimer
{
public:
	Work();
	~Work();
	int Init();
	int Run();
	int Stop();

	void OnTimer(const TimerCBArg& );
	void OnTimer1ms(const TimerCBArg&);
	void OnTimer1s(const TimerCBArg& );

private:
	std::vector<EventStruct>* pEventC = nullptr;
	std::vector<EventStruct>* pEventP = nullptr;

	CLibevent *net = nullptr;
};

extern Work *work;

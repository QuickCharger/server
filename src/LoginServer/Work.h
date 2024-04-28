#pragma  once

#include "./frame/common.h"
#include "./frame/IRunnable.cc"
#include "./frame/ProductConsume.cc"

class Work : IRunnable
{
public:
	Work();
	~Work();
	int Init();
	int Run();
	int Stop();

private:
	std::vector<EventStruct>* pEventC = nullptr;
	std::vector<EventStruct>* pEventP = nullptr;

	CLibevent *net = nullptr;
};

extern Work *work;

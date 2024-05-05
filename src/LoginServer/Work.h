#pragma  once

#include "./frame/common.h"
#include "./frame/IRunnable.h"
#include "./frame/ProductConsume.h"

class Work : IRunnable
{
public:
	Work();
	~Work();
	int Init();
	int Run();
	int Stop();

private:
	std::vector<Event>* pEventC = nullptr;
	std::vector<Event>* pEventP = nullptr;

	CLibevent *net = nullptr;
};

extern Work *work;

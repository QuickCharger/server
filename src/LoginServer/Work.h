#pragma  once

#include "./frame/common.h"
#include "./frame/Runnable.h"
#include "./frame/ProductConsume.h"

class Work : public Runnable
{
public:
	Work();
	~Work();

	virtual int OnInit();
	virtual int OnRun();
	virtual int OnStop();

	virtual int OnNet_SocketAccept(long long uid);
	virtual int OnNet_DataIn(long long uid, char* ch, int len);
	virtual int OnNet_Err(long long uid);
	virtual int OnNet_Close(long long uid);

	virtual void OnTimer1s(const TimerCBArg& arg)
	{
		//std::cout << __FUNCTION__ << std::endl;
	}
	virtual void OnTimer10s(const TimerCBArg&)
	{
		po->Desc();
	}
};

extern Work *gWork;

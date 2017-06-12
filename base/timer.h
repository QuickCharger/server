#ifndef _BASE_TIMER_H_
#define _BASE_TIMER_H_

#include <functional>
#include <event2/bufferevent.h>
#include <event2/event.h>


template<class T>
class CTimer
{
	typedef std::function< void(T* t, void*)> FuncCB;

public:
	CTimer(event_base* a_pEventBase)
		: m_pEventBase(a_pEventBase)
	{

	}

	virtual ~CTimer()
	{

	}

public:
	void AddTimer(int a_nSec, int a_nMillSec, FuncCB a_nFunc, void *a_pArg)
	{
		event* evListen2 = evtimer_new(m_pEventBase,
			[](int, short, void *a_pArg)
		{
			a_nFunc(a_pArg);
		},
			this);

		timeval tv;
		tv.tv_sec = a_nSec;
		tv.tv_usec = a_nMillSec;
		int nResult = evtimer_add(evListen2, &tv);
	}

private:
	event_base* m_pEventBase = nullptr;
};

#endif

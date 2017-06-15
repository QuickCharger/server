#ifndef _BASE_TIMER_H_
#define _BASE_TIMER_H_

#include <iostream>

#include <functional>
#include <event2/bufferevent.h>
#include <event2/event.h>

template<typename T>
class CTimer
{
	typedef std::function< void(void*)> FuncCB;

	struct TimerParam
	{
		TimerParam(void *a_pT, int a_nMillSecond, int a_nCount, FuncCB a_pCB, void *a_pArg)
			: t(a_pT)
			, count(a_nCount)
			, cb(a_pCB)
			, arg(a_pArg)
		{
			tv.tv_sec = a_nMillSecond/1000;
			tv.tv_usec = a_nMillSecond%1000;
		}
		void *t;
		struct event *ev;
		int count;
		FuncCB cb;
		void *arg;
		struct timeval tv;
	};

public:
	CTimer()
	{

	}

	virtual ~CTimer()
	{

	}

public:
	void InitTimer(event_base* a_pEventBase)
	{
		m_pEventBase = a_pEventBase;
	}
	void AddTimer(int a_nMillSec, FuncCB a_nFunc, void *a_pArg, int a_nCount = 1)
	{
		if (a_nMillSec < 1000)
		{
			LOG(WARNING) << "AddTimer. a_nMillSec " << a_nMillSec << " maybe too short ?";
		}
		TimerParam *pParam = new TimerParam(this, a_nMillSec, a_nCount, a_nFunc, a_pArg);

		event* evListen2 = evtimer_new(m_pEventBase,
			[](evutil_socket_t fd, short event, void *a_pArg)
		{
			TimerParam *pParam = static_cast<TimerParam *>(a_pArg);
			T* t = static_cast<T*>(pParam->t);
			t->TimeoutCB(fd, event, a_pArg);
		},
			pParam);
		pParam->ev = evListen2;
		evtimer_add(pParam->ev, &pParam->tv);
	}

	void TimeoutCB(int fd, short event, void* a_pArg)
	{
		TimerParam *pParam = static_cast<TimerParam *>(a_pArg);
		(pParam->cb)(pParam->arg);
		pParam->count--;
		if (pParam->count != 0)
		{
			evtimer_add(pParam->ev, &pParam->tv);
		}
		else
		{
			delete pParam;
		}
	}

private:
	event_base* m_pEventBase = nullptr;
};

#endif

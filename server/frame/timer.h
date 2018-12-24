#pragma once

#include <functional>
#include <map>
#include <iostream>

//#include <event2/bufferevent.h>
//#include <event2/event.h>

#include "Libevent.h"
#include "log.h"

timeval GetTimeval(int a_sec, int a_usec = 0);

template<typename T>
class CTimer
{
	typedef void (T::*timerCB)(void*);
	struct STimer{
		event *ev;
		unsigned int sec;
		void* target;		// target::cb
		timerCB cb;
		void *param;
		int times;
	};

public:
	virtual ~CTimer()
	{
		for (auto it = m_mTimer.begin(); it != m_mTimer.end(); ++it)
		{
			evtimer_del(it->first);
			delete it->second;
		}
		m_mTimer.clear();
	}
	//void InitTimer(event_base* a_pEventBase)
	//{
	//	m_pEventBase = a_pEventBase;
	//}

	event* AddTimer(unsigned int sec, void *t, timerCB cb, void * param = nullptr, int times = 1)
	{
		if (CLibevent::GetInstance() == nullptr)
		{
			LOG(ERROR) << "Timer not initialize.";
			return 0;
		}
		STimer *scb = new STimer;
		scb->ev = nullptr;
		scb->sec = sec;
		scb->target = t;
		scb->cb = cb;
		scb->param = param;
		scb->times = times;
		event *evListen = event_new(CLibevent::GetInstance(), -1, EV_PERSIST | EV_TIMEOUT,
			[](evutil_socket_t a_Socket, short a_nEvent, void *a_pArg) {
			STimer *scb = (STimer*)a_pArg;
			T * target = static_cast<T*>(scb->target);
			(target->*(scb->cb))(scb->param);
			//DLOG(INFO) << "timer times " << scb->times;
			DLOG(INFO) << "on  Timer sec " << scb->sec << "s" << ". ev " << scb->ev;
			if (scb->times >= 0 && --scb->times <= 0)
			{
				//DLOG(INFO) << "delete timer. event: ";
				//evtimer_del(scb->ev);
				//event_free();
				//delete scb;

				DLOG(INFO) << "del Timer sec " << scb->sec << "s" << ". ev " << scb->ev;
				target->DeleteTimer(scb->ev);
			}
		}
		, scb);
		scb->ev = evListen;

		if (!evListen)
		{
			delete scb;
			return 0;
		}

		evtimer_add(evListen, &GetTimeval(sec));

		DLOG(INFO) << "add Timer sec " << sec << "s" << ". ev " << evListen;

		m_mTimer[evListen] = scb;
		return evListen;
	}

	bool DeleteTimer(event * ev)
	{
		if (m_mTimer.find(ev) == m_mTimer.end())
			return false;
		delete m_mTimer[ev];
		evtimer_del(ev);
		m_mTimer.erase(ev);
		return true;
	}

private:
	std::map<event*, STimer*> m_mTimer;
};

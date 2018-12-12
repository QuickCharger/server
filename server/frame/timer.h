#pragma once

#include <iostream>
#include <functional>

#include <event2/bufferevent.h>
#include <event2/event.h>

#include "log.h"


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
	void InitTimer(event_base* a_pEventBase)
	{
		m_pEventBase = a_pEventBase;
	}

	int AddTimer(unsigned int sec, void *t, timerCB cb, void * param = nullptr, int times = 1)
	{
		if (m_pEventBase == nullptr)
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
		event *evListen = event_new(m_pEventBase, -1, EV_PERSIST | EV_TIMEOUT,
			[](evutil_socket_t a_Socket, short a_nEvent, void *a_pArg) {
			STimer *scb = (STimer*)a_pArg;
			T * target = static_cast<T*>(scb->target);
			(target->*(scb->cb))(scb->param);
			LOG(INFO) << "timer times " << scb->times;
			if (scb->times >= 0 && --scb->times <= 0)
			{
				LOG(INFO) << "delete timer. event: ";
				evtimer_del(scb->ev);
				delete scb;
			}
		}
		, scb);
		scb->ev = evListen;

		if (!evListen)
		{
			delete scb;
			return 0;
		}

		timeval tv;
		tv.tv_sec = sec;
		tv.tv_usec = 0;
		evtimer_add(evListen, &tv);
		return (int)evListen;
	}

private:
	event_base* m_pEventBase = nullptr;
};

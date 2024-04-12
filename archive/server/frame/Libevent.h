#pragma once

#include <list>
#include <signal.h>

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

class CLibevent
{
	CLibevent() {};
	~CLibevent() {};
public:
	static event_base* GetInstance()
	{
		if (!base)
		{
			base  = event_base_new();
		}
		return base;
	}

	static void Run()
	{
		if (!base)
			return;
		event_base_dispatch(base);
	}

	static void Close()
	{
		if (!base)
			return;

		for (auto it = m_lEvent.begin(); it != m_lEvent.end(); ++it)
		{
			event_free(*it);
			*it = nullptr;
		}
		m_lEvent.clear();

		event_base_loopexit(base,nullptr);
		event_base_free(base);
		base = nullptr;
	}

	static bool CaughtSignal(int sig, event_callback_fn cb, void* arg)
	{
		if (!base)
			return false;
		event* ev = evsignal_new(base, sig, cb, (void *)arg);

		if (!ev || event_add(ev, NULL) < 0) {
			return false;
		}

		m_lEvent.push_back(ev);

		return true;
	}

private:
	static event_base *base;
	static std::list<event*> m_lEvent;
};

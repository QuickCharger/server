#pragma once

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
		event_base_loopexit(base,nullptr);
		event_base_free(base);
		base = nullptr;
	}

private:
	static event_base *base;
};

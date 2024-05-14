#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <map>

struct TimerCBArg {
	long long cur = 0;
	void* p1 = nullptr;
	void *p2 = nullptr;
	int i1 = 0; 
	int i2 = 0;
};

struct TimerArg {
	long long tRegAt = 0;		// 注册事件
	long long tTriggerAt = 0;	// 触发时间
	int TimerId = 0;			// 
	int TriggerLeft = 0;		// 触发剩余次数
	int TriggerTimes = 0;		// 已经触发的次数 用于精确触发
	int TriggerIntervalMS = 0;	// 触发间隔
	std::function<void(const TimerCBArg&)> cb;
	TimerCBArg cbArg;
};

class CTimer
{
public:
	CTimer() { };
	virtual ~CTimer()
	{
	};

	void Init() {
		auto now = std::chrono::system_clock::now();
		long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		{
			TimerCBArg arg;
			CTimer::AddTimer(milliseconds, 1000, std::bind(&CTimer::OnTimer1s, this, std::placeholders::_1), std::move(arg), -1);
		}
		{
			TimerCBArg arg;
			CTimer::AddTimer(milliseconds, 1000 * 10, std::bind(&CTimer::OnTimer10s, this, std::placeholders::_1), std::move(arg), -1);
		}
		{
			TimerCBArg arg;
			CTimer::AddTimer(milliseconds, 1000 * 100, std::bind(&CTimer::OnTimer100s, this, std::placeholders::_1), std::move(arg), -1);
		}
	}

	static int TimerId;

	int Check(long long cur) {
		std::list<int> needErase;
		for (auto &it : m) {
			if (it.second.tTriggerAt <= cur) {
				it.second.cbArg.cur = cur;
				it.second.cb(it.second.cbArg);
				it.second.TriggerTimes++;
				it.second.tTriggerAt = it.second.tRegAt + it.second.TriggerIntervalMS * (it.second.TriggerTimes + 1);
				// std::cout << "tRegAt" << it.second.tRegAt << " triggerIntervalMS " << it.second.TriggerIntervalMS << " triggertimes " << it.second.TriggerTimes << " triggerAt " << it.second.tTriggerAt << std::endl;
				if (it.second.TriggerLeft > 0) {
					--it.second.TriggerLeft;
				}
				if (it.second.TriggerLeft == 0) {
					needErase.push_back(it.first);
				}
			}
		}
		for (auto it : needErase) {
			m.erase(it);
		}
		return 0;
	}
	
	int AddTimer(long long cur, unsigned int ms, std::function<void(const TimerCBArg &)> cb, TimerCBArg && cbArg, int leftTimes = -1)
	{
		int timerId = ++TimerId;

		TimerArg arg;
		arg.tRegAt = cur;
		arg.TriggerIntervalMS = ms;
		arg.tTriggerAt = cur + ms;
		arg.cb = cb;
		arg.cbArg = cbArg;
		arg.TimerId = timerId;
		arg.TriggerLeft = leftTimes;
		arg.TriggerTimes = 0;
		m[timerId] = std::move(arg);

		return timerId;
	}

	bool DeleteTimer(int timerId)
	{
		auto it = m.find(timerId);
		if (it == m.end())
			return false;
		m.erase(it);
		return true;
	}

	virtual void OnTimer(const TimerCBArg& arg) { std::cout << __FUNCTION__ << std::endl; }
	virtual void OnTimer1s(const TimerCBArg& arg) { std::cout << __FUNCTION__ << std::endl; }
	virtual void OnTimer10s(const TimerCBArg& arg) { std::cout << __FUNCTION__ << std::endl; }
	virtual void OnTimer100s(const TimerCBArg& arg) { std::cout << __FUNCTION__ << std::endl; }

private:
	std::map<int, TimerArg> m;
};

int CTimer::TimerId = 0;
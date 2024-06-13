#pragma once

#include <iomanip>
#include <map>
#include <math.h>
#include <mutex>
#include <set>
#include <sstream>
#include <vector>

static int formatSiz(int s, int step)
{
	return (int)std::ceil(s *1.0 / step) * step;
}

template<class T>
class CMemPool
{
public:
	CMemPool() {};
	~CMemPool() {};

	void onTimer(long long t)
	{};

	T* Acquire(int s = 1)
	{
		std::unique_lock<std::mutex> lck(mtx);

		int siz = formatSiz(s, step);
		if (useFree.find(siz) != useFree.end() && useFree[siz].size() > 0)
		{
			// 任意取值 begin的是O(n) 效率最高
			auto it = useFree[siz].begin();
			T * ret = *it;
			useFree[siz].erase(it);
			use[siz].insert(ret);
			return ret;
		}
		T *p = alloc(siz);
		use[siz].insert(p);
		return p;
	}

	void Release(T* p)
	{
		std::unique_lock<std::mutex> lck(mtx);

		if (m.find(p) == m.end())
			return;
		int siz = m[p];
		if (use[siz].find(p) != use[siz].end())
		{
			use[siz].erase(p);
		}
		useFree[siz].insert(p);
	}

	void Desc()
	{
		std::unique_lock<std::mutex> lck(mtx);
		std::set<int> k;
		for (auto it : use)
			k.insert(it.first);
		for (auto it : useFree)
			k.insert(it.first);
		std::stringstream ss;
		for (auto it : k)
		{
			auto itUse = use.find(it);
			auto itUseFree = useFree.find(it);
			auto sizUse = itUse != use.end() ? itUse->second.size() : 0;
			auto sizUseFree = itUseFree != useFree.end() ? itUseFree->second.size() : 0;
			ss << it << " byte * [ use: " << sizUse << " + free: " << sizUseFree << " ] = " << std::fixed << std::setprecision(2) << it * (sizUse + sizUseFree) /1000.0/1000.0 << "MB" << std::endl;
		}
		std::cout << ss.str().c_str() << std::endl;
	}

private:

	T* alloc(int siz)
	{
		T *p = new T[siz];
		m[p] = siz;
		return p;
	}

	void dealloc(T**p)
	{
		m.erase(*p);
		delete[] * p;
		*p = nullptr;
	}

	std::map<int, std::set<T*>> use;		// <siz, set<T*>>
	std::map<int, std::set<T*>> useFree;
	std::map<T*, int> m;		// T* -> siz

public:
	int step = 1000;		// 每种内存池的步进

// 定时清理
public:
	int cleanInterval = 0;	// <= 0 不清理
	long long cleanAt = 0;
	std::map<int, std::vector<int>> memUsageRecord;

private:
	std::mutex mtx;
};

#pragma once

#include <mutex>
#include <vector>

struct bufferevent;

// 确保plain
struct Event {
	enum Type {
		BLANK = 0,			// 占位
		// 服务器
		SocketAccept = 1,	// server
		// 客户端
		ConnectTo,			// 客户端主动链接 主动发送 
		ConnectSuccess,		// 主动链接成功， 如果失败发送Err
		// 通用
		DataIn,				// 接收数据
		Err,
		Close,				// socket 关闭了 数据应该都清理掉
		TryClose,			// socket 主动断开链接 主动发送 
		Send,				// 发送数据
	};

	long long uid = 0;
	Type e = Type::BLANK;

	void* p1 = nullptr;
	void* p2 = nullptr;
	long long l1 = 0;
	long long l2 = 0;
	//int i1 = 0;
	//int i2 = 0;
	std::string str1;
	std::string str2;
};

// 三个buffer 一个生产者 一个消费者 轮流切换到 闲置buffer
// 有缺陷 只有当生产者不停调用Productor时 才能确保生产者手中最后的数据被消费者拿到 todo
template <class T>
class SafeBuffer {
public:
	SafeBuffer() {
		vProduct = new std::vector<T>;
		vComsum = new std::vector<T>;
		v3rd = new std::vector<T>;
	}
	virtual ~SafeBuffer() {
		delete vProduct;
		delete vComsum;
		delete v3rd;
	}

	// 消费者获取新的buffer
	// 先消耗vComsum 再消耗v3rd 如此才能保证数据顺序！！！
	std::vector<T>* Comsumer(std::vector<T>* p, bool wait = true)
	{
		// 强制 否则外部可能获得 nullptr
		if (p == nullptr)
			wait = true;

		if (wait)
		{
			std::unique_lock<std::mutex> lck(mtx);
			if (p == nullptr)
			{
				return vComsum;
			}
			if (p->size() > 0)
			{
				return p;
			}
			if (v3rd->size() > 0)
			{
				std::swap(v3rd, vComsum);
				return vComsum;
			}
			return p;
		}
		else
		{
			std::unique_lock<std::mutex> lck(mtx, std::try_to_lock);
			if (lck.owns_lock())
			{
				if (p == nullptr)
				{
					return vComsum;
				}
				if (p->size() > 0)
				{
					return p;
				}
				if (v3rd->size() > 0)
				{
					std::swap(v3rd, vComsum);
					return vComsum;
				}
			}
			return p;
		}
	}

	// 生产者获取新的buffer
	// 先写入vProduct 只有当v3rd为空时 才交换 如此才能保证数据顺序！！！
	std::vector<T>* Productor(std::vector<T>*p, bool wait = true)
	{
		// 强制 否则外部可能获得 nullptr
		if (p == nullptr)
			wait = true;

		if (wait)
		{
			std::unique_lock<std::mutex> lck(mtx);
			{
				// 获取新的buffer 
				if (p == nullptr)
				{
					return vProduct;
				}
				if (v3rd->size() == 0)
				{
					std::swap(v3rd, vProduct);
					return vProduct;
				}
			}
			return p;
		}
		else
		{
			std::unique_lock<std::mutex> lck(mtx, std::try_to_lock);
			if (lck.owns_lock())
			{
				// 获取新的buffer 
				if (p == nullptr)
				{
					return vProduct;
				}
				if (v3rd->size() == 0)
				{
					std::swap(v3rd, vProduct);
					return vProduct;
				}
			}
		}
		return p;
	}
private:
	std::mutex mtx;
	std::vector<T> *vProduct;
	std::vector<T> *vComsum;
	std::vector<T> *v3rd;
};

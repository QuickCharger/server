#pragma once

#include <mutex>
#include <vector>

enum class Event {
	BLANK = 0,	// 占位
	SocketCreate = 1,
	DataIn,
	SocketErr,
	Close,
};

struct bufferevent;

// 确保plain
struct EventStruct {
	bufferevent* bev = nullptr;
	void* p1 = nullptr;
	void* p2 = nullptr;
	void *p3 = nullptr;
	int i1 = 0;
	int i2 = 0;
	int i3 = 0;
	Event e = Event::BLANK;
};

// 三个buffer 一个生产者 一个消费者 轮流切换到 闲置buffer
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
	std::vector<T>* Comsumer(std::vector<T>* p) {
		std::unique_lock<std::mutex> lck(mtx);
		if (p == nullptr) {
			return vComsum;
		}
		if (p->size() > 0) {
			return p;
		}
		if (v3rd->size() > 0) {
			std::swap(v3rd, vComsum);
			return vComsum;
		}
		return p;
	}

	// 生产者获取新的buffer
	// 先写入vProduct 只有当v3rd为空时 才交换 如此才能保证数据顺序！！！
	std::vector<T>* Productor(std::vector<T>*p) {
		std::unique_lock<std::mutex> lck(mtx);
		// 获取新的buffer 
		if (p == nullptr) {
			return vProduct;
		}
		if (v3rd->size() == 0) {
			std::swap(v3rd, vProduct);
			return vProduct;
		}
		return p;
	}
private:
	std::mutex mtx;
	std::vector<T> *vProduct;
	std::vector<T> *vComsum;
	std::vector<T> *v3rd;
};

#pragma once

#include <mutex>
#include <vector>

enum class Event {
	BLANK = 0,	// ռλ
	SocketCreate = 1,
	DataIn,
	SocketErr,
	Close,
};

struct bufferevent;

// ȷ��plain
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

// ����buffer һ�������� һ�������� �����л��� ����buffer
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

	// �����߻�ȡ�µ�buffer
	// ������vComsum ������v3rd ��˲��ܱ�֤����˳�򣡣���
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

	// �����߻�ȡ�µ�buffer
	// ��д��vProduct ֻ�е�v3rdΪ��ʱ �Ž��� ��˲��ܱ�֤����˳�򣡣���
	std::vector<T>* Productor(std::vector<T>*p) {
		std::unique_lock<std::mutex> lck(mtx);
		// ��ȡ�µ�buffer 
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

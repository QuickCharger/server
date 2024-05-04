#pragma once

#include <mutex>
#include <vector>

enum class Event {
	BLANK = 0,	// ռλ
	SocketAccept = 1,	// server
	DataIn,
	SocketErr,
	Close,
	SocketConnectTo,	// client
	SocketConnectErr,
	SocketConnectSuccess,
	RegBufferEvent,		// ��socket���bufferevent
};

enum class SocketState {
	BLANK = 0,	// ռλ
	Connecting,	// ��������ing
	Connected,	// ����
	Err,		// ������
	Closeing,	// �����ر�ing Ҳ���Ǵ����Ĵ�����
	Closed,		// 
};

struct bufferevent;

// ȷ��plain
struct EventStruct {
	// libevent������
	bufferevent* bev = nullptr;
	void* that = nullptr;
	long long uid = 0;
	Event e = Event::BLANK;
	SocketState state = SocketState::BLANK;

	// �û�������
	void* p1 = nullptr;
	void* p2 = nullptr;
	long long l1 = 0;
	long long l2 = 0;
	int i1 = 0;
	int i2 = 0;
	std::string str1;
	std::string str2;
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

#pragma once

#include <string>
#include <vector>
#include <utility>

#include "common.h"
#include "IRunnable.h"
#include "ProductConsume.h"
#include "Timer.h"
#include "Libevent.h"

class Runnable : public IRunnable, public CTimer {
public:
	Runnable();
	virtual ~Runnable();
	virtual int Init();
	virtual int Run();
	virtual int Stop();

	void InitNet();

	virtual int OnInit() = 0;
	virtual int OnRun() = 0;
	virtual int OnStop() = 0;

	// 服务端 接收新的fd
	virtual int OnNet_SocketAccept(long long uid) { std::cout << "warning " << __FUNCTION__ << " need override" << std::endl; return 0; }
	// 客户端 主动链接成功， 如果失败发送Err
	virtual int OnNet_ConnectSuccess(long long uid) { std::cout << "warning " << __FUNCTION__ << " need override" << std::endl; return 0; };
	// fd 消息
	virtual int OnNet_DataIn(long long uid, char* ch, int len) { std::cout << "warning " << __FUNCTION__ << " need override" << std::endl; return 0; };
	// fd 错误
	virtual int OnNet_Err(long long uid) { std::cout << "warning " << __FUNCTION__ << " need override" << std::endl; return 0; };
	// 服务端 fd关闭 资源应该全部清理
	virtual int OnNet_Close(long long uid) { std::cout << "warning " << __FUNCTION__ << " need override" << std::endl; return 0; };

	void ProductMsg(long long uid, Event::Type t, void* p1 = nullptr, void *p2 = nullptr, long long l1 = 0, long long l2 = 0, const std::string& str1 = "", const std::string& str2 = "");

private:
	std::vector<Event>* pEventC = nullptr;
	std::vector<Event>* pEventP = nullptr;

	CLibevent *net = nullptr;
};

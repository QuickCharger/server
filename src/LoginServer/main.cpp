#include "Libevent.h"
#include "thread"
#include "mutex"
#include <csignal>
#include "Session.h"
#include "common.h"
#include "Work.h"

void signal_handler(int signal) {
	exit(0);
}

//class C {
//public:
//	void cb() {
//		std::cout << "cb" << std::endl;
//	}
//};

int main(int argc, char **argv)
{
	//C c;
	//typedef void (*pfunc)();
	//std::function<void(void)> pf = std::bind(&C::cb, &c);
	//pf();

	//int *pch = new int[10000 * 100];
	//for (int i = 0; i < 10000 * 100; ++i) {
	//	pch[i] = i;
	//}

	std::signal(SIGINT, signal_handler);   // Ctrl+C
	std::signal(SIGTERM, signal_handler);  // kill

	std::thread t([]() {
		LIBEVENT::Init();
		LIBEVENT::Listen(12345);
		LIBEVENT::Run();
	});

	std::thread t2([](){
		work->Init();
		work->Run();
	});

	t.join();
	{
		std::unique_lock<std::mutex> lck(ioMtx);
		std::cout << "t finish" << std::endl;
	}
	t2.join();
	{
		std::unique_lock<std::mutex> lck(ioMtx);
		std::cout << "t2 finish" << std::endl;
	}

	return 0;
}

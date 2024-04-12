#include "Libevent.h"
#include "thread"
#include "mutex"
#include <csignal>
#include "Session.h"
#include "common.h"

void consumer() {

	std::vector<EventStruct>* pEventRead = nullptr;
	std::vector<EventStruct>* pEventWrite = nullptr;

	while (true) {
		pEventRead = eventsRead.Comsumer(pEventRead);
		pEventWrite = eventsWrite.Productor(pEventWrite);
		// 处理net
		// event处理不了无所谓 可以先执行内部的事件循环
		{
			for (auto it = pEventRead->begin(); it != pEventRead->end(); ++it) {
				if (it->e == Event::SocketCreate) {
					Session_New(it->fd, (bufferevent*)it->p1);
				} else if (it->e == Event::DataIn) {
					Session_Append(it->fd, (char*)it->p1, it->i1);
				} else if (it->e == Event::SocketErr) {
					Session_Destroy(it->fd);
				}
			}
			pEventRead->clear();
			//std::unique_lock<std::mutex> lck(LIBEVENT::mtxEventsOUT);
			//std::unique_lock<std::mutex> lck(LIBEVENT::mtxEventsOUT, std::try_to_lock);
			//if (lck.owns_lock()) 
			//{
			//	for (auto &it : LIBEVENT::eventsOUT) {
			//		int fd = std::get<0>(it);
			//		LIBEVENT::Event e = std::get<1>(it);
			//		void* ch = std::get<2>(it);
			//		int len = std::get<3>(it);
			//		if (e == LIBEVENT::Event::SocketCreate) {
			//			Session_New(len, (bufferevent*)ch);
			//		}
			//		else if (e == LIBEVENT::Event::DataIn) {
			//			Session_Append(fd, (char*)ch, len);
			//		}
			//		else if (e == LIBEVENT::Event::SocketErr) {
			//			Session_Destroy(fd);
			//		}
			//	}
			//	LIBEVENT::eventsOUT.clear();
			//}
		}

		// 处理该线程逻辑
		{
			// 处理个虚假的逻辑
			for (auto &it : Sessions) {
				it.second->FakeNews();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void signal_handler(int signal) {
	exit(0);
}

int main(int argc, char **argv)
{
	std::signal(SIGINT, signal_handler);   // Ctrl+C
	std::signal(SIGTERM, signal_handler);  // kill

	std::thread t([]() {
		LIBEVENT::Init();
		LIBEVENT::Listen(12345);
		LIBEVENT::Run();
	});

	std::thread t2(consumer);

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

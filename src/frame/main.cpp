#include "Libevent.h"
#include "thread"
#include "mutex"
#include <csignal>
#include "Session.h"
#include "common.h"
//
//void consumer() {
//
//	std::vector<EventStruct>* pEventRead = nullptr;
//	std::vector<EventStruct>* pEventWrite = nullptr;
//
//	while (true) {
//		pEventRead = eventsRead.Comsumer(pEventRead);
//		pEventWrite = eventsWrite.Productor(pEventWrite);
//		// 处理net
//		// event处理不了无所谓 可以先执行内部的事件循环
//		{
//			for (auto it = pEventRead->begin(); it != pEventRead->end(); ++it) {
//				if (it->e == Event::SocketCreate) {
//					Session_New(it->fd, (bufferevent*)it->p1);
//				} else if (it->e == Event::DataIn) {
//					Session_Append(it->fd, (char*)it->p1, it->i1);
//				} else if (it->e == Event::SocketErr) {
//					Session_Destroy(it->fd);
//				}
//			}
//			pEventRead->clear();
//		}
//
//		// 处理该线程逻辑
//		{
//			// 处理个虚假的逻辑
//			for (auto &it : Sessions) {
//				it.second->FakeNews();
//			}
//		}
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//	}
//}

void signal_handler(int signal) {
	exit(0);
}

//std::thread t(createThread, std::ref(*it));
//void createThread(IRunnable* r) {
//	r->Init();
//	r->Run();
//	r->Stop();
//}

int main(int argc, char **argv)
{
	std::signal(SIGINT, signal_handler);   // Ctrl+C
	std::signal(SIGTERM, signal_handler);  // kill


	std::vector<IRunnable*>* threadC = nullptr;

	while (true) {
		threadC = threadPC->Comsumer(threadC);
		for (auto it = threadC->begin(); it != threadC->end(); ++it) {
			IRunnable* i = *it;
			std::thread t([i]()-> void {
				i->Init();
				i->Run();
				i->Stop();
			});
			t.detach();
		}
		threadC->clear();
	}

	return 0;
}

#include "Libevent.h"
#include "thread"
#include "mutex"
#include <csignal>
#include "Session.h"
#include "common.h"

/*
宕机
event.c L3248
win32select.c L265
*/
void consumer() {
	while (true) {
		// 处理net
		{
			std::unique_lock<std::mutex> lck(LIBEVENT::mtxEventsOUT);
			//std::unique_lock<std::mutex> lck(LIBEVENT::mtxEventsOUT, std::try_to_lock);
			//if (lck.owns_lock()) 
			{
				// A
				//if (LIBEVENT::eventsOUT.size()) {
				//	std::cout << "for begin" << std::endl;
				//	for (auto &it : LIBEVENT::eventsOUT) {
				//		std::tuple<int, LIBEVENT::Event, void*, int> d = it;
				//		int fd = std::get<0>(d);
				//		LIBEVENT::Event e = std::get<1>(d);
				//		void* ch = std::get<2>(d);
				//		int len = std::get<3>(d);
				//		std::cout << fd << " " << (int)e << " " << ch << " " << len << " " << std::endl;
				//	}
				//	std::cout << "for end" << std::endl;
				//}

				// B 
				// 不留AC留B 会崩溃
				for (auto &it : LIBEVENT::eventsOUT) {
					int fd = std::get<0>(it);
					LIBEVENT::Event e = std::get<1>(it);
					void* ch = std::get<2>(it);
					int len = std::get<3>(it);
					if (e == LIBEVENT::Event::SocketCreate) {
						Session_New(len, (bufferevent*)ch);
					}
					else if (e == LIBEVENT::Event::DataIn) {
						Session_Append(fd, (char*)ch, len);
					}
					else if (e == LIBEVENT::Event::SocketErr) {
						Session_Destroy(fd);
					}
				}
				LIBEVENT::eventsOUT.clear();

				//{
				//	static int c = 0;
				//	++c;
				//	if (c % 100 == 0) {
				//		std::cout << "Sessions size " << Sessions.size() << std::endl;
				//	}
				//}
			}
		}

		// 处理该线程逻辑
		{
			// 处理个虚假的逻辑
			// 如果session本身有数据 则desc 并把数据返回给fd
			for (auto &it : Sessions) {
				it.second->FakeNews();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
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

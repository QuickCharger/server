#include "Libevent.h"
#include "thread"
#include "mutex"
#include <csignal>
#include "Session.h"
#include "common.h"
#include "Work.h"

Work *work = new Work;

void Work::Init() {
	pEventRead = eventsRead.Comsumer(pEventRead);
	pEventWrite = eventsWrite.Productor(pEventWrite);
}

void Work::Run() {
	while (true) {
		pEventRead = eventsRead.Comsumer(pEventRead);
		pEventWrite = eventsWrite.Productor(pEventWrite);

		// 处理net
		for (auto it = pEventRead->begin(); it != pEventRead->end(); ++it) {
			if (it->e == Event::SocketCreate) {
				Session_New(it->fd, (bufferevent*)it->p1);
			}
			else if (it->e == Event::DataIn) {
				Session_Append(it->fd, (char*)it->p1, it->i1);
			}
			else if (it->e == Event::SocketErr) {
				Session_Destroy(it->fd);
			}
		}
		pEventRead->clear();

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

#include "thread"
#include "mutex"
#include <csignal>
#include "./frame/Session.h"
#include "Work.h"
#include "Client.h"
#include "./frame/Libevent.h"

Work *work = new Work;
int port = 12345;

Work::Work() {
	RegThread(this);
}

Work::~Work() {
	// do nothing
}

int Work::Init() {
	net = new CLibevent;
	net->config.port = port;
	RegThread(net);

	net->Consume(&pEventC);
	net->Product(&pEventP);
	return 0;
}

int Work::Run() {
	while (true) {
		net->Consume(&pEventC);
		net->Product(&pEventP);

		// 处理net
		for (auto it = pEventC->begin(); it != pEventC->end(); ++it) {
			if (it->e == Event::SocketAccept) {
				Client::CreateClient(it->bev);
			}
			else if (it->e == Event::DataIn) {
				long long uid = it->uid;
				gClients[uid]->OnMsg(it->p1, it->i1);
			}
			else if (it->e == Event::SocketErr) {
				Client::DestroyClient(it->bev);
			}
		}
		pEventC->clear();

		// 处理该线程逻辑
		{
			// 处理个虚假的逻辑
			for (auto &it : gClients) {
				it.second->FakeNews();
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

int Work::Stop() {
	// todo
	return 0;
}

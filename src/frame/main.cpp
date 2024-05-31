#include "Libevent.h"
#include "thread"
#include "mutex"
#include <csignal>
#include "common.h"

void signal_handler(int signal)
{
	exit(0);
}

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

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}

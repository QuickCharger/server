#include "common.h"

std::mutex ioMtx;

SafeBuffer<IRunnable*>* threadPC = nullptr;
std::vector<IRunnable*>* threadP = nullptr;

void RegThread(IRunnable* r) {
	if (threadPC == nullptr) {
		threadPC = new SafeBuffer<IRunnable *>;
	}
	threadP = threadPC->Productor(threadP);
	threadP->push_back(r);
	threadP = threadPC->Productor(threadP);
}

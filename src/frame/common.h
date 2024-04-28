#pragma once

#include <mutex>
#include <vector>

#include "ProductConsume.cc"
#include "IRunnable.cc"

extern std::mutex ioMtx;

extern SafeBuffer<IRunnable*> *threadPC;
extern void RegThread(IRunnable*);

#pragma once

#include <mutex>
#include <vector>

#include "ProductConsume.h"
#include "IRunnable.h"

extern std::mutex ioMtx;

extern SafeBuffer<IRunnable*> *threadPC;
extern void RegThread(IRunnable*);

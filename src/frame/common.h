#pragma once

#include <mutex>
#include <vector>

#include "ProductConsume.h"
#include "IRunnable.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif


extern std::mutex ioMtx;

extern SafeBuffer<IRunnable*> *threadPC;
extern void RegThread(IRunnable*);

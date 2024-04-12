#include "common.h"

std::mutex ioMtx;

SafeBuffer<EventStruct> eventsRead;
SafeBuffer<EventStruct> eventsWrite;

//SafeBuffer eventsRead;
//SafeBuffer eventsWrite;

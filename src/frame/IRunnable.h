#pragma once

#include <string>
#include <vector>
#include <utility>

struct Config {
	int port = 0;
};

class IRunnable {
public:
	virtual int Init() = 0;
	virtual int Run() = 0;
	virtual int Stop() = 0;
	Config config;
};

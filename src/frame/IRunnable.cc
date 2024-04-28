#pragma once

class IRunnable {
public:
	virtual int Init() = 0;
	virtual int Run() = 0;
	virtual int Stop() = 0;
};

#pragma once

class IRunnable {
public:
	virtual void Init() = 0;
	virtual void Run() = 0;
	virtual void Stop() = 0;
};

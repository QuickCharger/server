#pragma once

#include <map>
#include <string>

// plain
struct Msg {

};

class Robot {
public:
	Robot();
	virtual ~Robot();

	void OnConnectSuccess();
	void OnDataIn(char*, int);
	void OnErr();
	void OnClose();

	int  Send(char *p, int len);
	void DoReconnect(const std::string& ip, int port);
	int  DoLogout();
	void Desc(const char* format, ...);

public:
	bool reConnect = false;
	int reConnectIntervalSec = 0;
	long long uid = 0;
};

extern std::map<long long, Robot*> gRobots;

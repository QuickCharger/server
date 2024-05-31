#pragma once
#include <map>

// plain
struct Msg {

};

class Client {
public:
	Client();
	virtual ~Client();
	static void CreateClient(long long uid);
	static void DestroyClient(long long uid);

	void OnMsg(char* ch, int len);
	void Send(char* ch, int len);

private:
	long long uid = 0;
};

extern std::map<long long, Client*> gClients;

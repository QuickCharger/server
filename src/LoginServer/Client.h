#include "Session.h"

class Client {
public:
	Client();
	virtual ~Client();
	static void CreateClient(bufferevent*);
	static void DestroyClient(bufferevent*);

	void OnMsg(void*, int);

public:
	bufferevent* m_bev = nullptr;
};

extern std::map<int, Client*> gClients;

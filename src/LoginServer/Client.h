#include "./frame/Session.h"

class Client {
public:
	Client();
	virtual ~Client();
	static void CreateClient(bufferevent*);
	static void DestroyClient(bufferevent*);

	void InitSession(bufferevent*);

	void OnMsg(void*, int);

	void FakeNews();

public:
	Session* m_session = nullptr;
};

extern std::map<int, Client*> gClients;

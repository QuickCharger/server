#include "./frame/Session.h"

// plain
struct Msg {

};

class Client {
public:
	Client();
	virtual ~Client();
	static void CreateClient(bufferevent*);
	static void DestroyClient(long long uid);

	void InitSession(bufferevent*);

	void OnMsg(void*, int);

	void FakeNews();

public:
	std::vector<std::string> m_unhandledMsg;	
	Session* m_session = nullptr;
};

extern std::map<long long, Client*> gClients;

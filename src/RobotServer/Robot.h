#include "./frame/Session.h"

// plain
struct Msg {

};

class Robot {
public:
	Robot();
	virtual ~Robot();

	void OnEvent(Event& e);
	void OnMsg(void*, int);

	int  Send(char *p, int len);
	void DoReconnect(const std::string& ip, int port);
	int  DoLogout();
	void Desc(const char* format, ...);

public:
	bool reConnect = false;
	int reConnectIntervalSec = 0;
	long long uid = 0;
	Session* m_session = nullptr;
};

extern std::map<long long, Robot*> gRobots;

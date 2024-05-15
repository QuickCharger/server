#include "./frame/Session.h"

// plain
struct Msg {

};

class Robot {
public:
	Robot();
	virtual ~Robot();

	void OnSession(Event& e);

	void DoReconnect(const std::string& ip, int port);
	void Desc(const char* format, ...);

public:
	bool reConnect = false;
	int reConnectIntervalSec = 0;
	long long uid = 0;
	Session* m_session = nullptr;
};

extern std::map<long long, Robot*> gRobots;

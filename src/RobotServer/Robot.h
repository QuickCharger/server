#include "./frame/Session.h"

// plain
struct Msg {

};

class Robot {
public:
	Robot();
	virtual ~Robot();

	void OnSession(EventStruct& e);

public:
	long long uid = 0;
	Session* m_session = nullptr;
};

extern std::map<int, Robot*> gRobots;

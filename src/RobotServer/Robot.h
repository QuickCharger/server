#include "./frame/Session.h"

// plain
struct Msg {

};

class Robot {
public:
	Robot();
	virtual ~Robot();

	void OnSession(Event& e);

	void Desc();

public:
	long long uid = 0;
	Session* m_session = nullptr;
};

extern std::map<long long, Robot*> gRobots;

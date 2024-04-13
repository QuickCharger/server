#pragma  once

class Work
{
public:
	void Init();
	void Run();

private:
	std::vector<EventStruct>* pEventRead = nullptr;
	std::vector<EventStruct>* pEventWrite = nullptr;
};

extern Work *work;

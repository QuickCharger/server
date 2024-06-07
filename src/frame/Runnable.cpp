#include "Runnable.h"

Runnable::Runnable()
{
	RegThread(this);
}

Runnable::~Runnable()
{

}

int Runnable::Init()
{
	this->OnInit();
	InitNet();
	CTimer::Init();
	return 0;
}

int Runnable::Run() 
{
	while (true)
	{
		this->OnRun();
		net->Consume(&pEventC);
		net->Product(&pEventP);

		auto now = std::chrono::system_clock::now();
		long long milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		CTimer::Check(milliseconds);

		if (pEventC)
		{
			for (auto it = pEventC->begin(); it != pEventC->end(); ++it) {
				if (it->e == Event::SocketAccept)
				{
					OnNet_SocketAccept(it->uid);
				}
				else if (it->e == Event::DataIn)
				{
					OnNet_DataIn(it->uid, (char*)it->p1, (int)it->l1);
				}
				else if (it->e == Event::Err)
				{
					OnNet_Err(it->uid);
				}
				else if (it->e == Event::Close)
				{
					OnNet_Close(it->uid);
				}
				else if (it->e == Event::ConnectSuccess)
				{
					OnNet_ConnectSuccess(it->uid);
				}
			}
			pEventC->clear();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int Runnable::Stop() {
	this->OnStop();
	return 0;
}

void Runnable::InitNet()
{
	if (net != nullptr)
		return;

	net = new CLibevent;
	net->config = config;
	RegThread(net);
	net->Consume(&pEventC);
	net->Product(&pEventP);
}

void Runnable::ProductMsg(long long uid, Event::Type t, void* p1, void *p2, long long l1, long long l2, const std::string& str1, const std::string& str2)
{
	if (pEventP == nullptr)
		return;
	Event e;
	e.uid = uid;
	e.e = t;
	e.p1 = p1;
	e.p2 = p2;
	e.l1 = l1;
	e.l2 = l2;
	e.str1 = str1;
	e.str2 = str2;
	pEventP->push_back(std::move(e));
}

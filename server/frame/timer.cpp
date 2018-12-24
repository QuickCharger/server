#include "timer.h"
//template<typename T> event_base* CTimer<typename T>::m_pEventBase = nullptr;

timeval GetTimeval(int a_sec, int a_usec)
{
	timeval tv;
	tv.tv_sec = a_sec;
	tv.tv_usec = a_usec;
	return tv;
}

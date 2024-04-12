#include "Libevent.h"

event_base* CLibevent::base = nullptr;
std::list<event*> CLibevent::m_lEvent;

#include "CTime.h"
#include <time.h>

std::string CTime::currentDate(std::string format)
{
	time_t tCurrent = time(nullptr);
	struct tm *pTime = localtime(&tCurrent);
	char ret[100];
	strftime(ret, sizeof(ret), format.c_str(), pTime);
	return ret;
}

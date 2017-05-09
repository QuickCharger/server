#ifndef _BASE_ERRRECORD_H_
#define _BASE_ERRRECORD_H_

#include <string>
#include "macro.h"

template<typename T>
class CErrRecord
{
private:
	DEFINE_TYPE_REFER(std::string, m_strErrorRecord, "", GetErr, SetErr);
};

#endif

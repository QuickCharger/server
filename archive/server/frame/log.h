#pragma once

#define GOOGLE_GLOG_DLL_DECL
#undef ERROR

#include <glog/logging.h>

extern "C"{
	void InitLog(char *argv[]);
	void CloseLog();
}

#undef GOOGLE_GLOG_DLL_DECL

#pragma once

#include <string>

class CFile
{
public:
	static bool ReadFile(const char* a_pFileName, std::string& a_strContext);
};

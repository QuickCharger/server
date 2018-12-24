#ifndef _BASE_FUNC_H_
#define _BASE_FUNC_H_

#include <string>
#include <vector>

namespace Func
{
	//void showSomething();
	void SplitString(const std::string& a_strSource, const std::string& delim, std::vector<std::string >& a_vStrDest);
	bool SplitFilename(const std::string& a_strFullFileName, char a_Delim, std::string& a_strFilePath, std::string& a_strFileName);
	std::string GetFileName(const std::string& a_strFullFileName);
	std::string Truncate(const std::string& a_strSource, const char a_ch);
	std::string Abandon(const std::string& a_strSource, const char a_ch);
}

#endif

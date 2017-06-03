#ifndef _BASE_FUNC_H_
#define _BASE_FUNC_H_

#include <string>
#include <vector>

namespace Func
{
	//void showSomething();
	void SplitString(const std::string& a_strSource, const std::string& delim, std::vector<std::string >& a_vStrDest);
	void SplitFilename(const std::string& a_strFullFileName, const char * const a_pDelim, std::string& a_strFilePath, std::string& a_strFileName);
	std::string Truncate(const std::string& a_strSource, const char a_ch);
	std::string Abandon(const std::string& a_strSource, const char a_ch);
}

#endif

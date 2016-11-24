#ifndef _FUNC_H_
#define _FUNC_H_

#include <string>
#include <vector>

namespace Func
{
	//void showSomething();
	void SplitString(const std::string& a_StrSource, const std::string& delim, std::vector<std::string >& a_vStrDest);
	void SplitFilename(const std::string& a_StrFullFileName, const char * const a_pDelim, std::string& a_StrFilePath, std::string& a_StrFileName);
}

#endif

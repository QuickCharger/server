#include "func.h"

using namespace std;

namespace Func{
	void SplitString(const string& a_StrSource, const string& delim, std::vector<string >& a_vStrDest)
	{
		size_t last = 0;
		size_t index = a_StrSource.find_first_of(delim, last);
		while (index != -1)
		{
			a_vStrDest.push_back(a_StrSource.substr(last, index - last));
			last = index + 1;
			index = a_StrSource.find_first_of(delim, last);
		}
		if (a_StrSource.length() > last)
		{
			a_vStrDest.push_back(a_StrSource.substr(last));
		}
	}

	void SplitFilename(const string& a_StrFullFileName, const char * const a_pDelim, string& a_StrFilePath, string& a_StrFileName)
	{
		std::size_t found = a_StrFullFileName.find_last_of(a_pDelim);
		a_StrFilePath.assign(a_StrFullFileName.substr(0, found + 1));
		a_StrFileName.assign(a_StrFullFileName.substr(found + 1));
	}
}

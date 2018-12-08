#include "func.h"

using namespace std;

namespace Func{
	void SplitString(const string& a_strSource, const string& delim, std::vector<string >& a_vStrDest)
	{
		size_t last = 0;
		size_t index = a_strSource.find_first_of(delim, last);
		while (index != -1)
		{
			a_vStrDest.push_back(a_strSource.substr(last, index - last));
			last = index + 1;
			index = a_strSource.find_first_of(delim, last);
		}
		if (a_strSource.length() > last)
		{
			a_vStrDest.push_back(a_strSource.substr(last));
		}
	}

	void SplitFilename(const string& a_StrFullFileName, const char * const a_pDelim, string& a_StrFilePath, string& a_StrFileName)
	{
		std::size_t found = a_StrFullFileName.find_last_of(a_pDelim);
		a_StrFilePath.assign(a_StrFullFileName.substr(0, found + 1));
		a_StrFileName.assign(a_StrFullFileName.substr(found + 1));
	}

	std::string Truncate(const std::string& a_strSource, const char a_ch)
	{
		string str(a_strSource, 0, a_strSource.find(a_ch));
		return str;
	}

	std::string Abandon(const std::string& a_strSource, const char a_ch)
	{
		string str;
		for (auto it = a_strSource.begin(); it != a_strSource.end(); ++it)
		{
			if (*it != a_ch)
			{
				str += *it;
			}
		}
		return str;
	}
}

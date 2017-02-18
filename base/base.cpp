#include "base.h"
#include <iostream>
#include <vector>
#include "func.h"

using namespace std;

#define ERRSIZE 1000
char chLastErr[ERRSIZE];

void setLastErr(const char* a_pCh)
{
	memset(chLastErr, 0, ERRSIZE);
	memcpy(chLastErr, a_pCh, strlen(a_pCh) >= ERRSIZE ? ERRSIZE - 1 : strlen(a_pCh));
}

const char* GetLastErr()
{
	return chLastErr;
}

void ReadFile(const char* a_pFileName, std::string& a_strContext)
{
	FILE *pFile;
	pFile = fopen(a_pFileName, "r");
	if (pFile == NULL)
	{
		setLastErr("Open file failed.");
		return;
	}

	fseek(pFile, 0L, SEEK_END);
	int nFileSize = ftell(pFile);
	char *pContext = (char *)malloc(nFileSize + 1);
	memset(pContext, 0, nFileSize + 1);
	if (pContext == nullptr)
	{
		setLastErr("malloc failed.");
		return;
	}
	fseek(pFile, 0L, SEEK_SET);
	fread(pContext, nFileSize, 1, pFile);
	pContext[nFileSize] = 0;

	a_strContext = pContext;
	free(pContext);
}

int InitConfig(const char* a_pFileName, std::map<std::string, std::string>& a_mDefaultValue, std::map<std::string, std::pair<std::string, int>>& a_mServer)
{
	std::string strConfigName = Func::Truncate(a_pFileName, '.') + ".cfg";
	if (strConfigName.size() < 4)
	{
		return 1;
	}
	std::string strContext;
	ReadFile(strConfigName.c_str(), strContext);
	if (strContext.size() == 0)
	{
		return 2;
	}

	std::vector<std::string > vSplit;
	Func::SplitString(strContext, "\n", vSplit);

	for (auto it = vSplit.begin(); it != vSplit.end(); ++it)
	{
		std::vector<std::string > vSplit;
		if (strncmp(it->c_str(), "set ", strlen("set ")) == 0)
		{
			if (strncmp(it->c_str(), "set int ", strlen("set int ")) == 0)
			{
				std::string str(*it, strlen("set int "));
				std::string strType(str.c_str(), str.find_first_of("="));
				std::string strValue(str, str.find_first_of("=") + 1);
				a_mDefaultValue[strType] = strValue;
				continue;
			}
			else if (strncmp(it->c_str(), "set float ", strlen("set float ")) == 0)
			{
				std::string str(*it, strlen("set float "));
				std::string strType(str.c_str(), str.find_first_of("="));
				std::string strValue(str, str.find_first_of("=") + 1);
				a_mDefaultValue[strType] = strValue;
				continue;
			}
			else if (strncmp(it->c_str(), "set string ", strlen("set string ")) == 0)
			{
				std::string str(*it, strlen("set string "));
				std::string strType(str.c_str(), str.find_first_of("="));
				std::string strValue(str, str.find_first_of("=") + 1);
				a_mDefaultValue[strType] = strValue;
				continue;
			}
			std::cout << "Unknown Type: " << *it << endl;
			continue;
		}
		Func::SplitString(*it, "server ", vSplit);
		if (strncmp(it->c_str(), "server ", strlen("server ")) == 0)
		{
			std::string str(*it, strlen("server "));
			std::string strServerName(str.c_str(), str.find_first_of("="));
			std::string strValue(str, str.find_first_of("=") + 1);
			std::string strIP(strValue.c_str(), strValue.find_first_of(":"));
			std::string strPort(strValue, strValue.find_first_of(":") + 1);
			int nPort = atoi(strPort.c_str());
			if (strIP.size() == 0 || strPort.size() == 0)
			{
				std::cout << "Server cfg Err : " << *it << endl;
				continue;
			}
			a_mServer[strServerName] = make_pair(strIP, nPort);
		}
	}
	return 0;
}

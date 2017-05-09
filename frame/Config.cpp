#include "Config.h"
#include <iostream>
#include "base.h"
#include "func.h"

CConfig* CConfig::GetInstance()
{
	static CConfig conf;
	return &conf;
}

bool CConfig::InitConfig(const char *a_pFileName)
{
	std::string strConfigName = Func::Truncate(a_pFileName, '.') + ".cfg";
	if (strConfigName.size() < 4)
	{
		SetErr("file name too short");
		return false;
	}
	std::string strContext;
	ReadFile(strConfigName.c_str(), strContext);
	if (strContext.size() == 0)
	{
		SetErr("context open failed");
		return false;
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
				m_DefaultValue[strType] = strValue;
				continue;
			}
			else if (strncmp(it->c_str(), "set float ", strlen("set float ")) == 0)
			{
				std::string str(*it, strlen("set float "));
				std::string strType(str.c_str(), str.find_first_of("="));
				std::string strValue(str, str.find_first_of("=") + 1);
				m_DefaultValue[strType] = strValue;
				continue;
			}
			else if (strncmp(it->c_str(), "set string ", strlen("set string ")) == 0)
			{
				std::string str(*it, strlen("set string "));
				std::string strType(str.c_str(), str.find_first_of("="));
				std::string strValue(str, str.find_first_of("=") + 1);
				m_DefaultValue[strType] = strValue;
				continue;
			}
			std::cout << "Unknown Type: " << *it << std::endl;
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
				std::cout << "Server cfg Err : " << *it << std::endl;
				continue;
			}
			m_Server[strServerName] = make_pair(strIP, nPort);
		}
	}
	return true;
}

bool CConfig::GetValue(const std::string& a_strKey, std::string& a_strValue)
{
	if (m_DefaultValue.find(a_strKey) == m_DefaultValue.end())
	{
		SetErr(std::string("Cannot find key: ") + a_strKey);
		return false;
	}
	a_strValue = m_DefaultValue[a_strKey];
	return true;
}

bool CConfig::GetValue(const std::string& a_strKey, int &a_Value)
{
	if (m_DefaultValue.find(a_strKey) == m_DefaultValue.end())
	{
		SetErr(std::string("Cannot find key: ") + a_strKey);
		return false;
	}
	a_Value = atoi(m_DefaultValue[a_strKey].c_str());
	return true;
}

std::map<std::string, std::pair<std::string, int>>& CConfig::GetServer()
{
	return m_Server;
}

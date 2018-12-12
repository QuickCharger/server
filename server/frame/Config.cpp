#include "Config.h"
#include <iostream>
#include <utility>
#include "json/json.h"
#include "base.h"

//CConfig* CConfig::GetInstance()
//{
//	static CConfig conf;
//	return &conf;
//}

bool CConfig::InitConfig(const char *a_pFileName)
{
	std::string strConfigName = Func::Truncate(a_pFileName, '.') + ".cfg";
	if (strConfigName.size() < 4)
	{
		SetErr("file name too short");
		return false;
	}
	std::string strContext;
	if (!CFile::ReadFile(strConfigName.c_str(), strContext))
	{
		std::string err = std::string("open ") + strConfigName + " failed. reason: " + strContext;
		SetErr(err);
		return false;
	}

	Json::Reader reader;

	if (!reader.parse(strContext.c_str(), m_root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		SetErr(std::string("file ") + strConfigName + " parse failed. not json ?");
		return false;
	}
	return true;

	//std::vector<std::string > vSplit;
	//Func::SplitString(strContext, "\n", vSplit);

	//for (auto it = vSplit.begin(); it != vSplit.end(); ++it)
	//{
	//	std::vector<std::string > vSplit;
	//	if (strncmp(it->c_str(), "set ", strlen("set ")) == 0)
	//	{
	//		if (strncmp(it->c_str(), "set int ", strlen("set int ")) == 0)
	//		{
	//			std::string str(*it, strlen("set int "));
	//			std::string strType(str.c_str(), str.find_first_of("="));
	//			std::string strValue(str, str.find_first_of("=") + 1);
	//			m_DefaultValue[strType] = strValue;
	//			continue;
	//		}
	//		else if (strncmp(it->c_str(), "set float ", strlen("set float ")) == 0)
	//		{
	//			std::string str(*it, strlen("set float "));
	//			std::string strType(str.c_str(), str.find_first_of("="));
	//			std::string strValue(str, str.find_first_of("=") + 1);
	//			m_DefaultValue[strType] = strValue;
	//			continue;
	//		}
	//		else if (strncmp(it->c_str(), "set string ", strlen("set string ")) == 0)
	//		{
	//			std::string str(*it, strlen("set string "));
	//			std::string strType(str.c_str(), str.find_first_of("="));
	//			std::string strValue(str, str.find_first_of("=") + 1);
	//			m_DefaultValue[strType] = strValue;
	//			continue;
	//		}
	//		std::cout << "Unknown Type: " << *it << std::endl;
	//		continue;
	//	}
	//	Func::SplitString(*it, "server ", vSplit);
	//	if (strncmp(it->c_str(), "server ", strlen("server ")) == 0)
	//	{
	//		std::string str(*it, strlen("server "));
	//		std::string strServerName(str.c_str(), str.find_first_of("="));
	//		std::string strValue(str, str.find_first_of("=") + 1);
	//		std::string strIP(strValue.c_str(), strValue.find_first_of(":"));
	//		std::string strPort(strValue, strValue.find_first_of(":") + 1);
	//		int nPort = atoi(strPort.c_str());
	//		if (strIP.size() == 0 || strPort.size() == 0)
	//		{
	//			std::cout << "Server cfg Err : " << *it << std::endl;
	//			continue;
	//		}
	//		m_Server[strServerName] = make_pair(strIP, nPort);
	//	}
	//}
	return true;
}

bool CConfig::GetValue(const std::string& a_strKey, std::string& a_strValue)
{
	if (!m_root.isMember(a_strKey))
		return false;

	const Json::Value& v = m_root[a_strKey];

	if (v.isString())
	{
		a_strValue = v.asString();
		return true;
	}
	return false;
}

bool CConfig::GetValue(const std::string& a_strKey, int &a_Value)
{
	i++;
	if (!m_root.isMember(a_strKey))
		return false;

	const Json::Value& v = m_root[a_strKey];

	if (v.isInt())
	{
		a_Value = v.asInt();
		return true;
	}

	if (v.isString())
	{
		a_Value =  atoi(v.asString().c_str());
		return true;
	}
	return false;
}

std::vector<std::pair<std::string, int>> CConfig::GetServerList()
{
	i++;
	std::vector<std::pair<std::string, int>> ret;

	if (!m_root.isMember("ServerList"))
	{
		return ret;
	}

	const Json::Value& v = m_root["ServerList"];

	if (!v.isArray())
	{
		return ret;
	}

	for (auto it = v.begin(); it != v.end(); ++it)
	{
		if (it->isMember("IP") && (*it)["IP"].isString()
			&& it->isMember("Port") && (*it)["Port"].isInt())
		{
			ret.push_back(make_pair((*it)["IP"].asString(), (*it)["Port"].asInt()));
		}
	}
	return ret;
}

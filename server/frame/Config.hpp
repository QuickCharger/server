#pragma once

//#include "common.h"

#include "base.h"
#include "json/json.h"

class CConfig : public CErrRecord
{
public:
	CConfig():CErrRecord()	{};
	virtual ~CConfig()	{};

public:
	//static CConfig* GetInstance();
	bool InitConfig(const char *a_pFileName)
	{
		std::string&& strConfigName = Func::Truncate(a_pFileName, '.') + ".cfg";
		if (strConfigName.size() < 4)
		{
			SetErr("file name too short");
			return false;
		}
		std::string strContext;
		if (!CFile::ReadFile(strConfigName.c_str(), strContext))
		{
			std::string err = std::string("open ") + strConfigName + " failed. reason: " + strContext;
			SetErr(std::move(err));
			return false;
		}

		Json::Reader reader;

		if (!reader.parse(strContext.c_str(), m_root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
		{
			SetErr(std::string("file ") + strConfigName + " parse failed. not json ?");
			return false;
		}
		return true;
	}

	bool GetValue(const std::string& a_strKey, std::string& a_strValue) const
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

	bool GetValue(const std::string& a_strKey, int &a_Value) const
	{
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
			a_Value = atoi(v.asString().c_str());
			return true;
		}
		return false;
	}

	std::vector<std::pair<std::string, int>> GetServerList() const
	{
		std::vector<std::pair<std::string, int>> ret;

		if (!m_root.isMember("ServerList"))
		{
			return std::move(ret);
		}

		const Json::Value& v = m_root["ServerList"];

		if (!v.isArray())
		{
			return std::move(ret);
		}

		for (auto it = v.begin(); it != v.end(); ++it)
		{
			if (it->isMember("IP") && (*it)["IP"].isString()
				&& it->isMember("Port") && (*it)["Port"].isInt())
			{
				ret.push_back(make_pair((*it)["IP"].asString(), (*it)["Port"].asInt()));
			}
		}
		return std::move(ret);
	}

private:
	Json::Value m_root;
};

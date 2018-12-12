#pragma once

#include <map>
#include <string>
#include <vector>
#include "base.h"
#include "json/json.h"

class CConfig : public CErrRecord
{
public:
	CConfig()	{};
	virtual ~CConfig()	{};

public:
	//static CConfig* GetInstance();
	bool InitConfig(const char *a_pFileName);
	bool GetValue(const std::string& a_strKey, std::string& a_strValue);
	bool GetValue(const std::string& a_strKey, int &a_Value);
	std::vector<std::pair<std::string, int>> GetServerList();
	int i = 0;
	
private:
	//std::map<std::string, std::string> m_DefaultValue;
	//std::map<std::string, std::pair<std::string, int>> m_Server;
	Json::Value m_root;
};

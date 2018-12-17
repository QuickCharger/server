#pragma once

#include <map>
#include <string>
#include <vector>
#include "base.h"
#include "json/json.h"

class CConfig : public CErrRecord
{
private:
	CConfig()	{};
	virtual ~CConfig()	{};

public:
	static CConfig* GetInstance();
	bool InitConfig(const char *a_pFileName);
	bool GetValue(const std::string& a_strKey, std::string& a_strValue) const;
	bool GetValue(const std::string& a_strKey, int &a_Value) const;
	std::vector<std::pair<std::string, int>> GetServerList() const;

private:
	Json::Value m_root;
};

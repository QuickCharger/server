#ifndef _FRAME_CONFIG_H_
#define _FRAME_CONFIG_H_

#include <map>
#include <string>
#include "ErrRecord.h"

class CConfig : public CErrRecord<CConfig>
{
	CConfig()	{};
	virtual ~CConfig()	{};

public:
	static CConfig* GetInstance();
	bool InitConfig(const char *a_pFileName);
	bool GetValue(const std::string& a_strKey, std::string& a_strValue);
	bool GetValue(const std::string& a_strKey, int &a_Value);
	std::map<std::string, std::pair<std::string, int>>& GetServer();
	
private:
	std::map<std::string, std::string> m_DefaultValue;
	std::map<std::string, std::pair<std::string, int>> m_Server;
};

#endif

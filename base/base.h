#ifndef _BASE_H_
#define _BASE_H_

#include <list>
#include <map>
#include <string>
#include <utility>
#include <vector>

const char* GetLastErr();
void ReadFile(const char* a_pFileName, std::string& a_strContext);
//bool InitConfig(const char* a_pFileName, std::map<std::string, std::string>& a_mValue);

void InitConfig(const std::vector<std::string >& a_vContext, std::map<std::string, std::string>& a_mDefaultValue, std::map<std::string, std::pair<std::string, int>>& a_mServer);

//void ConnectServer(std::list<std::pair<std::string, int>>& serverConfig);
#endif

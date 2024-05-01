#pragma once

#include <string>
#include <vector>
#include <utility>

//class CConfig {
//public:
//	void Set(std::string&& k, std::string&& v) {
//		values.push_back(std::make_pair(k, v));
//	}
//
//	bool Get(const std::string& k, std::string& outV ) {
//		std::string ret;
//		for (auto it : values) {
//			if (it.first == k) {
//				outV = it.second;
//				return true;
//			}
//		}
//		return false;
//	}
//private:
//	std::vector < std::pair < std::string, std::string >> values;
//};

struct Config {
	int port = 0;
};

class IRunnable {
public:
	virtual int Init() = 0;
	virtual int Run() = 0;
	virtual int Stop() = 0;
	//CConfig config;
	Config config;
};

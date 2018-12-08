#pragma once

#include <string>

class CErrRecord
{
public:
	const std::string& GetErr() const { return m_err; }
	void SetErr(const std::string& err) { m_err = err; }
private:
	std::string m_err;
};

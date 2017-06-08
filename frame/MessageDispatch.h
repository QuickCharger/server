#ifndef _FRAME_MESSAGEDISPTACH_H_
#define _FRAME_MESSAGEDISPTACH_H_

#include <functional>
#include <map>

class CSession;

class CMessageDispatch
{
public:
	CMessageDispatch() {};
	virtual ~CMessageDispatch()
	{
		m_mFuncCB.clear();
	};

	typedef std::function< void(CSession* a_pSession, const char *)> FuncCB;
public:
	bool RegistMessage(int a_nCode, FuncCB a_nFunc)
	{
		FuncCB* cb = getFunc(a_nCode);
		if (cb != nullptr)
		{
			return false;
		}
		m_mFuncCB[a_nCode] = a_nFunc;
		return true;
	};
	
	bool DeRegistMessage(int a_nCode)
	{
		FuncCB* cb = getFunc(a_nCode);
		if (cb == nullptr)
		{
			return false;
		}
		m_mFuncCB.erase(a_nCode);
		return true;
	}

	bool DoMessageCB(CSession* a_pSession, int a_nCode, const char * a_pCh)
	{
		FuncCB* cb = getFunc(a_nCode);
		if (cb == nullptr)
		{
			return false;
		}
		(*cb)(a_pSession, a_pCh);
		return true;
	}

private:
	FuncCB* getFunc(int a_nCode)
	{
		std::map<int, FuncCB>::iterator it = m_mFuncCB.find(a_nCode);
		if (it == m_mFuncCB.end())
		{
			return nullptr;
		}
		return &(it->second);
	}

	std::map<int, FuncCB> m_mFuncCB;
};

#endif

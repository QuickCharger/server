#include "IServer.h"
#include "session.h"

//IServer::IServer(event_base* a_pEventBase, CSession *a_pSession)
//	: m_pEventBase(a_pEventBase)
//	, m_pSession(a_pSession)
//{
//	m_pSession->SetReadCB([this](void* a_pArg){ OnReadCB(a_pArg); });
//	m_pSession->SetWriteCB([this](void* a_pArg){ OnWriteCB(a_pArg); });
//	m_pSession->SetErrorCB([this](void* a_pArg){ OnErrorCB(a_pArg); });
//	m_pSession->SetPackCB([this](const char *a_pSource, int& a_nLength, char **a_pDest) { OnPackCB(a_pSource, a_nLength, a_pDest); });
//	m_pSession->SetUnPackCB([this](const char *a_pSource, int a_nLength)->bool { return OnUnPackCB(a_pSource, a_nLength); });
//};

IServer::IServer()
{

}

IServer::~IServer()
{
	//m_pEventBase = nullptr;
	//if (m_pSession != nullptr)
	//{
	//	delete m_pSession;
	//	m_pSession = nullptr;
	//}
};

void IServer::SetCallBack()
{
	//m_pSession->SetReadCB([this](void* a_pArg){ OnReadCB(a_pArg); });
	//m_pSession->SetWriteCB([this](void* a_pArg){ OnWriteCB(a_pArg); });
	//m_pSession->SetErrorCB([this](void* a_pArg){ OnErrorCB(a_pArg); });
//	m_pSession->SetPackCB([this](const char *a_pSource, int& a_nLength, char **a_pDest) { OnPackCB(a_pSource, a_nLength, a_pDest); });
//	m_pSession->SetUnPackCB([this](const char *a_pSource, int a_nLength)->bool { return OnUnPackCB(a_pSource, a_nLength); });
}

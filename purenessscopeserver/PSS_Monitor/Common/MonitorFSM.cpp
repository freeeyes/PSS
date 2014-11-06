#include "MonitorFSM.h"

MonitorFSM::MonitorFSM()
{
	m_pClientMessage = NULL;
	m_pServerInfo    = NULL;
}

MonitorFSM::~MonitorFSM()
{
	SAFE_DELETE(m_pClientMessage);
}

IClientMessage* MonitorFSM::GetClientMessage()
{
	return (IClientMessage* )m_pClientMessage;
}

void MonitorFSM::Init(_ServerInfo* pServerInfo, _MailAlert* pMailAlert)
{
	m_pServerInfo = pServerInfo;

	if(NULL == m_pClientMessage)
	{
		m_pClientMessage = new CClientMessage();
		m_pClientMessage->Init(pMailAlert);
		m_pClientMessage->SetServerName(pServerInfo->m_szServerName);
	}
}

int MonitorFSM::GetServerID()
{
	if(NULL == m_pClientMessage)
	{
		return m_pClientMessage->GetServerID();
	}
	else
	{
		return 0;
	}
}

char* MonitorFSM::GetKey()
{
	if(NULL != m_pServerInfo)
	{
		return m_pServerInfo->m_szKey;
	}
	else
	{
		return NULL;
	}
}
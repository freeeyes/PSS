#include "MonitorFSM.h"

MonitorFSM::MonitorFSM()
{
	m_pClientMessage = NULL;
	m_szIP[0]        = '\0';
	m_szKey[0]       = '\0';
}

MonitorFSM::~MonitorFSM()
{
	SAFE_DELETE(m_pClientMessage);
}

IClientMessage* MonitorFSM::GetClientMessage()
{
	return (IClientMessage* )m_pClientMessage;
}

void MonitorFSM::Init(const char* pIP, uint32 u4Port, const char* pKey)
{
	sprintf_safe(m_szIP, MAX_BUFF_50, "%s", pIP);
	sprintf_safe(m_szKey, MAX_BUFF_50, "%s", pKey);
	m_u4Port = u4Port;

	if(NULL == m_pClientMessage)
	{
		m_pClientMessage = new CClientMessage();
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
	return m_szKey;
}
#include "MonitorFSMManager.h"

#ifdef WIN32
#include "ClientProConnectManager.h"
#else
#include "ClientReConnectManager.h"
#endif


CMonitorFSMManager::CMonitorFSMManager()
{
}

CMonitorFSMManager::~CMonitorFSMManager()
{
	Close();
}

MonitorFSM* CMonitorFSMManager::Create(_ServerInfo* pServerInfo, _MailAlert* pMailAlert)
{
	MonitorFSM* pMonitorFSM = new MonitorFSM();
	if(NULL != pMonitorFSM)
	{
		pMonitorFSM->Init(pServerInfo, pMailAlert);
		m_vecMonitorFSM.push_back(pMonitorFSM);
		return pMonitorFSM;
	}
	else
	{
		return NULL;
	}
}

void CMonitorFSMManager::Close()
{
	for(int i = 0; i < (int)m_vecMonitorFSM.size(); i++)
	{
		MonitorFSM* pMonitorFSM = (MonitorFSM* )m_vecMonitorFSM[i];
		if(NULL != pMonitorFSM)
		{
			SAFE_DELETE(pMonitorFSM);
		}
	}

	m_vecMonitorFSM.clear();
}

uint32 CMonitorFSMManager::GetCount()
{
	return (uint32)m_vecMonitorFSM.size();
}

IClientMessage* CMonitorFSMManager::GetClientMessage(uint32 u4Index)
{
	if(u4Index < (uint32)m_vecMonitorFSM.size())
	{
		return m_vecMonitorFSM[u4Index]->GetClientMessage();
	}
	else
	{
		return NULL;
	}
}

int CMonitorFSMManager::handle_timeout(const ACE_Time_Value& tv, const void* arg)
{
	for(int i = 0; i < (int)m_vecMonitorFSM.size(); i++)
	{
#ifdef WIN32
		char szMonitorText[MAX_BUFF_100] = {'\0'};
		MonitorFSM* pMonitorFSM = (MonitorFSM* )m_vecMonitorFSM[i];
		int nServerID = pMonitorFSM->GetServerID();
		sprintf_safe(szMonitorText, 100, "%s Monitor -a", pMonitorFSM->GetKey());
		
		char szSendMessage[MAX_BUFF_200] = {'\0'};
		int nSendLen = (int)ACE_OS::strlen(szMonitorText);
		ACE_OS::memcpy(szSendMessage, &nSendLen, sizeof(int));
		ACE_OS::memcpy(&szSendMessage[4], &szMonitorText, nSendLen);
		
		App_ClientProConnectManager::instance()->SendData(nServerID, 
			szSendMessage, 
			nSendLen + sizeof(int), false);

#else
		char szMonitorText[MAX_BUFF_100] = {'\0'};
		MonitorFSM* pMonitorFSM = (MonitorFSM* )m_vecMonitorFSM[i];
		int nServerID = pMonitorFSM->GetServerID();
		sprintf_safe(szMonitorText, 100, "%s Monitor -a", pMonitorFSM->GetKey());

		char szSendMessage[MAX_BUFF_200] = {'\0'};
		int nSendLen = (int)ACE_OS::strlen(szMonitorText);
		ACE_OS::memcpy(szSendMessage, &nSendLen, sizeof(int));
		ACE_OS::memcpy(&szSendMessage[4], &szMonitorText, nSendLen);

		App_ClientReConnectManager::instance()->SendData(nServerID, 
			szSendMessage, 
			nSendLen + sizeof(int), false);

#endif
	}

	return 0;
}

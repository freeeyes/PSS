#include "ListManager.h"

ClistManager::ClistManager()
{
	m_szMD5[0] = '\0';
}

ClistManager::~ClistManager()
{

}

void ClistManager::Add_LG_Info(uint32 u4ConnectID, uint32 u4LGID, const char* pIP, uint32 u4Port)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

	_LG_Info* pInfo = Get_LG_Info_By_LGID(u4LGID);
	if(NULL == pInfo)
	{
		_LG_Info objNewInfo;
		objNewInfo.m_u4ConnectID = u4ConnectID;
		objNewInfo.m_u4LGID      = u4LGID;
		objNewInfo.m_u4Port      = u4Port;
		sprintf_safe(objNewInfo.m_szIP, 50, "%s", pIP);
		m_vecLGInfo.push_back(objNewInfo);
	}
	else
	{
		pInfo->m_u4ConnectID = u4ConnectID;
		pInfo->m_u4LGID      = u4LGID;
		pInfo->m_u4Port      = u4Port;
		sprintf_safe(pInfo->m_szIP, 50, "%s", pIP);
	}

	Compare_MD5();
}

_LG_Info* ClistManager::Get_LG_Info_By_LGID(uint32 u4LGID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

	for(uint32 u4Index = 0; u4Index < (uint32)m_vecLGInfo.size(); u4Index++)
	{
		if(m_vecLGInfo[u4Index].m_u4LGID == u4LGID)
		{
			return &m_vecLGInfo[u4Index];
		}
	}

	return NULL;
}

_LG_Info* ClistManager::Get_LG_Info_By_ConnectID(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

	for(uint32 u4Index = 0; u4Index < (uint32)m_vecLGInfo.size(); u4Index++)
	{
		if(m_vecLGInfo[u4Index].m_u4ConnectID == u4ConnectID)
		{
			return &m_vecLGInfo[u4Index];
		}
	}

	return NULL;
}

_LG_Info* ClistManager::Get_LG_Info_By_Index(uint32 u4Index)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

	if(u4Index >= (uint32)m_vecLGInfo.size())
	{
		return NULL;
	}
	else
	{
		return &m_vecLGInfo[u4Index];
	}
}

void ClistManager::Del_LG_Info_By_ConnectID(uint32 u4ConnectID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

	for(vecLGInfo::iterator b = m_vecLGInfo.begin(); b != m_vecLGInfo.end(); b++)
	{
		if(b->m_u4ConnectID == u4ConnectID)
		{
			m_vecLGInfo.erase(b);
			return;
		}
	}

	Compare_MD5();
}

uint32 ClistManager::Get_LG_Count()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

	return (uint32)m_vecLGInfo.size();
}

void ClistManager::Get_All_LG_List(IBuffPacket* pbpList, uint32& u4Count)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

	if(pbpList == NULL)
	{
		return;
	}

	u4Count = Get_LG_Count();

	for(uint32 i = 0; i < u4Count; i++)
	{
		_LG_Info* pInfo = Get_LG_Info_By_Index(i);
		_VCHARS_STR strIP;
		uint8 u1Len = (uint8)strlen(pInfo->m_szIP);
		strIP.SetData(pInfo->m_szIP, u1Len);	
		
		(*pbpList) << pInfo->m_u4LGID;
		(*pbpList) << strIP;
		(*pbpList) << pInfo->m_u4Port;
	}
}

void ClistManager::Compare_MD5()
{
	CMD5 objMD5;
	objMD5.ENCODE_DATA((char* )&m_vecLGInfo, sizeof(m_vecLGInfo), m_szMD5);
}

char* ClistManager::Get_MD5_Data()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

	return (char* )m_szMD5;
}

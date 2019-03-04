#include "ListManager.h"

ClistManager::ClistManager()
{
    sprintf_safe(m_szMD5, 33, "%s", LG_LIST_MD5);
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

char* ClistManager::Get_MD5_Data()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

    return (char* )m_szMD5;
}

void ClistManager::Set_MD5_Data(const char* pMD5)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

    sprintf_safe(m_szMD5, 33, "%s", pMD5);
}

void ClistManager::Clear()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

    m_vecLGInfo.clear();
    sprintf_safe(m_szMD5, 33, "%s", LG_LIST_MD5);
}

void ClistManager::SaveList()
{
    //将当前列表信息存入存储文件
    FILE* pFile = fopen(LS_LIST_FILENAME, "w+");

    if(NULL == pFile)
    {
        OUR_DEBUG((LM_ERROR, "[ClistManager::SaveList](%s)Write Ini fail.\n", LS_LIST_FILENAME));
        return;
    }

    char szTemp[200] = {'\0'};
    sprintf_safe(szTemp, 200, "[List]\nMD5=%s\nCount=%d\n",
                 Get_MD5_Data(),
                 Get_LG_Count());
    fwrite(szTemp, sizeof(char), strlen(szTemp), pFile);

    for(uint32 i = 0; i < Get_LG_Count(); i++)
    {
        sprintf_safe(szTemp, 200, "LGID%d=%d\nLGIP%d=%s\nLGPort%d=%d",
                     i,
                     m_vecLGInfo[i].m_u4LGID,
                     i,
                     m_vecLGInfo[i].m_szIP,
                     i,
                     m_vecLGInfo[i].m_u4Port);
        fwrite(szTemp, sizeof(char), strlen(szTemp), pFile);
    }

    fclose(pFile);
}

void ClistManager::ReadList()
{
    //从存储文件中还原List列表信息
    Clear();

    dictionary* pDictionary = NULL;
    pDictionary = iniparser_load(LS_LIST_FILENAME);

    if(NULL == pDictionary)
    {
        OUR_DEBUG((LM_ERROR, "[ClistManager::ReadList](%s)Read Ini fail.\n", LS_LIST_FILENAME));
        return;
    }

    char* pData = iniparser_getstring(pDictionary, "List:MD5", NULL);

    if(NULL != pData)
    {
        sprintf_safe(m_szMD5, 33, "%s", pData);
    }

    uint32 u4Count = (uint32)iniparser_getint(pDictionary, "List:Count", 0);

    char   szIP[50] = {'\0'};
    uint32 u4Port   = 0;
    uint32 u4LGID   = 0;

    char szTempName[50] = {'\0'};

    for(uint32 i = 0; i < u4Count; i++)
    {
        sprintf_safe(szTempName, 50, "List:LGID%d", i);
        u4LGID = (uint32)iniparser_getint(pDictionary, szTempName, 0);
        sprintf_safe(szTempName, 50, "List:LGIP%d", i);
        pData = iniparser_getstring(pDictionary, szTempName, NULL);
        {
            sprintf_safe(szIP, 50, "%s", pData);
        }
        sprintf_safe(szTempName, 50, "List:LGPort%d", i);
        u4Port = (uint32)iniparser_getint(pDictionary, szTempName, 0);;
        Add_LG_Info(0, u4LGID, szIP, u4Port);
    }
}

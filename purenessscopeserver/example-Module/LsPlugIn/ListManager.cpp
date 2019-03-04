#include "ListManager.h"

ClistManager::ClistManager()
{
    m_szMD5[0] = '\0';
}

ClistManager::~ClistManager()
{

}

void ClistManager::Add_LG_Info(uint32 u4ConnectID, uint32 u4LGID, const char* pIP, uint32 u4Port, const char* pMD5, const char* pSession, uint16 u2Version)
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
        sprintf_safe(objNewInfo.m_szMD5, 33, "%s", pMD5);
        sprintf_safe(objNewInfo.m_szSession, 33, "%s", pSession);
        m_vecLGInfo.push_back(objNewInfo);
        objNewInfo.m_u2Version   = u2Version;
    }
    else
    {
        pInfo->m_u4ConnectID = u4ConnectID;
        pInfo->m_u4LGID      = u4LGID;
        pInfo->m_u4Port      = u4Port;
        sprintf_safe(pInfo->m_szIP, 50, "%s", pIP);
        sprintf_safe(pInfo->m_szMD5, 33, "%s", pMD5);
        sprintf_safe(pInfo->m_szSession, 33, "%s", pSession);
        pInfo->m_u2Version   = u2Version;
    }

    Compare_MD5();

    Check_LG_Target(u4ConnectID, u4LGID, pIP, u4Port);
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
    //CMD5 objMD5;
    //objMD5.ENCODE_DATA((char* )&m_vecLGInfo, sizeof(m_vecLGInfo), m_szMD5);
    unsigned char uszMD5[16] = {'\0'};

    MD5_CTX obj_MD5_CTX;
    MD5Init(&obj_MD5_CTX);
    MD5Update(&obj_MD5_CTX, (unsigned char* )&m_vecLGInfo, sizeof(m_vecLGInfo));
    MD5Final(&obj_MD5_CTX, uszMD5);

    //将数组转换为字符串
    char szDigit[3] = {'\0'};

    for(int i = 0; i < 16; i++)
    {
        sprintf_safe(szDigit, 3, "%02x", (char)uszMD5[i]);
        memcpy_safe((char* )&uszMD5[i*2], 32 - i*2, szDigit, 2);
    }

}

char* ClistManager::Get_MD5_Data()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

    return (char* )m_szMD5;
}

void ClistManager::Clear()
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> WGuard(m_ThreadLock);

    m_vecLGInfo.clear();
    m_szMD5[0] = '\0';
}

void ClistManager::ReadList()
{
    m_vecLGTargetInfo.clear();

    dictionary* pDictionary = NULL;
    pDictionary = iniparser_load(LS_LIST_FILENAME);

    if(NULL == pDictionary)
    {
        OUR_DEBUG((LM_ERROR, "[ClistManager::ReadList](%s)Read Ini fail.\n", LS_LIST_FILENAME));
        return;
    }

    uint32 u4Count = (uint32)iniparser_getint(pDictionary, "List:Count", 0);

    char szTempName[50] = {'\0'};

    for(uint32 i = 0; i < u4Count; i++)
    {
        _LG_Target_Info objTarhgetInfo;
        sprintf_safe(szTempName, 50, "List:LGID%d", i);
        objTarhgetInfo.m_u4LGID = (uint32)iniparser_getint(pDictionary, szTempName, 0);
        sprintf_safe(szTempName, 50, "List:LGIP%d", i);
        char* pData = iniparser_getstring(pDictionary, szTempName, NULL);
        {
            sprintf_safe(objTarhgetInfo.m_szIP, 50, "%s", pData);
        }
        sprintf_safe(szTempName, 50, "List:LGPort%d", i);
        objTarhgetInfo.m_u4Port = (uint32)iniparser_getint(pDictionary, szTempName, 0);;
        m_vecLGTargetInfo.push_back(objTarhgetInfo);
    }
}

void ClistManager::Check_LG_Target(uint32 u4ConnectID, uint32 u4LGID, const char* pIP, uint32 u4Port)
{
    for(int i = 0; i < (int)m_vecLGTargetInfo.size(); i++)
    {
        if(m_vecLGTargetInfo[i].m_u4LGID == u4LGID &&
           m_vecLGTargetInfo[i].m_u4Port == u4Port &&
           ACE_OS::strcmp(m_vecLGTargetInfo[i].m_szIP, pIP) == 0)
        {
            m_vecLGTargetInfo[i].m_nConnectID = (int)u4ConnectID;
            break;
        }
    }
}

bool ClistManager::Get_All_Target_list_Finish()
{
    bool blRet = true;

    for(int i = 0; i < (int)m_vecLGTargetInfo.size(); i++)
    {
        if(m_vecLGTargetInfo[i].m_nConnectID == -1)
        {
            blRet = false;
            break;
        }
    }

    return blRet;
}
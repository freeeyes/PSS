#include "TcpRedirection.h"

CForwardManager::CForwardManager() : m_nActive(0), m_nNeedLoad(0)
{

}

void CForwardManager::Close()
{
    m_vecForwardInfo.clear();

    for (auto it = m_mapForwardConnectList.begin(); it != m_mapForwardConnectList.end(); ++it)
    {
        CForwardConnectInfo* pForwardConnectInfo = it->second;
        SAFE_DELETE(pForwardConnectInfo);
    }

    m_mapForwardConnectList.clear();
}

int CForwardManager::Init(int nNeedLoad)
{
    //读取配置文件
    CXmlOpeation objXmlOperation;

    bool blFileExist = objXmlOperation.Init(FORWARD_XML);

    if (false == blFileExist)
    {
        //file is not exist.
        return -3;
    }

    objXmlOperation.Read_XML_Data_Single_Int("Info", "Active", m_nActive);

    m_nNeedLoad = nNeedLoad;

    if (0 == nNeedLoad && 0 == m_nActive)
    {
        return -4;
    }

    Close();

    TiXmlElement* pSource = NULL;
    TiXmlElement* pTarget = NULL;

    string strSource;
    string strTarget;

    while (objXmlOperation.Read_XML_Data_Multiple_String("Forward", "EventID", strSource, pSource)
           && objXmlOperation.Read_XML_Data_Multiple_String("Timer", "Name", strTarget, pTarget))
    {
        //写入配置文件
        CForwardInfo objForwardInfo;

        objForwardInfo.m_strSource = strSource;
        objForwardInfo.m_strTarget = strTarget;

        m_vecForwardInfo.push_back(objForwardInfo);
    }

    //写入类型
    int nSize = (int)m_vecForwardInfo.size();

    for (int i = 0; i < nSize; i++)
    {
        CForwardConnectInfo* pForwardConnectInfo = new CForwardConnectInfo();
        pForwardConnectInfo->m_strSource = m_vecForwardInfo[i].m_strSource;
        pForwardConnectInfo->m_strTarget = m_vecForwardInfo[i].m_strTarget;

        mapForwardConnectList::iterator f = m_mapForwardConnectList.find(pForwardConnectInfo->m_strSource);

        if (f == m_mapForwardConnectList.end())
        {
            m_mapForwardConnectList.insert(std::make_pair(pForwardConnectInfo->m_strSource, pForwardConnectInfo));
        }

        pForwardConnectInfo = new CForwardConnectInfo();
        pForwardConnectInfo->m_strSource = m_vecForwardInfo[i].m_strTarget;
        pForwardConnectInfo->m_strTarget = m_vecForwardInfo[i].m_strSource;

        f = m_mapForwardConnectList.find(pForwardConnectInfo->m_strSource);

        if (f == m_mapForwardConnectList.end())
        {
            m_mapForwardConnectList.insert(std::make_pair(pForwardConnectInfo->m_strSource, pForwardConnectInfo));
        }
    }

    return 0;
}

void CForwardManager::ConnectRegedit(const char* pIP, int nPort, ENUM_FORWARD_TYPE em_type)
{
    if (0 == m_nNeedLoad || 0 == m_nActive)
    {
        return;
    }

    char szSource[MAX_BUFF_100] = { '\0' };

    sprintf_safe(szSource, MAX_BUFF_100, "%s:%d", pIP, nPort);

    Check_Connect_IP(szSource, em_type, 1);
}

void CForwardManager::ConnectRegedit(const char* pName, ENUM_FORWARD_TYPE em_type)
{
    if (0 == m_nNeedLoad || 0 == m_nActive)
    {
        return;
    }

    Check_Connect_IP(pName, em_type, 1);
}

void CForwardManager::DisConnectRegedit(const char* pIP, int nPort, ENUM_FORWARD_TYPE em_type)
{
    if (0 == m_nNeedLoad || 0 == m_nActive)
    {
        return;
    }

    char szSource[MAX_BUFF_100] = { '\0' };

    sprintf_safe(szSource, MAX_BUFF_100, "%s:%d", pIP, nPort);

    Check_Connect_IP(szSource, em_type, 0);
}

void CForwardManager::DisConnectRegedit(const char* pName, ENUM_FORWARD_TYPE em_type)
{
    if (0 == m_nNeedLoad || 0 == m_nActive)
    {
        return;
    }

    Check_Connect_IP(pName, em_type, 0);
}

void CForwardManager::SendData(const char* pIP, int nPort, ACE_Message_Block* pmb)
{
    ACE_UNUSED_ARG(pIP);
    ACE_UNUSED_ARG(nPort);
    ACE_UNUSED_ARG(pmb);
}

void CForwardManager::RecvData(const char* pIP, int nPort, ACE_Message_Block* pmb)
{
    ACE_UNUSED_ARG(pIP);
    ACE_UNUSED_ARG(nPort);
    ACE_UNUSED_ARG(pmb);
}

void CForwardManager::Check_Connect_IP(const char* pName, ENUM_FORWARD_TYPE em_type, int ConnectState)
{
    mapForwardConnectList::iterator f = m_mapForwardConnectList.find((string)pName);

    if (f != m_mapForwardConnectList.end())
    {
        CForwardConnectInfo* pForwardConnectInfo = (CForwardConnectInfo*)f->second;
        pForwardConnectInfo->m_emForwardType = em_type;
        pForwardConnectInfo->m_u1ConnectState = ConnectState;
    }
}

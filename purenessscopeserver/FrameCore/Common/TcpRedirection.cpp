#include "TcpRedirection.h"

void CForwardManager::Close()
{
    m_vecForwardInfo.clear();

    m_mapForwardConnectList.clear();
}

int CForwardManager::Init()
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

    if (0 == m_nActive)
    {
        return -4;
    }

    Close();

    TiXmlElement* pSource = nullptr;
    TiXmlElement* pTarget = nullptr;

    string strSource;
    string strTarget;

    while (objXmlOperation.Read_XML_Data_Multiple_String("Forward", "Source", strSource, pSource)
           && objXmlOperation.Read_XML_Data_Multiple_String("Forward", "Target", strTarget, pTarget))
    {
        //写入配置文件
        CForwardInfo objForwardInfo;

        objForwardInfo.m_strSource = strSource;
        objForwardInfo.m_strTarget = strTarget;

        m_vecForwardInfo.push_back(objForwardInfo);
    }

    //写入类型
    auto nSize = (int)m_vecForwardInfo.size();

    for (int i = 0; i < nSize; i++)
    {
        auto pForwardConnectInfo = std::make_shared<CForwardConnectInfo>();
        pForwardConnectInfo->m_strSource = m_vecForwardInfo[i].m_strSource;
        pForwardConnectInfo->m_strTarget = m_vecForwardInfo[i].m_strTarget;

        m_mapForwardConnectList[pForwardConnectInfo->m_strSource] = pForwardConnectInfo;

        pForwardConnectInfo = std::make_shared<CForwardConnectInfo>();
        pForwardConnectInfo->m_strSource = m_vecForwardInfo[i].m_strTarget;
        pForwardConnectInfo->m_strTarget = m_vecForwardInfo[i].m_strSource;

        m_mapForwardConnectList[pForwardConnectInfo->m_strSource] = pForwardConnectInfo;
    }

    return 0;
}

string CForwardManager::ConnectRegedit(const char* pIP, int nPort, ENUM_FORWARD_TYPE em_type, IHandler* pHandler)
{
    if (0 == m_nActive)
    {
        return "";
    }

    std::stringstream ss_format;
    ss_format << pIP << ":" << nPort;
    string strSource = ss_format.str();

    return Check_Connect_IP(strSource.c_str(), em_type, 1, pHandler);
}

string CForwardManager::ConnectRegedit(const char* pName, ENUM_FORWARD_TYPE em_type, IHandler* pHandler)
{
    if (0 == m_nActive)
    {
        return "";
    }

    return Check_Connect_IP(pName, em_type, 1, pHandler);
}

void CForwardManager::DisConnectRegedit(const char* pIP, int nPort, ENUM_FORWARD_TYPE em_type)
{
    if (0 == m_nActive)
    {
        return;
    }

    std::stringstream ss_format;
    ss_format << pIP << ":" << nPort;
    string strSource = ss_format.str();

    Check_Connect_IP(strSource.c_str(), em_type, 0);
}

void CForwardManager::DisConnectRegedit(const char* pName, ENUM_FORWARD_TYPE em_type)
{
    if (0 == m_nActive)
    {
        return;
    }

    Check_Connect_IP(pName, em_type, 0);
}

void CForwardManager::SendData(const string& strTarget, ACE_Message_Block* pmb)
{
    if (0 == m_nActive)
    {
        return;
    }

    auto pIHandler = Get_Device_Handler(strTarget);

    if (nullptr != pIHandler)
    {
        pIHandler->Device_Send_Data(pmb->rd_ptr(), pmb->length());
        pmb->wr_ptr(pmb->length());
    }
}

void CForwardManager::AddForward(const string& strSource, const string& strTarget)
{
    if (0 == m_nActive)
    {
        m_nActive = 1;
    }

    //写入配置文件
    CForwardInfo objForwardInfo;

    objForwardInfo.m_strSource = strSource;
    objForwardInfo.m_strTarget = strTarget;

    m_vecForwardInfo.push_back(objForwardInfo);

    auto pForwardConnectInfo = std::make_shared<CForwardConnectInfo>();
    pForwardConnectInfo->m_strSource = objForwardInfo.m_strSource;
    pForwardConnectInfo->m_strTarget = objForwardInfo.m_strTarget;

    m_mapForwardConnectList[pForwardConnectInfo->m_strSource] = pForwardConnectInfo;

    pForwardConnectInfo = std::make_shared<CForwardConnectInfo>();
    pForwardConnectInfo->m_strSource = objForwardInfo.m_strTarget;
    pForwardConnectInfo->m_strTarget = objForwardInfo.m_strSource;

    m_mapForwardConnectList[pForwardConnectInfo->m_strSource] = pForwardConnectInfo;
}

string CForwardManager::Check_Connect_IP(const char* pName, ENUM_FORWARD_TYPE em_type, int ConnectState, IHandler* pHandler)
{
    mapForwardConnectList::iterator f = m_mapForwardConnectList.find((string)pName);

    if (f != m_mapForwardConnectList.end())
    {
        auto pForwardConnectInfo = f->second;
        pForwardConnectInfo->m_emForwardType  = em_type;
        pForwardConnectInfo->m_u1ConnectState = (uint8)ConnectState;
        pForwardConnectInfo->m_pHandler       = pHandler;
        return pForwardConnectInfo->m_strTarget;
    }
    else
    {
        return "";
    }
}

IHandler* CForwardManager::Get_Device_Handler(const string& strTarget)
{
    mapForwardConnectList::iterator f = m_mapForwardConnectList.find(strTarget);

    if (f != m_mapForwardConnectList.end() && 1 == f->second->m_u1ConnectState)
    {
        return f->second->m_pHandler;
    }

    return nullptr;
}

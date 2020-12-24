#include "ProControlListen.h"

CProControlListen::CProControlListen()
{
}

CProControlListen::~CProControlListen()
{
}

bool CProControlListen::AddListen( const char* pListenIP, uint16 u2Port, uint8 u1IPType, int nPacketParseID)
{
    bool blState = App_ProConnectAcceptManager::instance()->CheckIPInfo(pListenIP, u2Port);

    if(true == blState)
    {
        //当前监听已经存在，不可以重复建设
        PSS_LOGGER_DEBUG("[CProControlListen::AddListen]({0}:{1}) is exist.", pListenIP, u2Port);
        return false;
    }

    //创建一个新的accept对象
    ProConnectAcceptor* pProConnectAcceptor = App_ProConnectAcceptManager::instance()->GetNewConnectAcceptor();

    if(nullptr == pProConnectAcceptor)
    {
        PSS_LOGGER_DEBUG("[CProControlListen::AddListen]({0}:{1}) new ConnectAcceptor error.", pListenIP, u2Port);
        return false;
    }

    ACE_INET_Addr listenAddr;
    //判断IPv4还是IPv6
    int nErr = 0;

    if(u1IPType == TYPE_IPV4)
    {
        nErr = listenAddr.set(u2Port, pListenIP);
    }
    else
    {
        nErr = listenAddr.set(u2Port, pListenIP, 1, PF_INET6);
    }

    if(nErr != 0)
    {
        PSS_LOGGER_DEBUG("[CProControlListen::AddListen]({0}:{1})set_address error[{2}].", pListenIP, u2Port, errno);
        return false;
    }

    //创建新的监听
    //设置监听IP信息
    pProConnectAcceptor->SetListenInfo(pListenIP, u2Port);
    pProConnectAcceptor->SetPacketParseInfoID(nPacketParseID);

    ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor();

    if(nullptr == pProactor)
    {
        PSS_LOGGER_DEBUG("[CProControlListen::AddListen]App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is nullptr.");
        return false;
    }

    int nRet = pProConnectAcceptor->open(listenAddr, 0, 1, GetXmlConfigAttribute(xmlNetWorkMode)->BackLog, 1, pProactor);

    if(-1 == nRet)
    {
        PSS_LOGGER_DEBUG("[CProControlListen::AddListen] Listen from [{0}:{1}] error({2}).",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno);
        return false;
    }

    PSS_LOGGER_DEBUG("[CProControlListen::AddListen]({0}:{1})Add Listen success.", pListenIP, u2Port);

    return true;
}

bool CProControlListen::DelListen(const char* pListenIP, uint16 u2Port)
{
    bool blState = App_ProConnectAcceptManager::instance()->CheckIPInfo(pListenIP, u2Port);

    if(false == blState)
    {
        //当前监听已经存在，不可以重复建设
        PSS_LOGGER_DEBUG("[CProControlListen::AddListen]({0}:{1}) is exist.", pListenIP, u2Port);
        return false;
    }

    return App_ProConnectAcceptManager::instance()->Close(pListenIP, u2Port);
}

uint32 CProControlListen::GetListenCount()
{
    m_vecListenList.clear();

    if (0 == App_ProConnectAcceptManager::instance()->GetCount())
    {
        //监控尚未启动，需要从配置文件中获取
        int nServerPortCount = (int)GetXmlConfigAttribute(xmlTCPServerIPs)->vec.size();

        for (int i = 0; i < nServerPortCount; i++)
        {
            _ControlInfo objInfo;

            objInfo.m_strListenIP = GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].ip;
            objInfo.m_u4Port = GetXmlConfigAttribute(xmlTCPServerIPs)->vec[i].port;
            m_vecListenList.push_back(objInfo);
        }
    }
    else
    {
        for (int i = 0; i < App_ProConnectAcceptManager::instance()->GetCount(); i++)
        {
            ProConnectAcceptor* pProConnectAcceptor = App_ProConnectAcceptManager::instance()->GetConnectAcceptor(i);

            if (nullptr != pProConnectAcceptor)
            {
                _ControlInfo objInfo;

                objInfo.m_strListenIP = pProConnectAcceptor->GetListenIP();
                objInfo.m_u4Port = pProConnectAcceptor->GetListenPort();
                m_vecListenList.push_back(objInfo);
            }
        }
    }

    return (uint32)m_vecListenList.size();
}

bool CProControlListen::ShowListen(uint32 u4Index, _ControlInfo& objControlInfo)
{
    if (u4Index >= m_vecListenList.size())
    {
        return false;
    }

    objControlInfo = m_vecListenList[u4Index];
    return true;
}

uint32 CProControlListen::GetServerID()
{
    return GetXmlConfigAttribute(xmlServerID)->id;
}

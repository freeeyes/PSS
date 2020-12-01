#include "ForbiddenIP.h"

bool CForbiddenIP::Init(const char* szConfigPath)
{
    OUR_DEBUG((LM_INFO, "[CForbiddenIP::Init]Filename = %s.\n", szConfigPath));

    if(!m_ForbiddenData.Init(szConfigPath))
    {
        OUR_DEBUG((LM_INFO, "[CForbiddenIP::Init]Read Filename = %s error.\n", szConfigPath));
        return false;
    }

    m_VecForeverForbiddenIP.clear();
    m_VecTempForbiddenIP.clear();

    _ForbiddenIP ForbiddenIP;

    TiXmlElement* pNextTiXmlElementIP   = nullptr;
    TiXmlElement* pNextTiXmlElementType = nullptr;

    while(true)
    {
        const char* pIpData   = m_ForbiddenData.GetData("ForbiddenIP", "ip", pNextTiXmlElementIP);
        const char* pTypeData = m_ForbiddenData.GetData("ForbiddenIP", "type", pNextTiXmlElementType);

        if(nullptr == pIpData || nullptr == pTypeData)
        {
            break;
        }

        ForbiddenIP.m_strClientIP = pIpData;

        if (ACE_OS::strcmp(pTypeData, "TCP") == 0)
        {
            ForbiddenIP.m_u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;
        }
        else
        {
            ForbiddenIP.m_u1ConnectType = EM_CONNECT_IO_TYPE::CONNECT_IO_UDP;
        }

        m_VecForeverForbiddenIP.push_back(ForbiddenIP);
    }

    return true;
}

bool CForbiddenIP::CheckIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType)
{
    for(const _ForbiddenIP& objForbiddenIP : m_VecForeverForbiddenIP)
    {
        if(objForbiddenIP.m_u1ConnectType == u1ConnectType
           && CompareIP(objForbiddenIP.m_strClientIP.c_str(), pIP) == true)
        {
            return false;
        }
    }

    for(VecForbiddenIP::iterator b = m_VecTempForbiddenIP.begin(); b != m_VecTempForbiddenIP.end(); ++b)
    {
        if((*b).m_u1ConnectType == u1ConnectType && CompareIP((*b).m_strClientIP.c_str(), pIP) == true)
        {
            //如果是禁止时间段内，则返回false，否则删除定时信息。
            if ((*b).m_tvBegin + ACE_Time_Value((*b).m_u4Second, 0) > ACE_OS::gettimeofday())
            {
                return false;
            }
            else
            {
                m_VecTempForbiddenIP.erase(b);
                return true;
            }
        }
    }

    return true;
}

bool CForbiddenIP::AddForeverIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType)
{
    _ForbiddenIP ForbiddenIP;
    ForbiddenIP.m_strClientIP   = pIP;
    ForbiddenIP.m_u1ConnectType = u1ConnectType;
    m_VecForeverForbiddenIP.push_back(ForbiddenIP);

    if (false == SaveConfig())
    {
        OUR_DEBUG((LM_INFO, "[CForbiddenIP::AddForeverIP]SaveConfig is error.\n"));
    }

    return true;
}

bool CForbiddenIP::AddTempIP(const char* pIP, uint32 u4Second, EM_CONNECT_IO_TYPE u1ConnectType)
{
    _ForbiddenIP ForbiddenIP;
    ForbiddenIP.m_strClientIP   = pIP;
    ForbiddenIP.m_u1Type        = 1;
    ForbiddenIP.m_tvBegin       = ACE_OS::gettimeofday();
    ForbiddenIP.m_u4Second      = u4Second;
    ForbiddenIP.m_u1ConnectType = u1ConnectType;
    m_VecTempForbiddenIP.push_back(ForbiddenIP);

    return true;
}

bool CForbiddenIP::DelForeverIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType)
{
    for(VecForbiddenIP::iterator b = m_VecForeverForbiddenIP.begin(); b != m_VecForeverForbiddenIP.end(); ++b)
    {
        if(ACE_OS::strcmp(pIP, (*b).m_strClientIP.c_str()) == 0 && (*b).m_u1ConnectType == u1ConnectType)
        {
            m_VecForeverForbiddenIP.erase(b);

            if (false == SaveConfig())
            {
                OUR_DEBUG((LM_INFO, "[CForbiddenIP::DelForeverIP]SaveConfig is error.\n"));
            }

            return true;
        }
    }

    return true;
}

bool CForbiddenIP::DelTempIP(const char* pIP, EM_CONNECT_IO_TYPE u1ConnectType)
{
    for(VecForbiddenIP::iterator b = m_VecTempForbiddenIP.begin(); b !=  m_VecTempForbiddenIP.end(); ++b)
    {
        if(ACE_OS::strcmp(pIP, (*b).m_strClientIP.c_str()) == 0 && (*b).m_u1ConnectType == u1ConnectType)
        {
            m_VecTempForbiddenIP.erase(b);
            return true;
        }
    }

    return true;
}

bool CForbiddenIP::SaveConfig() const
{

    //将修改的配置信息写入文件
    FILE* pFile = ACE_OS::fopen(FORBIDDENIP_FILE, "wb+");

    if(nullptr == pFile)
    {
        OUR_DEBUG((LM_ERROR, "[CForbiddenIP::SaveConfig]Open file fail.\n"));
        return false;
    }

    string strTemp = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<config>\r\n";

    size_t stSize = ACE_OS::fwrite(strTemp.c_str(), sizeof(char), strTemp.length(), pFile);

    if(stSize != strTemp.length())
    {
        OUR_DEBUG((LM_ERROR, "[CForbiddenIP::SaveConfig]Write file fail.\n"));
        ACE_OS::fclose(pFile);
        return false;
    }

    for(const _ForbiddenIP& objForbiddenIP : m_VecForeverForbiddenIP)
    {
        std::stringstream ss_format;
        if(objForbiddenIP.m_u1ConnectType == EM_CONNECT_IO_TYPE::CONNECT_IO_TCP)
        {
            ss_format << "<ForbiddenIP ip=\"" 
                << objForbiddenIP.m_strClientIP 
                << "\" type=\"TCP\" desc=\"ForbiddenIP，type is 'TCP' or 'UDP'\" />\r\n";
            strTemp = ss_format.str();
        }
        else
        {
            ss_format << "<ForbiddenIP ip=\"" 
                << objForbiddenIP.m_strClientIP
                << "\" type=\"UDP\" desc=\"ForbiddenIP，type is 'TCP' or 'UDP'\" />\r\n";
            strTemp = ss_format.str();
        }

        stSize = ACE_OS::fwrite(strTemp.c_str(), sizeof(char), strTemp.length(), pFile);

        if(stSize != strTemp.length())
        {
            OUR_DEBUG((LM_ERROR, "[CForbiddenIP::SaveConfig]Write file fail.\n"));
            ACE_OS::fclose(pFile);
            return false;
        }
    }

    strTemp = "</config>\r\n";

    stSize = ACE_OS::fwrite(strTemp.c_str(), sizeof(char), strTemp.length(), pFile);

    if(stSize != strTemp.length())
    {
        OUR_DEBUG((LM_ERROR, "[CForbiddenIP::SaveConfig]Write file fail.\n"));
        ACE_OS::fclose(pFile);
        return false;
    }

    ACE_OS::fflush(pFile);
    ACE_OS::fclose(pFile);
    return true;
}

bool CForbiddenIP::CompareIP(const char* pTargetIP, const char* pClientIP) const
{
    //这里不必在对查询做*的匹配操作，因为无意义。
    if (ACE_OS::strcmp(pTargetIP, pClientIP) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

VecForbiddenIP* CForbiddenIP::ShowForeverIP()
{
    return &m_VecForeverForbiddenIP;
}

VecForbiddenIP* CForbiddenIP::ShowTempIP()
{
    return &m_VecTempForbiddenIP;
}

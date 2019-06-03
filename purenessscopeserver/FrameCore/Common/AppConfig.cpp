#include "AppConfig.h"

CAppConfig::CAppConfig(void)
{
    m_pConfig = std::make_shared<ACE_Configuration_Heap>();
    m_pIniImp = std::make_shared<ACE_Ini_ImpExp>(*m_pConfig);
    m_szError[0] = '\0';
}

CAppConfig::CAppConfig(const CAppConfig& ar)
{
    //这里需要用move把实参的左值性转换为右值,避免调用复制构造CAppConfig(CAppConfig&& ar)
    (*this) = std::move(ar);
}

CAppConfig::~CAppConfig(void)
{
    OUR_DEBUG((LM_INFO, "[CAppConfig::~CAppConfig].\n"));
}

const char* CAppConfig::GetError()
{
    return m_szError;
}

bool CAppConfig::ReadConfig(const char* szConfigname)
{
    try
    {
        m_strConfigName = szConfigname;

        //m_pConfig = new ACE_Configuration_Heap();

        if (nullptr == m_pConfig)
        {
            char szError[MAX_BUFF_500] = { '\0' };
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::ReadConfig]New ACE_Configuration_Heap error.");
            throw std::domain_error(szError);
        }

        m_pConfig->open();

        //m_pIniImp = new ACE_Ini_ImpExp(*m_pConfig);

        if (nullptr == m_pIniImp)
        {
            char szError[MAX_BUFF_500] = { '\0' };
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::ReadConfig]New ACE_Ini_ImpExp error.");
            throw std::domain_error(szError);
        }

        int32 nRet = (int32)m_pIniImp->import_config((ACE_TCHAR*)m_strConfigName.c_str());

        if (0 != nRet)
        {
            char szError[MAX_BUFF_500] = { '\0' };
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::ReadConfig]import_config error FileName = %s nRet = %d.", m_strConfigName.c_str(), nRet);
            throw std::domain_error(szError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", ex.what());
        return false;
    }
}

bool CAppConfig::WriteConfig(const char* szConfigname)
{
    try
    {
        if (m_strConfigName == "")
        {
            throw std::domain_error("[CAppConfig::WriteConfig]m_strConfigName no exist.");
        }

        if (m_pIniImp == nullptr)
        {
            throw std::domain_error("[CAppConfig::WriteConfig]m_pIniImp is NULL.");
        }

        int32 nRet = (int32)m_pIniImp->export_config((ACE_TCHAR*)szConfigname);

        if (0 != nRet)
        {
            char szError[MAX_BUFF_500] = { '\0' };
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::WriteConfig]export_config error FileName = %s nRet = %d.", szConfigname, nRet);
            throw std::domain_error(szError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", ex.what());
        return false;
    }
}

bool CAppConfig::WriteConfig()
{
    try
    {
        if (m_strConfigName == "")
        {
            throw std::domain_error("[CAppConfig::WriteConfig]m_strConfigName no exist.");
        }

        if (m_pIniImp == NULL)
        {
            throw std::domain_error("[CAppConfig::WriteConfig]m_pIniImp is NULL.");
        }

        int32 nRet = (int32)m_pIniImp->export_config((ACE_TCHAR*)m_strConfigName.c_str());

        if (0 != nRet)
        {
            char szError[MAX_BUFF_500] = { '\0' };
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::WriteConfig]export_config error FileName = %s nRet = %d.", m_strConfigName.c_str(), nRet);
            throw std::domain_error(szError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", ex.what());
        return false;
    }
}

bool CAppConfig::GetValue(const char* szName, ACE_TString& strValue, const char* szRoot)
{
    try
    {
        ACE_Configuration_Section_Key key;

        if (ACE_OS::strlen(szRoot) == 0)
        {
            key = m_pConfig->root_section();
        }
        else
        {
            m_pConfig->expand_path(m_pConfig->root_section(), (ACE_TCHAR*)szRoot, key, 0);
        }

        int32 nRet = (int32)m_pConfig->get_string_value(key, (ACE_TCHAR*)szName, strValue);

        if (0 != nRet)
        {
            char szError[MAX_BUFF_500] = { '\0' };
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::GetValue]export_config error FileName = %s, szName = %s, szRoot = %s, nRet = %d.", m_strConfigName.c_str(), szName, szRoot, nRet);
            throw std::domain_error(szError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", ex.what());
        return false;
    }
}

bool CAppConfig::SetValue(const char* szName, ACE_TString& strValue, const char* szRoot)
{
    try
    {
        ACE_Configuration_Section_Key key;

        if (ACE_OS::strlen(szRoot) == 0)
        {
            key = m_pConfig->root_section();
        }
        else
        {
            m_pConfig->expand_path(m_pConfig->root_section(), (ACE_TCHAR*)szRoot, key, 0);
        }

        int32 nRet = (int32)m_pConfig->set_string_value(key, (ACE_TCHAR*)szName, strValue);

        if (0 != nRet)
        {
            char szError[MAX_BUFF_500] = { '\0' };
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::SetValue]export_config error FileName = %s, szName = %s, szRoot = %s, nRet = %d.", m_strConfigName.c_str(), szName, szRoot, nRet);
            throw std::domain_error(szError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", ex.what());
        return false;
    }
}

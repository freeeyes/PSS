#include "AppConfig.h"

CAppConfig::CAppConfig(void)
{
    m_pConfig    = NULL;
    m_pIniImp    = NULL;
    m_szError[0] = '\0';
}

CAppConfig::~CAppConfig(void)
{
    OUR_DEBUG((LM_INFO, "[CAppConfig::~CAppConfig].\n"));
    Close();
}

void CAppConfig::Close()
{
    if(NULL != m_pConfig)
    {
        SAFE_DELETE(m_pConfig);
    }

    if(NULL != m_pIniImp)
    {
        SAFE_DELETE(m_pIniImp);
    }

    m_strConfigName = "";
}

const char* CAppConfig::GetError()
{
    return m_szError;
}

bool CAppConfig::ReadConfig(const char* szConfigname)
{
    try
    {
        Close();

        m_strConfigName = szConfigname;

        m_pConfig = new ACE_Configuration_Heap();

        if(NULL == m_pConfig)
        {
            throw "[CAppConfig::ReadConfig]New ACE_Configuration_Heap error.";
        }

        m_pConfig->open();

        m_pIniImp = new ACE_Ini_ImpExp(*m_pConfig);

        if(NULL == m_pIniImp)
        {
            throw "[CAppConfig::ReadConfig]New ACE_Ini_ImpExp error.";
        }

        int32 nRet = (int32)m_pIniImp->import_config((ACE_TCHAR*)m_strConfigName.c_str());

        if(0 != nRet)
        {
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::ReadConfig]import_config error FileName = %s nRet = %d.", m_strConfigName.c_str(), nRet);
            throw szError;
        }

        return true;
    }
    catch (const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

bool CAppConfig::WriteConfig(const char* szConfigname)
{
    try
    {
        if(m_strConfigName == "")
        {
            throw "[CAppConfig::WriteConfig]m_strConfigName no exist.";
        }

        if(m_pIniImp == NULL)
        {
            throw "[CAppConfig::WriteConfig]m_pIniImp is NULL.";
        }

        int32 nRet = (int32)m_pIniImp->export_config((ACE_TCHAR*)szConfigname);

        if(0 != nRet)
        {
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::WriteConfig]export_config error FileName = %s nRet = %d.", szConfigname, nRet);
            throw szError;
        }

        return true;
    }
    catch (const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

bool CAppConfig::WriteConfig()
{
    try
    {
        if(m_strConfigName == "")
        {
            throw "[CAppConfig::WriteConfig]m_strConfigName no exist.";
        }

        if(m_pIniImp == NULL)
        {
            throw "[CAppConfig::WriteConfig]m_pIniImp is NULL.";
        }

        int32 nRet = (int32)m_pIniImp->export_config((ACE_TCHAR*)m_strConfigName.c_str());

        if(0 != nRet)
        {
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::WriteConfig]export_config error FileName = %s nRet = %d.", m_strConfigName.c_str(), nRet);
            throw szError;
        }

        return true;
    }
    catch (const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

bool CAppConfig::GetValue(const char* szName, ACE_TString& strValue, const char* szRoot)
{
    try
    {
        ACE_Configuration_Section_Key key;

        if(ACE_OS::strlen(szRoot) == 0)
        {
            key = m_pConfig->root_section();
        }
        else
        {
            m_pConfig->expand_path(m_pConfig->root_section(), (ACE_TCHAR* )szRoot, key, 0);
        }

        int32 nRet = (int32)m_pConfig->get_string_value(key, (ACE_TCHAR* )szName, strValue);

        if(0 != nRet)
        {
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::GetValue]export_config error FileName = %s, szName = %s, szRoot = %s, nRet = %d.", m_strConfigName.c_str(), szName, szRoot, nRet);
            throw szError;
        }

        return true;
    }
    catch (const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

bool CAppConfig::SetValue(const char* szName, ACE_TString& strValue, const char* szRoot)
{
    try
    {
        ACE_Configuration_Section_Key key;

        if(ACE_OS::strlen(szRoot) == 0)
        {
            key = m_pConfig->root_section();
        }
        else
        {
            m_pConfig->expand_path(m_pConfig->root_section(), (ACE_TCHAR* )szRoot, key, 0);
        }

        int32 nRet = (int32)m_pConfig->set_string_value(key, (ACE_TCHAR* )szName, strValue);

        if(0 != nRet)
        {
            char szError[MAX_BUFF_500] = {'\0'};
            sprintf_safe(szError, MAX_BUFF_500, "[CAppConfig::SetValue]export_config error FileName = %s, szName = %s, szRoot = %s, nRet = %d.", m_strConfigName.c_str(), szName, szRoot, nRet);
            throw szError;
        }

        return true;
    }
    catch (const char* szError)
    {
        sprintf_safe(m_szError, MAX_BUFF_500, "%s", szError);
        return false;
    }
}

#include "AppConfig.h"

CAppConfig::CAppConfig(void)
{
    m_pConfig = std::make_shared<ACE_Configuration_Heap>();
    m_pIniImp = std::make_shared<ACE_Ini_ImpExp>(*m_pConfig);
}

const char* CAppConfig::GetError() const
{
    return m_strError.c_str();
}

bool CAppConfig::ReadConfig(const char* szConfigname)
{
    try
    {
        m_strConfigName = szConfigname;

        if (nullptr == m_pConfig)
        {
            std::string strError = "[CAppConfig::ReadConfig]New ACE_Configuration_Heap error.";
            throw std::domain_error(strError);
        }

        m_pConfig->open();

        if (nullptr == m_pIniImp)
        {
            std::string strError = "[CAppConfig::ReadConfig]New ACE_Ini_ImpExp error.";
            throw std::domain_error(strError);
        }

        int nRet = m_pIniImp->import_config(m_strConfigName.c_str());

        if (0 != nRet)
        {
            stringstream ss_format;
            ss_format << "[CAppConfig::ReadConfig]import_config error FileName = "
                << m_strConfigName
                << "nRet = "
                << nRet;
            std::string strError = ss_format.str();
            throw std::domain_error(strError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        m_strError = ex.what();
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

        int nRet = m_pIniImp->export_config(szConfigname);

        if (0 != nRet)
        {
			stringstream ss_format;
			ss_format << "[CAppConfig::WriteConfig]export_config error FileName = "
				<< szConfigname
				<< "nRet = "
				<< nRet;
			std::string strError = ss_format.str();
			throw std::domain_error(strError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        m_strError = ex.what();
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

        int nRet = m_pIniImp->export_config(m_strConfigName.c_str());

        if (0 != nRet)
        {
			stringstream ss_format;
			ss_format << "[CAppConfig::WriteConfig]export_config error FileName = "
				<< m_strConfigName
				<< "nRet = "
				<< nRet;
			std::string strError = ss_format.str();
			throw std::domain_error(strError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        m_strError = ex.what();
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
            m_pConfig->expand_path(m_pConfig->root_section(), szRoot, key, 0);
        }

        int nRet = m_pConfig->get_string_value(key, szName, strValue);

        if (0 != nRet)
        {
			stringstream ss_format;
			ss_format << "[CAppConfig::GetValue]export_config error FileName = "
				<< m_strConfigName
				<< ", szName = "
				<< szName
                << ", szRoot = "
                << szRoot
                << ", nRet ="
                << nRet;
			std::string strError = ss_format.str();
			throw std::domain_error(strError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        m_strError = ex.what();
        return false;
    }
}

bool CAppConfig::SetValue(const char* szName, const ACE_TString& strValue, const char* szRoot)
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
            m_pConfig->expand_path(m_pConfig->root_section(), szRoot, key, 0);
        }

        int nRet = m_pConfig->set_string_value(key, szName, strValue);

        if (0 != nRet)
        {
			stringstream ss_format;
			ss_format << "[CAppConfig::SetValue]export_config error FileName = "
				<< m_strConfigName
				<< ", szName = "
				<< szName
				<< ", szRoot = "
				<< szRoot
				<< ", nRet ="
				<< nRet;
			std::string strError = ss_format.str();
			throw std::domain_error(strError);
        }

        return true;
    }
    catch (const std::domain_error& ex)
    {
        m_strError = ex.what();
        return false;
    }
}

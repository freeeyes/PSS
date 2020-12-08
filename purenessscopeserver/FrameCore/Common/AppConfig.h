// Define.h
// ��ȡACE�����ļ��ķ�����
// �κ�һ���£����Ǵ��㿪ʼ�ģ����ԣ���Ҫ�����Ժ�����ʧ�ܣ����ڵĻ���������δ���������ѵĻ�ʯ��
// add by freeeyes
// 2008-12-22

#ifndef _APPCONFIG_H
#define _APPCONFIG_H

#include "define.h"
#include <stdexcept>

#include "ace/ACE.h"
#include "ace/OS_main.h"
#include "ace/Configuration.h"
#include "ace/Configuration_Import_Export.h"
#include <memory>
#include <string>
#include <sstream>

class CAppConfig
{
public:
    CAppConfig(void);

    bool ReadConfig(const char* szConfigname);
    bool WriteConfig(const char* szConfigname);
    bool WriteConfig();

    bool GetValue(const char* szName, ACE_TString& strValue, const string& strRoot);
    bool SetValue(const char* szName, const ACE_TString& strValue, const string& strRoot);

    const char* GetError() const;

private:
    std::shared_ptr<ACE_Configuration_Heap> m_pConfig;
    std::shared_ptr<ACE_Ini_ImpExp>         m_pIniImp;
    ACE_TString             m_strConfigName;
    string                  m_strError;
};

#endif

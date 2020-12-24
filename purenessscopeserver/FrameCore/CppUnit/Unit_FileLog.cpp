#include "Unit_FileLog.h"

#ifdef _CPPUNIT_TEST

void CUnit_FileLogger::setUp(void)
{
    m_pLogFile = std::make_shared<CLogFile>();
}

void CUnit_FileLogger::tearDown(void)
{
    PSS_LOGGER_DEBUG("[CUnit_FileLogger::tearDown]Finish.");
}

void CUnit_FileLogger::Test_FileLogger(void)
{
    bool blRet = false;

    CLogFile ObjCopyLogFile();

    m_pLogFile->SetFileRoot("./");
    m_pLogFile->SetLoggerName("CppunitTest");
    m_pLogFile->SetLoggerID(2000);
    m_pLogFile->SetLoggerClass(1);
    m_pLogFile->SetLevel(1);
    m_pLogFile->SetServerName("127_");
    m_pLogFile->SetDisplay(0);
    m_pLogFile->Init("./", 8192, 1024);


    if (8192 != m_pLogFile->GetBufferSize())
    {
        PSS_LOGGER_DEBUG("[Test_FileLogger]m_pLogFile->GetBufferSize() fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetBufferSize() fail.", true == blRet);
        return;
    }

    if (ACE_OS::strcmp("./", m_pLogFile->GetFileRoot()) != 0)
    {
        PSS_LOGGER_DEBUG("[Test_FileLogger]m_pLogFile->GetFileRoot() fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetFileRoot() fail.", true == blRet);
        return;
    }
    
    auto logTime = m_pLogFile->GetLogTime();
    PSS_LOGGER_DEBUG("[Test_FileLogger]LogTime={0}.", logTime);

    if (MAX_BUFF_1024 * MAX_BUFF_1024 * 1024 != m_pLogFile->GetFileMaxSize())
    {
        PSS_LOGGER_DEBUG("[Test_FileLogger]m_pLogFile->GetFileMaxSize() fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetFileMaxSize() fail.", true == blRet);
        return;
    }

    if (1 != m_pLogFile->GetCurrFileIndex())
    {
        PSS_LOGGER_DEBUG("[Test_FileLogger]m_pLogFile->GetCurrFileIndex() fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetCurrFileIndex() fail.", true == blRet);
        return;
    }

    auto fileSize = m_pLogFile->GetCurrFileSize();
    PSS_LOGGER_DEBUG("[Test_FileLogger]fileSize={0}.", fileSize);

    if (false == m_pLogFile->Run())
    {
        PSS_LOGGER_DEBUG("[Test_FileLogger]m_pLogFile->Run() fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->Run() fail.", true == blRet);
        return;
    }

    auto objLogBlockInfo = std::make_shared<_LogBlockInfo>();
    string strData = "freeeyes";
;   objLogBlockInfo->m_strBlock   = strData;
    objLogBlockInfo->m_u4Length = (uint32)strData.length();

    if (0 != m_pLogFile->doLog(objLogBlockInfo))
    {
        PSS_LOGGER_DEBUG("[Test_FileLogger]m_pLogFile->doLog() fail.");
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->doLog() fail.", true == blRet);
    }

}

void CUnit_FileLogger::Test_SendMail(void)
{
    auto objLogBlockInfo = std::make_shared<_LogBlockInfo>();
    xmlMails::_Mail objMailInfo;

    objLogBlockInfo->m_strMailTitle = "Pss Test";
    objLogBlockInfo->m_strBlock = "Pss Test";

    objMailInfo.MailID       = 1;
    objMailInfo.fromMailAddr = "freeeyes@163.com";
    objMailInfo.toMailAddr   = "freeeyes@163.com";
    objMailInfo.MailPass     = "free98370112";
    objMailInfo.MailUrl      = "smtps://smtp.163.com";
    objMailInfo.MailPort     = 465;

    //这里测试邮件发送
    m_pLogFile->SendMail(objLogBlockInfo, &objMailInfo);
    m_nTestCount++;
}

#endif

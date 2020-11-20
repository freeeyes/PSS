#include "Unit_FileLog.h"

#ifdef _CPPUNIT_TEST

void CUnit_FileLogger::setUp(void)
{
    m_pLogFile = std::make_shared<CLogFile>("./", 8192, 1024);
}

void CUnit_FileLogger::tearDown(void)
{
    OUR_DEBUG((LM_INFO, "[CUnit_FileLogger::tearDown]Finish.\n"));
}

void CUnit_FileLogger::Test_FileLogger(void)
{
    bool blRet = false;

    CLogFile ObjCopyLogFile("./", 8192, 1024);

    m_pLogFile->SetFileRoot("./");
    m_pLogFile->SetLoggerName("CppunitTest");
    m_pLogFile->SetLoggerID(2000);
    m_pLogFile->SetLoggerClass(1);
    m_pLogFile->SetLevel(1);
    m_pLogFile->SetServerName("127_");
    m_pLogFile->SetDisplay(0);
    m_pLogFile->Init();

    ObjCopyLogFile = (*m_pLogFile);

    if (ObjCopyLogFile.GetBufferSize() != m_pLogFile->GetBufferSize())
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->GetBufferSize() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetBufferSize() fail.", true == blRet);
        return;
    }

    if (ACE_OS::strcmp(ObjCopyLogFile.GetFileRoot(), m_pLogFile->GetFileRoot()) != 0)
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->GetFileRoot() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetFileRoot() fail.", true == blRet);
        return;
    }

    if (ACE_OS::strcmp(ObjCopyLogFile.GetLogTime(), m_pLogFile->GetLogTime()) != 0)
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->GetLogTime() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetLogTime() fail.", true == blRet);
        return;
    }

    if (ObjCopyLogFile.GetFileMaxSize() != m_pLogFile->GetFileMaxSize())
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->GetFileMaxSize() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetFileMaxSize() fail.", true == blRet);
        return;
    }

    if (ObjCopyLogFile.GetCurrFileIndex() != m_pLogFile->GetCurrFileIndex())
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->GetCurrFileIndex() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetCurrFileIndex() fail.", true == blRet);
        return;
    }

    if (ObjCopyLogFile.GetCurrFileIndex() != m_pLogFile->GetCurrFileIndex())
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->GetCurrFileIndex() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetCurrFileIndex() fail.", true == blRet);
        return;
    }

    if (ObjCopyLogFile.GetCurrFileSize() != m_pLogFile->GetCurrFileSize())
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->GetCurrFileSize() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->GetCurrFileSize() fail.", true == blRet);
        return;
    }

    if (false == m_pLogFile->Run())
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->Run() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->Run() fail.", true == blRet);
        return;
    }

    _LogBlockInfo objLogBlockInfo;
    char szData[20] = { '\0' };
    sprintf_safe(szData, 20, "freeeyes OK");
    objLogBlockInfo.m_pBlock   = reinterpret_cast<char*>(szData);
    objLogBlockInfo.m_u4Length = (uint32)ACE_OS::strlen(szData);

    if (0 != m_pLogFile->doLog(&objLogBlockInfo))
    {
        OUR_DEBUG((LM_INFO, "[Test_FileLogger]m_pLogFile->doLog() fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_FileLogger]m_pLogFile->doLog() fail.", true == blRet);
    }

}

void CUnit_FileLogger::Test_SendMail(void)
{
    _LogBlockInfo   objLogBlockInfo;
    xmlMails::_Mail objMailInfo;

    char szMailBody[MAX_BUFF_200] = { '\0' };

    sprintf_safe(objLogBlockInfo.m_szMailTitle, MAX_BUFF_200, "Pss Test");
    sprintf_safe(szMailBody, MAX_BUFF_200, "Pss Test");

    objLogBlockInfo.m_pBlock = (char* )szMailBody;

    objMailInfo.MailID       = 1;
    objMailInfo.fromMailAddr = "freeeyes@163.com";
    objMailInfo.toMailAddr   = "freeeyes@163.com";
    objMailInfo.MailPass     = "free98370112";
    objMailInfo.MailUrl      = "smtps://smtp.163.com";
    objMailInfo.MailPort     = 465;

    //这里测试邮件发送
    m_pLogFile->SendMail(&objLogBlockInfo, &objMailInfo);
    m_nTestCount++;
}

#endif

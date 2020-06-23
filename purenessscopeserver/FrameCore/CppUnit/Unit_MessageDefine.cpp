#include "Unit_MessageDefine.h"

#ifdef _CPPUNIT_TEST

CUnit_MessageDefine::CUnit_MessageDefine()
{
}

void CUnit_MessageDefine::setUp(void)
{
    m_nMessageDefine = 1;
}

void CUnit_MessageDefine::tearDown(void)
{
    m_nMessageDefine = 0;
}


void CUnit_MessageDefine::Test_GetDyeingCommand(void)
{
    bool blRet = false;
    _DyeCommandInfo objDyeCommandInfo;

    blRet = GetDyeingCommand("-i 0x1000 -c 5 ", objDyeCommandInfo);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetDyeingCommand]GetDyeingCommand is fail.", true == blRet);

    blRet = false;

    if (objDyeCommandInfo.m_u2CommandID == 0x1000 && objDyeCommandInfo.m_u2MaxCount == 5)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetDyeingCommand]GetDyeingCommand Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetDyeingIP(void)
{
    bool blRet = false;
    _DyeIPInfo objDyeIPInfo;

    blRet = GetDyeingIP("-i 127.0.0.1 -c 5 ", objDyeIPInfo);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetDyeingIP]GetDyeingIP is fail.", true == blRet);

    blRet = false;

    if (ACE_OS::strcmp(objDyeIPInfo.m_szClientIP, "127.0.0.1") == 0 && objDyeIPInfo.m_u2MaxCount == 5)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetDyeingIP]GetDyeingIP Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetFileInfo(void)
{
    bool blRet = false;
    _FileInfo FileInfo;

    blRet = GetFileInfo("./,TcpTest.so,Param", FileInfo);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetFileInfo]GetFileInfo is fail.", true == blRet);

    blRet = false;

    if (ACE_OS::strcmp(FileInfo.m_szFileName, "TcpTest.so") == 0
        && ACE_OS::strcmp(FileInfo.m_szFilePath, "./") == 0
        && ACE_OS::strcmp(FileInfo.m_szFileParam, "Param") == 0)
    {
        blRet = true;
    }

    OUR_DEBUG((LM_INFO, "[Test_GetFileInfo]m_szFileName=%s,m_szFilePath=%s,m_szFileParam=%s.",
               FileInfo.m_szFileName,
               FileInfo.m_szFilePath,
               FileInfo.m_szFileParam));
    CPPUNIT_ASSERT_MESSAGE("[Test_GetFileInfo]GetFileInfo Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetTrackIP(void)
{
    bool blRet = false;
    _ForbiddenIP objForbiddenIP;
    blRet = GetTrackIP("-c 127.0.0.1 ", objForbiddenIP);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetTrackIP]GetTrackIP is fail.", true == blRet);

    if (ACE_OS::strcmp(objForbiddenIP.m_szClientIP, "127.0.0.1") == 0)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetTrackIP]GetTrackIP Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetLogLevel(void)
{
    bool blRet = false;
    int  nLevel = 0;

    blRet = GetLogLevel("-l 2 ", nLevel);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetLogLevel]GetLogLevel is fail.", true == blRet);

    if (nLevel == 2)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetLogLevel]GetLogLevel Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetAIInfo(void)
{
    bool blRet   = false;
    int nAI      = 0;
    int nDispose = 0;
    int nCheck   = 0;
    int nStop    = 0;
    blRet = GetAIInfo("-i 1,1000,2000,10 ", nAI, nDispose, nCheck, nStop);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetAIInfo]GetAIInfo is fail.", true == blRet);

    if (nAI == 1 &&
        nDispose == 1000 &&
        nCheck == 2000 &&
        nStop == 10)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetAIInfo]GetAIInfo Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetConnectID(void)
{
    bool blRet = false;
    uint32 u4ConnectID = 0;
    bool blFlag = false;

    blRet = GetConnectID("-n 1 -f 0 ", u4ConnectID, blFlag);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetConnectID]GetConnectID is fail.", true == blRet);

    if (u4ConnectID == 1 &&
        blFlag == false)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetConnectID]GetConnectID Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetMaxConnectCount(void)
{
    bool blRet = false;
    uint16 u2MaxConnectCount = 0;
    blRet = GetMaxConnectCount("-n 1000 ", u2MaxConnectCount);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetMaxConnectCount]GetMaxConnectCount is fail.", true == blRet);

    if (u2MaxConnectCount == 1000)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetMaxConnectCount]GetMaxConnectCount Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetListenInfo(void)
{
    bool blRet = false;
    _ListenInfo objListenInfo;

    blRet = GetListenInfo("-i 127.0.0.1 -p 10003 -t 1 -n 1 ", objListenInfo);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetListenInfo]GetListenInfo is fail.", true == blRet);

    if (ACE_OS::strcmp(objListenInfo.m_szListenIP, "127.0.0.1") == 0 &&
        objListenInfo.m_u1IPType == 1 &&
        objListenInfo.m_u2Port == 10003 &&
        objListenInfo.m_u4PacketParseID == 1)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetListenInfo]GetListenInfo Data is fail.", true == blRet);
    m_nTestCount++;
}

void CUnit_MessageDefine::Test_GetPoolSet(void)
{
    bool blRet = false;
    _PoolName objPoolName;

    blRet = GetPoolSet("-n freeeyes -b true ", objPoolName);

    CPPUNIT_ASSERT_MESSAGE("[Test_GetPoolSet]GetPoolSet is fail.", true == blRet);

    if (ACE_OS::strcmp(objPoolName.m_szPoolName, "freeeyes") == 0 &&
        objPoolName.m_blState == true)
    {
        blRet = true;
    }

    CPPUNIT_ASSERT_MESSAGE("[Test_GetPoolSet]GetPoolSet Data is fail.", true == blRet);
    m_nTestCount++;
}

#endif



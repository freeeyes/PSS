#include "Uint_ConnectHandler.h"

#ifdef _CPPUNIT_TEST

CUnit_ConnectHandler::~CUnit_ConnectHandler()
{
    m_pConnectHandler = NULL;
}

void CUnit_ConnectHandler::setUp(void)
{
    m_pConnectHandler = new CConnectHandler();
    m_pConnectHandler->Init(1);
}

void CUnit_ConnectHandler::tearDown(void)
{
    delete m_pConnectHandler;
    m_pConnectHandler = NULL;
}

void CUnit_ConnectHandler::Test_ConnectHandler(void)
{
    //测试创建指定的反应器
    int blRet = false;

    m_pConnectHandler->SetConnectID(111);
    m_pConnectHandler->SetPacketParseInfoID(1);

    //拼装测试发送数据
    char szSendBuffer[MAX_BUFF_200] = { '\0' };
    char szBuff[20] = { '\0' };
    char szSession[32] = { '\0' };
    sprintf_safe(szBuff, 20, "freeeyes");
    sprintf_safe(szSession, 32, "FREEEYES");

    short sVersion = 1;
    short sCommand = (short)0x1000;
    int nPacketLen = ACE_OS::strlen(szBuff);

    memcpy(szSendBuffer, (char*)&sVersion, sizeof(short));
    memcpy((char*)&szSendBuffer[2], (char*)&sCommand, sizeof(short));
    memcpy((char*)&szSendBuffer[4], (char*)&nPacketLen, sizeof(int));
    memcpy((char*)&szSendBuffer[8], (char*)&szSession, sizeof(char) * 32);
    memcpy((char*)&szSendBuffer[40], (char*)szBuff, sizeof(char) * nPacketLen);
    uint32 u4SendLen = nPacketLen + 40;

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(u4SendLen);
    memcpy_safe(szSendBuffer, u4SendLen, pmb->wr_ptr(), u4SendLen);
    pmb->wr_ptr(u4SendLen);

    blRet = m_pConnectHandler->Test_Paceket_Parse_Stream(pmb);

    if (false == blRet)
    {
        OUR_DEBUG((LM_INFO, "[Test_ConnectHandler]Test_Paceket_Parse_Stream is fail.\n"));
        CPPUNIT_ASSERT_MESSAGE("[Test_ConnectHandler]Test_Paceket_Parse_Stream is fail.", true == blRet);
    }
}

#endif


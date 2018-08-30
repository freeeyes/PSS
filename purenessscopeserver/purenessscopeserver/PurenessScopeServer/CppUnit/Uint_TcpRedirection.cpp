#include "Uint_TcpRedirection.h"

#ifdef _CPPUNIT_TEST

CUnit_Redirection::~CUnit_Redirection()
{
    m_pTcpRedirection = NULL;
}

void CUnit_Redirection::setUp(void)
{
    m_pTcpRedirection = new CTcpRedirection();
    xmlTcpRedirection::_RedirectionInfo redirecttioninfo;

    redirecttioninfo.Mode            = 0;
    redirecttioninfo.ConnectState    = 0;
    redirecttioninfo.RedirectionIP   = "127.0.0.1";
    redirecttioninfo.RedirectionPort = 10005;
    redirecttioninfo.SrcPort         = 10004;

    std::vector<xmlTcpRedirection::_RedirectionInfo> vec;

    IConnectManager* pConnectManager = dynamic_cast<IConnectManager*>(App_ConnectManager::instance());
    IClientManager*  pClientManager = dynamic_cast<IClientManager*>(App_ClientReConnectManager::instance());

    vec.push_back(redirecttioninfo);

    m_pTcpRedirection->Init(vec, 10, pClientManager, pConnectManager);
}

void CUnit_Redirection::tearDown(void)
{
    delete m_pTcpRedirection;
    m_pTcpRedirection = NULL;
}

void CUnit_Redirection::Test_Redirection(void)
{
    //测试转发模块
    uint32 u4ConnectID = 111;

    m_pTcpRedirection->ConnectRedirect(10004, u4ConnectID);

    //测试组装数据
    //拼装测试发送数据
    char szSendBufferData[MAX_BUFF_200] = { '\0' };
    char szBuff[20] = { '\0' };
    char szSession[32] = { '\0' };
    sprintf_safe(szBuff, 20, "redirecttion");
    sprintf_safe(szSession, 32, "redirecttion");

    short sVersion = 1;
    short sCommand = (short)0x1000;
    int nPacketLen = ACE_OS::strlen(szBuff);

    memcpy(szSendBufferData, (char*)&sVersion, sizeof(short));
    memcpy((char*)&szSendBufferData[2], (char*)&sCommand, sizeof(short));
    memcpy((char*)&szSendBufferData[4], (char*)&nPacketLen, sizeof(int));
    memcpy((char*)&szSendBufferData[8], (char*)&szSession, sizeof(char) * 32);
    memcpy((char*)&szSendBufferData[40], (char*)szBuff, sizeof(char) * nPacketLen);

    ACE_Message_Block* pmb = App_MessageBlockManager::instance()->Create(40);
    memcpy_safe((char*)&szSendBuffer[0], 40, pmb->wr_ptr(), 40);
    pmb->wr_ptr(40);
    m_pTcpRedirection->DataRedirect(u4ConnectID, pmb);
    App_MessageBlockManager::instance()->Close(pmb);

    pmb = App_MessageBlockManager::instance()->Create(8);
    memcpy_safe((char*)&szSendBuffer[40], 8, pmb->wr_ptr(), 8);
    pmb->wr_ptr(8);
    m_pTcpRedirection->DataRedirect(u4ConnectID, pmb);
    App_MessageBlockManager::instance()->Close(pmb);

    m_pTcpRedirection->CloseRedirect(u4ConnectID);

}

#endif


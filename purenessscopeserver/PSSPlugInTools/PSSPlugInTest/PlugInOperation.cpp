#include "StdAfx.h"
#include "PlugInOperation.h"

CPlugInOperation::CPlugInOperation(void)
{
  m_pProClientManager     = new CProClientManager();
  m_pLogManager           = new CLogManager();
  m_pProConnectManager    = new CProConnectManager();
  m_pProUDPConnectManager = new CProUDPConnectManager();
  m_pActiveTimer          = new ActiveTimer();
  m_pModuleMessageManager = new CModuleMessageManager();
  m_pvecCommaindInfo      = NULL;
  m_blIsRun               = false;
}

CPlugInOperation::~CPlugInOperation(void)
{
  Close();
}

bool CPlugInOperation::Init()
{
  App_BuffPacketManager::instance()->Init(200);
  //初始化给DLL的对象接口
  App_ServerObject::instance()->SetMessageManager((IMessageManager* )App_MessageManager::instance());
  App_ServerObject::instance()->SetLogManager((ILogManager*  )m_pLogManager);
  App_ServerObject::instance()->SetConnectManager((IConnectManager* )m_pProConnectManager);
  App_ServerObject::instance()->SetPacketManager((IPacketManager* )App_BuffPacketManager::instance());
  App_ServerObject::instance()->SetClientManager((IClientManager* )m_pProClientManager);
  App_ServerObject::instance()->SetUDPConnectManager((IUDPConnectManager* )m_pProUDPConnectManager);
  App_ServerObject::instance()->SetTimerManager((ActiveTimer* )m_pActiveTimer);
  App_ServerObject::instance()->SetModuleMessageManager((IModuleMessageManager* )m_pModuleMessageManager);

  return true;
}

void CPlugInOperation::Close()
{
  if(m_blIsRun == true)
  {
    m_blIsRun = false;
  }

  Sleep(100);

  App_ModuleLoader::instance()->Close();
  App_BuffPacketManager::instance()->Close();

  if(NULL != m_pProClientManager)
  {
    delete m_pProClientManager;
    m_pProClientManager = NULL;
  }

  if(NULL != m_pLogManager)
  {
    delete m_pLogManager;
    m_pLogManager = NULL;
  }

  if(NULL != m_pProConnectManager)
  {
    delete m_pProConnectManager;
    m_pProConnectManager = NULL;
  }

  if(NULL != m_pProUDPConnectManager)
  {
    delete m_pProUDPConnectManager;
    m_pProUDPConnectManager = NULL;
  }

  if(NULL != m_pActiveTimer)
  {
    delete m_pActiveTimer;
    m_pActiveTimer = NULL;
  }

  if(NULL != m_pModuleMessageManager)
  {
    delete m_pModuleMessageManager;
    m_pModuleMessageManager = NULL;
  }
}

bool CPlugInOperation::Start(vecCommaindInfo* pvecCommaindInfo, const char* pModuleString)
{
  //初始化模块
  App_ModuleLoader::instance()->Close();

  //保存发送信息
  m_pvecCommaindInfo = pvecCommaindInfo;

  //初始化模块加载
  return App_ModuleLoader::instance()->LoadModule("./", pModuleString);
}

bool CPlugInOperation::Run()
{
  if(m_pvecCommaindInfo == NULL|| m_blIsRun == true)
  {
    m_blIsRun = false;
    return false;
  }

  CPacketParse* pPacketParse = new CPacketParse();
  CMessage* pMessage         = new CMessage();

  m_blIsRun = true;
  while(m_blIsRun)
  {
    //这里循环发送数据包
    for(int i = 0; i < (int)m_pvecCommaindInfo->size(); i++)
    {
      _CommaindInfo* pCommaindInfo = (*m_pvecCommaindInfo)[i];
      if(NULL != pCommaindInfo)
      {
        pCommaindInfo->m_nSendDataCount++;
		pCommaindInfo->m_nSendMessageSize += pCommaindInfo->m_nSendDataLen;
        
        //开始整合数据（丢到逻辑接口处理里面去）
        ACE_Message_Block* pHeadBlock = App_MessageBlockManager::instance()->Create(pPacketParse->GetPacketHeadLen());
        ACE_OS::memcpy(pHeadBlock->wr_ptr(), pCommaindInfo->m_pSendData, pPacketParse->GetPacketHeadLen());
        pHeadBlock->wr_ptr(pPacketParse->GetPacketHeadLen());
		
        pPacketParse->SetPacketHead(0, pHeadBlock, App_MessageBlockManager::instance());
        uint32 u4BodySize = pCommaindInfo->m_nSendDataLen - pPacketParse->GetPacketHeadLen();
        ACE_Message_Block* pBodyBlock = App_MessageBlockManager::instance()->Create(u4BodySize);
        ACE_OS::memcpy(pBodyBlock->wr_ptr(), &pCommaindInfo->m_pSendData[pPacketParse->GetPacketHeadLen()], u4BodySize);
        pBodyBlock->wr_ptr(u4BodySize);
        pPacketParse->SetPacketBody(0, pBodyBlock, App_MessageBlockManager::instance());

        
        //开始组装数据
        pMessage->GetMessageBase()->m_u4ConnectID   = 1;   //测试数据，固定ConnectID
        pMessage->GetMessageBase()->m_u2Cmd         = pPacketParse->GetPacketCommandID();
        pMessage->GetMessageBase()->m_u4MsgTime     = (uint32)ACE_OS::gettimeofday().sec();
        pMessage->GetMessageBase()->m_u4HeadSrcSize = pPacketParse->GetPacketHeadSrcLen();
        pMessage->GetMessageBase()->m_u4BodySrcSize = pPacketParse->GetPacketBodySrcLen();
        //pMessage->GetMessageBase()->m_ProfileTime.Start();

        //将接受的数据缓冲放入CMessage对象
        pMessage->SetPacketHead(pPacketParse->GetMessageHead());
        pMessage->SetPacketBody(pPacketParse->GetMessageBody());

        uint16 u2CommandID = pPacketParse->GetPacketCommandID();
        App_MessageManager::instance()->DoMessage(pMessage, u2CommandID);
        pMessage->Clear();
        
        pCommaindInfo->m_nRecvDataCount++;
      }
      //Sleep(100);
    }
  }

  delete pMessage;
  delete pPacketParse;
  return true;
}

bool CPlugInOperation::Stop()
{
  m_blIsRun = false;
  return true;
}

vecCommaindInfo* CPlugInOperation::GetvecCommaindInfo()
{
  return m_pvecCommaindInfo;
}

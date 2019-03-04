#include "BaseCommand.h"

//接收消息队列线程
ACE_THR_FUNC Worker(void* arg)
{
	CBaseCommand* pBaseCommand = (CBaseCommand* )arg;

	if(pBaseCommand == NULL)
	{
		return NULL;
	}

	_Muti_QueueData objMutiQueueData; 
	while(true)
	{
		int nLen = GetDataFromMsgQ(pBaseCommand->GetMutiQueueID(), (unsigned char* )&objMutiQueueData, (int)sizeof(_Muti_QueueData));
		if(nLen == (int)sizeof(_Muti_QueueData))
		{
			//接收到了完整的数据体，进行处理
			pBaseCommand->SendData(objMutiQueueData);
		}
		else
		{
			OUR_DEBUG((LM_ERROR, "[Worker] Get MsgQ ERROR.\n"));
			break;
		}
	}

	return NULL;
}

CBaseCommand::CBaseCommand(void)
{
  m_pServerObject   = NULL;
  m_nPssQueueID     = 0;
  m_nMutiQueueID    = 0;
}

CBaseCommand::~CBaseCommand(void)
{
}

void CBaseCommand::SetServerObject(CServerObject* pServerObject)
{
  m_pServerObject = pServerObject;
}

int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
  __ENTER_FUNCTION();

  if(m_pServerObject == NULL)
  {
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL(%d).\n", bDeleteFlag));
    return -1;
  }

  if(pMessage == NULL)
  {
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
    return -1;
  }
  
  OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage]m_u2Cmd=%d.\n", pMessage->GetMessageBase()->m_u2Cmd));

  //入队列
  if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_LINUXQUEUE)
  {
	  Do_Linux_Queue(pMessage);
  }

  return 0;

  __LEAVE_FUNCTION_WITHRETURN(0);
}

void CBaseCommand::Do_Linux_Queue(IMessage* pMessage)
{
	uint16     u2CommandID  = 0;

	IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
	if(NULL == pBodyPacket)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
		return;
	}

	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);

	pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);

	//得到接收的命令
	(*pBodyPacket) >> u2CommandID;

	m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

	//将数据输送给队列
	_Pss_QueueData objPssQueueData;
	objPssQueueData.m_u4ConnectID = pMessage->GetMessageBase()->m_u4ConnectID;
	objPssQueueData.m_u2CommandID = u2CommandID;
	
	//OUR_DEBUG((LM_ERROR, "[CBaseCommand::InitQueue]m_nPssQueueID=%d,Begin.\n", m_nPssQueueID));
	PutDataToMsgQ(m_nPssQueueID, (unsigned char* )&objPssQueueData, (int)sizeof(_Pss_QueueData));
	//OUR_DEBUG((LM_ERROR, "[CBaseCommand::InitQueue]m_nPssQueueID=%d,End.\n", m_nPssQueueID));
}

bool CBaseCommand::InitQueue()
{
	//初始化发送消息队列
	m_nPssQueueID = GetLinuxMsgQID((key_t)PSS_QUEUE_ID);
	if(-1 == m_nPssQueueID)
	{
		return false;
	}

	//初始化接收消息队列
	m_nMutiQueueID = GetLinuxMsgQID((key_t)MI_QUEUE_ID);
	if(-1 == m_nMutiQueueID)
	{
		return false;
	}

	OUR_DEBUG((LM_ERROR, "[CBaseCommand::InitQueue]m_nPssQueueID=%d,m_nMutiQueueID=%d.\n", m_nPssQueueID, m_nMutiQueueID));

	ACE_Thread_Manager::instance()->spawn((ACE_THR_FUNC)Worker, (void* )this);

	return true;

}

int CBaseCommand::GetMutiQueueID()
{
	return m_nMutiQueueID;
}

void CBaseCommand::SendData(_Muti_QueueData& objMutiQueueData)
{
	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = objMutiQueueData.m_u2CommandID;

	//返回数据
	(*pResponsesPacket) << objMutiQueueData.m_u2CommandID;
	(*pResponsesPacket) << (uint32)0;
	(*pResponsesPacket) << (uint32)0;

	if(NULL != m_pServerObject->GetConnectManager())
	{
		//发送全部数据
		m_pServerObject->GetConnectManager()->PostMessage(objMutiQueueData.m_u4ConnectID, pResponsesPacket, SENDMESSAGE_NOMAL, u2PostCommandID, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::SendData] m_pConnectManager = NULL"));
		m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	}
}


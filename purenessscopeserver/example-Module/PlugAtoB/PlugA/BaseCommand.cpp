#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
  m_pServerObject = NULL;
  m_nCount        = 0;
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
  //__ENTER_FUNCTION是自动给你的函数追加try的宏，用于捕捉错误，你可以使用__THROW_FUNCTION()抛出你的错误
  //比如__THROW_FUNCTION("hello"); 它会自动在Log文件夹下的assert.log记录文件名，函数名，行数，描述。
  //推荐你使用这个宏，记住一定要在函数的结束使用__LEAVE_FUNCTION或者__LEAVE_FUNCTION_WITHRETURN与之配对。
  //__LEAVE_FUNCTION_WITHRETURN和__LEAVE_FUNCTION不同是前者可以支持函数退出的时候返回一个变量
  //比如这个函数是返回一个int
  //那么末尾就是__LEAVE_FUNCTION_WITHRETURN(0); 0就是返回的int，当然，也支持别的类型变量，具体看你自己的函数。
  //如果函数不返回任何变量，你可以使用__LEAVE_FUNCTION即可。
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

  //处理链接建立信息
  if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CONNECT)
  {
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CONNECT OK.\n"));
    return 0;
  }

  //处理链接断开信息
  if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
  {
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
    return 0;
  }

  //处理正常信息
  if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_PLUGA)
  {
    Do_PlugA_DATA(pMessage);
  }

  return 0;

  __LEAVE_FUNCTION_WITHRETURN(0);
}

void CBaseCommand::Do_PlugA_DATA(IMessage* pMessage)
{
	VCHARM_STR strUserText;

	IBuffPacket* pBodyPacket = m_pServerObject->GetPacketManager()->Create();
	if(NULL == pBodyPacket)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pBodyPacket is NULL.\n"));
		return;
	}

	_PacketInfo BodyPacket;
	pMessage->GetPacketBody(BodyPacket);

	pBodyPacket->WriteStream(BodyPacket.m_pData, BodyPacket.m_nDataLen);
	(*pBodyPacket) >> strUserText;

	//获得数据，并交给PlugB插件去处理
	IBuffPacket* pModuleSendBuff = m_pServerObject->GetPacketManager()->Create();
	IBuffPacket* pModuleRecvBuff = m_pServerObject->GetPacketManager()->Create();

	(*pModuleSendBuff) << pMessage->GetMessageBase()->m_u4WorkThreadID;
	(*pModuleSendBuff) << pMessage->GetMessageBase()->m_u4ConnectID;
	(*pModuleSendBuff) << strUserText;

	//向PlugB通知数据处理
	m_pServerObject->GetModuleMessageManager()->SendModuleMessage("PlugB模块", MODULE_COMMAND_PLUGB, pModuleSendBuff, pModuleRecvBuff);

	uint32 u4Ret = 0;
	(*pModuleRecvBuff) >> u4Ret;

	m_pServerObject->GetPacketManager()->Delete(pModuleSendBuff);
	m_pServerObject->GetPacketManager()->Delete(pModuleRecvBuff);

	//回收收到的数据包
	m_pServerObject->GetPacketManager()->Delete(pBodyPacket);

	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	//uint16 u2PostCommandID = COMMAND_RETURN_PLUGA;

	//返回结果
	//(*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
	(*pResponsesPacket) << (uint32)u4Ret;

	if(NULL != m_pServerObject->GetConnectManager())
	{
		//发送全部数据
		m_pServerObject->GetConnectManager()->PostMessage(pMessage->GetMessageBase()->m_u4ConnectID, pResponsesPacket, SENDMESSAGE_JAMPNOMAL, (uint16)COMMAND_RETURN_PLUGA, PACKET_SEND_IMMEDIATLY, PACKET_IS_FRAMEWORK_RECYC);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
		m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	}

}

void CBaseCommand::PlugInit()
{
	if(m_pServerObject != NULL)
	{
		IBuffPacket* pModuleSendBuff = m_pServerObject->GetPacketManager()->Create();
		IBuffPacket* pModuleRecvBuff = m_pServerObject->GetPacketManager()->Create();

		uint32 u4WorkThreadCount = m_pServerObject->GetMessageManager()->GetWorkThreadCount();
		(*pModuleSendBuff) << u4WorkThreadCount;
		for(uint32 u4Index = 0; u4Index < u4WorkThreadCount; u4Index++)
		{
			(*pModuleSendBuff) << m_pServerObject->GetMessageManager()->GetWorkThreadByIndex(u4Index);
		}

		//向PlugB通知数据处理
		m_pServerObject->GetModuleMessageManager()->SendModuleMessage("PlugB模块", MODULE_COMMAND_INIT, pModuleSendBuff, pModuleRecvBuff);

		m_pServerObject->GetPacketManager()->Delete(pModuleSendBuff);
		m_pServerObject->GetPacketManager()->Delete(pModuleRecvBuff);
	}
}

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

  if(NULL == pMessage)
  {
	  OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL(%d).\n", bDeleteFlag));
  }

  return 0;

  __LEAVE_FUNCTION_WITHRETURN(0);
}

void CBaseCommand::DoModuleMessage(IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> WGrard(m_ThreadWriteLock);

	if(NULL == pBuffPacket || NULL == pReturnBuffPacket)
	{
		return;
	}

	//写入日志文件
	uint32 u4WorkThreadID = 0;
	uint32 u4ConnectID    = 0;
	VCHARM_STR strUserText;
	char szText[MAX_BUFF_200] = {'\0'};
	char szLog[MAX_BUFF_300]  = {'\0'};

	(*pBuffPacket) >> u4WorkThreadID;
	(*pBuffPacket) >> u4ConnectID;
	(*pBuffPacket) >> strUserText;

	if(strUserText.u2Len >= 200)
	{
		(*pReturnBuffPacket) << (uint32)MODULE_RETURN_ERROR;
		return;
	}

	ACE_OS::memcpy(szText, strUserText.text, strUserText.u2Len);

	ACE_Date_Time dt;
	char szDateBuff[MAX_BUFF_200] = {'\0'};

	sprintf_safe(szDateBuff, MAX_BUFF_50, "%04d-%02d-%02d %02d:%02d:%02d,", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());

	sprintf_safe(szLog, MAX_BUFF_300, "[%s]工作线程ID:%d,内容:%s.\n", szDateBuff, u4WorkThreadID, szText);

	FILE* pFile = fopen("aaa.txt", "ab+");
	if(NULL == pFile)
	{
		(*pReturnBuffPacket) << (uint32)MODULE_RETURN_ERROR;
		return;
	}

	fwrite(szLog, ACE_OS::strlen(szLog), sizeof(char), pFile);

	ACE_OS::fclose(pFile);

	(*pReturnBuffPacket) << (uint32)MODULE_RETURN_SUCCESS;

	//这里附加测试模块B输入发送数据，等待返回和A一起发送给客户端的测试代码。
	//测试框架的发送缓冲机制
	//B把要发送的数据放在缓冲中，然后等待A再加入数据后一起发出去。

	IBuffPacket* pResponsesPacket = m_pServerObject->GetPacketManager()->Create();
	uint16 u2PostCommandID = COMMAND_RETURN_PLUGA;

	//返回结果
	(*pResponsesPacket) << (uint32)(2 + 4 + 4);       //手动拼接返回包 
	(*pResponsesPacket) << (uint16)u2PostCommandID;   //拼接应答命令ID
	(*pResponsesPacket) << (uint32)1;

	if(NULL != m_pServerObject->GetConnectManager())
	{
		//发送数据到框架缓冲中
		m_pServerObject->GetConnectManager()->PostMessage(u4ConnectID, pResponsesPacket, SENDMESSAGE_JAMPNOMAL, u2PostCommandID, PACKET_SEND_CACHE, PACKET_IS_FRAMEWORK_RECYC);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[CBaseCommand::DoMessage] m_pConnectManager = NULL"));
		m_pServerObject->GetPacketManager()->Delete(pResponsesPacket);
	}

}

void CBaseCommand::DoModuleInit( IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket )
{
	if(NULL == pBuffPacket || NULL == pReturnBuffPacket)
	{
		return;
	}

	//写入日志文件
	uint32 u4ThreadCount = 0;
	uint32 u4ThreadID    = 0;
	char szLog[MAX_BUFF_300]  = {'\0'};

	FILE* pFile = fopen("aaa.txt", "ab+");
	if(NULL == pFile)
	{
		(*pReturnBuffPacket) << (uint32)MODULE_RETURN_ERROR;
		return;
	}

	ACE_Date_Time dt;
	char szDateBuff[MAX_BUFF_50] = {'\0'};

	sprintf_safe(szDateBuff, MAX_BUFF_50, "%04d-%02d-%02d %02d:%02d:%02d,", dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second());

	(*pBuffPacket) >> u4ThreadCount;
	sprintf_safe(szLog, MAX_BUFF_300, "[%s]初始化工作线程个数:%d.\n", szDateBuff, u4ThreadCount);
	fwrite(szLog, ACE_OS::strlen(szLog), sizeof(char), pFile);

	for(uint32 nIndex = 0; nIndex < u4ThreadCount; nIndex++)
	{
		(*pBuffPacket) >> u4ThreadID;
		sprintf_safe(szLog, MAX_BUFF_300, "[%s]初始化工作线程ID:%d.\n", szDateBuff, u4ThreadID);
		fwrite(szLog, ACE_OS::strlen(szLog), sizeof(char), pFile);
	}

	ACE_OS::fclose(pFile);

	(*pReturnBuffPacket) << (uint32)MODULE_RETURN_SUCCESS;
}

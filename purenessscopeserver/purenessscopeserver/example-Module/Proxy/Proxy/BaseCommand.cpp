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
	Do_Proxy_Connect(pMessage);
    return 0;
  }

  //处理链接断开信息
  if(pMessage->GetMessageBase()->m_u2Cmd == CLIENT_LINK_CDISCONNET)
  {
    OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] CLIENT_LINK_CDISCONNET OK.\n"));
	Do_Proxy_DisConnect(pMessage);
    return 0;
  }

  //处理正常信息
  if(pMessage->GetMessageBase()->m_u2Cmd == COMMAND_PROXY)
  {
	  Do_Proxy_Data(pMessage);
  }

  return 0;

  __LEAVE_FUNCTION_WITHRETURN(0);
}

void CBaseCommand::Init()
{
	uint32 u4WorkThreadCount = m_pServerObject->GetMessageManager()->GetWorkThreadCount();
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] WorkThread Count=%d.\n", u4WorkThreadCount));

	App_ProxyThreadManager::instance()->Init(u4WorkThreadCount);


}

void CBaseCommand::Do_Proxy_Connect(IMessage* pMessage)
{
	ACE_INET_Addr AddrProxyServer;
	AddrProxyServer.set((int)PROXY_SERVER_PORT, PROXY_SERVER_IP);

	//获得当前的工作线程ID
	uint32 u4ThreadID = pMessage->GetMessageBase()->m_u4WorkThreadID;

	CProxyClient* pProxyClient = new CProxyClient();
	if(NULL == pProxyClient)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Proxy_Connect]pProxyClient new is NULL.\n"));
		return;
	}

	//设置关联关系
	pProxyClient->SetWorkThreadID(u4ThreadID);
	pProxyClient->SetServerObject(pMessage->GetMessageBase()->m_u4ConnectID, m_pServerObject);
	pProxyClient->reactor(App_ProxyThreadManager::instance()->GetProxyClientReactor(u4ThreadID));

	//连接远程服务器
	int nError = App_ProxyThreadManager::instance()->GetProxyClientConnector(u4ThreadID)->connect(pProxyClient, AddrProxyServer);
	if(nError != 0)
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Proxy_Connect]Connect Fail.\n"));
		return;
	}
}

void CBaseCommand::Do_Proxy_DisConnect( IMessage* pMessage )
{
	//获得当前的工作线程ID
	uint32 u4ThreadID = pMessage->GetMessageBase()->m_u4WorkThreadID;

	CProxyClient* pProxyClient = App_ProxyThreadManager::instance()->GetProxyClientManager(u4ThreadID)->FindProxyClient(pMessage->GetMessageBase()->m_u4ConnectID);
	if(NULL != pProxyClient)
	{
		//需要关闭远程的连接
		pProxyClient->Close();
	}
}

void CBaseCommand::Do_Proxy_Data( IMessage* pMessage )
{
	//获得当前的工作线程ID
	uint32 u4ThreadID = pMessage->GetMessageBase()->m_u4WorkThreadID;

	CProxyClient* pProxyClient =  App_ProxyThreadManager::instance()->GetProxyClientManager(u4ThreadID)->FindProxyClient(pMessage->GetMessageBase()->m_u4ConnectID);
	if(NULL != pProxyClient)
	{
		_PacketInfo HeadPacket;
		_PacketInfo BodyPacket;
		pMessage->GetPacketHead(HeadPacket);
		pMessage->GetPacketBody(BodyPacket);

		char szPostData[MAX_RECV_BUFF] = {'\0'};
		int nSendSize = BodyPacket.m_nDataLen;

		if(nSendSize > MAX_RECV_BUFF)
		{
			OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Proxy_Data]data is more than MAX_RECV_BUFF.\n"));
			return;
		}

		//组装转发包
		ACE_OS::memcpy(szPostData, HeadPacket.m_pData, HeadPacket.m_nDataLen);	
		ACE_OS::memcpy(&szPostData[HeadPacket.m_nDataLen], BodyPacket.m_pData, BodyPacket.m_nDataLen);	

		pProxyClient->SendData(szPostData, BodyPacket.m_nDataLen + HeadPacket.m_nDataLen);
	}
	else
	{
		OUR_DEBUG((LM_ERROR, "[CBaseCommand::Do_Proxy_Data]pProxyClient is Lost.\n"));
	}
}
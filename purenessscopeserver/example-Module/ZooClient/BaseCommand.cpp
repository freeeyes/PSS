#include "BaseCommand.h"

CBaseCommand::CBaseCommand(void)
{
  m_pServerObject = NULL;
  m_nCount        = 0;
}

CBaseCommand::~CBaseCommand(void)
{
}

void CBaseCommand::Init()
{
	//实现freeeyes节点子节点数据变化的监控
	m_ZKClient.Init(ZOOKEEPER_ADDRESS);
	
	Watch_Zoo_Path_Child();
}

void CBaseCommand::Fini()
{
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::Fini] Begin.\n"));
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::Fini] End.\n"));
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

void CBaseCommand::Watch_Zoo_Path_Child()
{
	int nCount = m_ZKClient.Get_Node_Count("/freeeyes");
	OUR_DEBUG((LM_ERROR, "[CBaseCommand::Watch_Zoo_Path_Child] nCount=%d.\n", nCount));
}


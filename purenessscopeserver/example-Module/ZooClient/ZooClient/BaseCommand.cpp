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
	//ʵ��freeeyes�ڵ��ӽڵ����ݱ仯�ļ��
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
  //__ENTER_FUNCTION���Զ�����ĺ���׷��try�ĺ꣬���ڲ�׽���������ʹ��__THROW_FUNCTION()�׳���Ĵ���
  //����__THROW_FUNCTION("hello"); �����Զ���Log�ļ����µ�assert.log��¼�ļ�������������������������
  //�Ƽ���ʹ������꣬��סһ��Ҫ�ں����Ľ���ʹ��__LEAVE_FUNCTION����__LEAVE_FUNCTION_WITHRETURN��֮��ԡ�
  //__LEAVE_FUNCTION_WITHRETURN��__LEAVE_FUNCTION��ͬ��ǰ�߿���֧�ֺ����˳���ʱ�򷵻�һ������
  //������������Ƿ���һ��int
  //��ôĩβ����__LEAVE_FUNCTION_WITHRETURN(0); 0���Ƿ��ص�int����Ȼ��Ҳ֧�ֱ�����ͱ��������忴���Լ��ĺ�����
  //��������������κα����������ʹ��__LEAVE_FUNCTION���ɡ�
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


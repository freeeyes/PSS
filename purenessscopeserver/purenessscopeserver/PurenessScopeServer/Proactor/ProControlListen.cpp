#include "ProControlListen.h"

CProControlListen::CProControlListen()
{
}

CProControlListen::~CProControlListen()
{
}

bool CProControlListen::AddListen( const char* pListenIP, uint32 u4Port, uint8 u1IPType )
{
	bool blState = App_ProConnectAcceptManager::instance()->CheckIPInfo(pListenIP, u4Port);
	if(true == blState)
	{
		//当前监听已经存在，不可以重复建设
		OUR_DEBUG((LM_INFO, "[CProControlListen::AddListen](%s:%d) is exist.\n", pListenIP, u4Port));
		return false;
	}

	//创建一个新的accept对象
	ProConnectAcceptor* pProConnectAcceptor = App_ProConnectAcceptManager::instance()->GetNewConnectAcceptor();
	if(NULL == pProConnectAcceptor)
	{
		OUR_DEBUG((LM_INFO, "[CProControlListen::AddListen](%s:%d) new ConnectAcceptor error.\n", pListenIP, u4Port));
		return false;
	}

	ACE_INET_Addr listenAddr;
	//判断IPv4还是IPv6
	int nErr = 0;
	if(u1IPType == TYPE_IPV4)
	{
		nErr = listenAddr.set(u4Port, pListenIP);
	}
	else
	{
		nErr = listenAddr.set(u4Port, pListenIP, 1, PF_INET6);
	}

	if(nErr != 0)
	{
		OUR_DEBUG((LM_INFO, "[CProControlListen::AddListen](%s:%d)set_address error[%d].\n", pListenIP, u4Port, errno));
		return false;
	}

	//创建新的监听
	//设置监听IP信息
	pProConnectAcceptor->SetListenInfo(pListenIP, u4Port);

	ACE_Proactor* pProactor = App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE);
	if(NULL == pProactor)
	{
		OUR_DEBUG((LM_INFO, "[CProControlListen::AddListen]App_ProactorManager::instance()->GetAce_Proactor(REACTOR_CLIENTDEFINE) is NULL.\n"));
		return false;
	}

	int nRet = pProConnectAcceptor->open(listenAddr, 0, 1, App_MainConfig::instance()->GetBacklog(), 1, pProactor);
	if(-1 == nRet)
	{
		OUR_DEBUG((LM_INFO, "[CProControlListen::AddListen] Listen from [%s:%d] error(%d).\n",listenAddr.get_host_addr(), listenAddr.get_port_number(), errno));
		return false;
	}

	OUR_DEBUG((LM_INFO, "[CProControlListen::AddListen](%s:%d)Add Listen success.\n", pListenIP, u4Port));

	return true;
}

bool CProControlListen::DelListen(const char* pListenIP, uint32 u4Port)
{
	bool blState = App_ProConnectAcceptManager::instance()->CheckIPInfo(pListenIP, u4Port);
	if(false == blState)
	{
		//当前监听已经存在，不可以重复建设
		OUR_DEBUG((LM_INFO, "[CProControlListen::AddListen](%s:%d) is exist.\n", pListenIP, u4Port));
		return false;
	}

	return App_ProConnectAcceptManager::instance()->Close(pListenIP, u4Port);
}

void CProControlListen::ShowListen( vecProControlInfo& objProControlInfo )
{
	objProControlInfo.clear();
	for(int i = 0; i < App_ProConnectAcceptManager::instance()->GetCount(); i++)
	{
		ProConnectAcceptor* pProConnectAcceptor = App_ProConnectAcceptManager::instance()->GetConnectAcceptor(i);
		if(NULL != pProConnectAcceptor)
		{
			_ProControlInfo objInfo;
			sprintf_safe(objInfo.m_szListenIP, 
				MAX_BUFF_20, "%s", pProConnectAcceptor->GetListenIP());
			objInfo.m_u4Port = pProConnectAcceptor->GetListenPort();
			objProControlInfo.push_back(objInfo);
		}
	}
}

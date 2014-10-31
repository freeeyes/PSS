#include "ClientProConnectManager.h"
#include "MessageBlockManager.h"

CProactorClientInfo::CProactorClientInfo()
{
	m_pProConnectClient = NULL;
	m_pProAsynchConnect = NULL;
	m_pClientMessage    = NULL;
	m_szServerIP[0]     = '\0';
	m_nPort             = 0;
	m_nServerID         = 0;
	m_emConnectState    = SERVER_CONNECT_READY;
	m_AddrLocal         = (ACE_INET_Addr &)ACE_Addr::sap_any;
}

CProactorClientInfo::~CProactorClientInfo()
{
}

bool CProactorClientInfo::Init(const char* pIP, int nPort, uint8 u1IPType, int nServerID, CProAsynchConnect* pProAsynchConnect, IClientMessage* pClientMessage)
{
	OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Init]SetAddrServer(%s:%d) Begin.\n", pIP, nPort));

	int nRet = 0;
	if(u1IPType == TYPE_IPV4)
	{
		nRet = m_AddrServer.set(nPort, pIP);
	}
	else
	{
		nRet = m_AddrServer.set(nPort, pIP, 1, PF_INET6);
	}
	if(-1 == nRet)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Init]nServerID = %d, adrClient(%s:%d) error.\n", nServerID, pIP, nPort));
		return false;
	}

	m_pProAsynchConnect = pProAsynchConnect;
	m_pClientMessage    = pClientMessage;
	m_nServerID         = nServerID;

	sprintf_safe(m_szServerIP, MAX_BUFF_20, "%s", pIP);
	m_nPort = nPort;

	return true;
}

bool CProactorClientInfo::Run(bool blIsReadly, EM_Server_Connect_State emState)
{
	if(NULL != m_pProConnectClient)
	{
		OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]Connect is exist.\n"));
		return false;
	}

	/*
	//链接远端服务器
	if(m_pProAsynchConnect->GetConnectState() == true)
	{
	OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pProAsynchConnect is run.\n"));
	return false;
	}
	*/

	if(true == blIsReadly && SERVER_CONNECT_FIRST != m_emConnectState && SERVER_CONNECT_RECONNECT != m_emConnectState)
	{
		m_pProAsynchConnect->SetConnectState(true);
		OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]Connect IP=%s,Port=%d.\n", m_AddrServer.get_host_addr(), m_AddrServer.get_port_number()));
		
		//创建一个数据参数，传给远端
		_ProConnectState_Info* pProConnectInfo = new _ProConnectState_Info();
		pProConnectInfo->m_nServerID = m_nServerID;
		
		m_emConnectState = emState;
		if(m_pProAsynchConnect->connect(m_AddrServer, (const ACE_INET_Addr &)m_AddrLocal, 1, (const void*)pProConnectInfo) == -1)
		{
			OUR_DEBUG((LM_ERROR, "[CProactorClientInfo::Run]m_pAsynchConnect open error(%d).\n", ACE_OS::last_error()));
			return false;
		}
	}

	return true;
}

bool CProactorClientInfo::SendData(ACE_Message_Block* pmblk)
{
	if(NULL == m_pProConnectClient)
	{
		//如果连接正在建立中，等5ms看看连接是否建立
		if(SERVER_CONNECT_FIRST == m_emConnectState || SERVER_CONNECT_RECONNECT == m_emConnectState)
		{
			ACE_Time_Value tvSleep(0, 5000);
			ACE_OS::sleep(tvSleep);
		}

		if(NULL == m_pProConnectClient)
		{
			//如果连接不存在，则建立链接。
			if(SERVER_CONNECT_FIRST != m_emConnectState && SERVER_CONNECT_RECONNECT != m_emConnectState)
			{
				//如果连接不存在，则建立链接。
				Run(true);
			}

			if(NULL != pmblk)
			{
				pmblk->release();
			}

			//如果消息有处理接口，则返回失败接口
			if(NULL != m_pClientMessage)
			{
				//服务器已经断开，需要等待重新连接的结果
				_ClientIPInfo objServerIPInfo;
				sprintf_safe(objServerIPInfo.m_szClientIP, MAX_BUFF_20, "%s", m_AddrServer.get_host_addr());
				objServerIPInfo.m_nPort = m_AddrServer.get_port_number();
				m_pClientMessage->ConnectError(101, objServerIPInfo);
			}

			return false;
		}
	}

	//发送数据
	return m_pProConnectClient->SendData(pmblk);

}

int CProactorClientInfo::GetServerID()
{
	if(NULL == m_pProConnectClient)
	{
		return 0;
	}
	else
	{
		return m_pProConnectClient->GetServerID();
	}
}

bool CProactorClientInfo::Close()
{
	if(NULL == m_pProConnectClient)
	{
		return false;
	}
	else
	{
		m_pProConnectClient->ClientClose();
		return true;
	}

	SAFE_DELETE(m_pClientMessage);
}

void CProactorClientInfo::SetProConnectClient(CProConnectClient* pProConnectClient)
{
	m_pProConnectClient = pProConnectClient;
}

CProConnectClient* CProactorClientInfo::GetProConnectClient()
{
	return m_pProConnectClient;
}

IClientMessage* CProactorClientInfo::GetClientMessage()
{
	//这里增加是否是连接重练的判定以及是否是第一次连接的回调
	if((m_emConnectState == SERVER_CONNECT_RECONNECT || m_emConnectState == SERVER_CONNECT_FIRST) && NULL != m_pClientMessage)
	{
		//通知上层某一个连接已经恢复或者已建立
		m_pClientMessage->ReConnect(m_nServerID);
	}

	m_emConnectState = SERVER_CONNECT_OK;
	return m_pClientMessage;
}

ACE_INET_Addr CProactorClientInfo::GetServerAddr()
{
	return m_AddrServer;
}

EM_Server_Connect_State CProactorClientInfo::GetServerConnectState()
{
	return m_emConnectState;
}

void CProactorClientInfo::SetServerConnectState( EM_Server_Connect_State objState )
{
	m_emConnectState = objState;
}

void CProactorClientInfo::SetLocalAddr( const char* pIP, int nPort, uint8 u1IPType )
{
	if(u1IPType == TYPE_IPV4)
	{
		m_AddrLocal.set(nPort, pIP);
	}
	else
	{
		m_AddrLocal.set(nPort, pIP, 1, PF_INET6);
	}
}

CClientProConnectManager::CClientProConnectManager(void)
{
	m_nTaskID          = -1;
	m_blProactorFinish = false;
}

CClientProConnectManager::~CClientProConnectManager(void)
{
	Close();
}

bool CClientProConnectManager::Init(ACE_Proactor* pProactor)
{
	if(pProactor == NULL)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Init]pProactor is NULL.\n"));
		return false;
	}

	m_u4ConnectServerTimeout = PRO_CONNECT_SERVER_TIMEOUT;

	if(-1 == m_ProAsynchConnect.open(false, pProactor, true))
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Init]m_ProAsynchConnect open error(%d).\n", ACE_OS::last_error()));
		return false;
	}
	else
	{
		//标记Proactor已经连接成功
		m_blProactorFinish = true;

		//连接器启动成功，这时候启动定时器
		m_ActiveTimer.activate();

		return true;
	}
}

bool CClientProConnectManager::Connect(int nServerID, const char* pIP, int nPort, uint8 u1IPType, IClientMessage* pClientMessage)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f != m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	//初始化链接信息
	CProactorClientInfo* pClientInfo = new CProactorClientInfo();
	if(false == pClientInfo->Init(pIP, nPort, u1IPType, nServerID, &m_ProAsynchConnect, pClientMessage))
	{
		delete pClientInfo;
		pClientInfo = NULL;
		return false;
	}

	//第一次开始链接
	if(false == pClientInfo->Run(m_blProactorFinish, SERVER_CONNECT_FIRST))
	{
		delete pClientInfo;
		pClientInfo = NULL;
		return false;
	}

	//链接已经建立，添加进map
	m_mapClientInfo[nServerID] = pClientInfo;
	OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));

	//自动休眠0.1秒
	ACE_Time_Value tvSleep(0, m_u4ConnectServerTimeout);
	ACE_OS::sleep(tvSleep);

	return true;
}

bool CClientProConnectManager::Connect( int nServerID, const char* pIP, int nPort, uint8 u1IPType, const char* pLocalIP, int nLocalPort, uint8 u1LocalIPType, IClientMessage* pClientMessage )
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f != m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	//初始化链接信息
	CProactorClientInfo* pClientInfo = new CProactorClientInfo();
	if(false == pClientInfo->Init(pIP, nPort, u1IPType, nServerID, &m_ProAsynchConnect, pClientMessage))
	{
		delete pClientInfo;
		pClientInfo = NULL;
		return false;
	}

	//设置本地IP和端口
	pClientInfo->SetLocalAddr(pLocalIP, nLocalPort, u1LocalIPType);

	//第一次开始链接
	if(false == pClientInfo->Run(m_blProactorFinish, SERVER_CONNECT_FIRST))
	{
		delete pClientInfo;
		pClientInfo = NULL;
		return false;
	}

	//链接已经建立，添加进map
	m_mapClientInfo[nServerID] = pClientInfo;
	OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Connect]nServerID =(%d) connect is OK.\n", nServerID));

	//自动休眠0.1秒
	ACE_Time_Value tvSleep(0, m_u4ConnectServerTimeout);
	ACE_OS::sleep(tvSleep);

	return true;
}

bool CClientProConnectManager::SetHandler(int nServerID, CProConnectClient* pProConnectClient)
{
	if(NULL == pProConnectClient)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SetHandler]pProConnectClient is NULL.\n"));
		return false;
	}

	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不再添加到已经存在的客户端map管理中
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::SetHandler]nServerID =(%d) is not exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	pClientInfo->SetProConnectClient(pProConnectClient);
	return true;
}

bool CClientProConnectManager::Close(int nServerID)
{
	//如果是因为服务器断开，则只删除ProConnectClient的指针
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	//关闭链接对象
	if(NULL != pClientInfo->GetProConnectClient())
	{
		pClientInfo->GetProConnectClient()->ClientClose();
	}

	pClientInfo->SetProConnectClient(NULL);
	SAFE_DELETE(pClientInfo);
	//从map里面删除当前存在的对象
	m_mapClientInfo.erase(f);
	return true;
}

bool CClientProConnectManager::ConnectErrorClose(int nServerID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	//从map里面删除当前存在的对象
	m_mapClientInfo.erase(f);
	SAFE_DELETE(pClientInfo);

	return true;
}

IClientMessage* CClientProConnectManager::GetClientMessage(int nServerID)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		return NULL;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL != pClientInfo)
	{
		return pClientInfo->GetClientMessage();
	}

	return NULL;
}

bool CClientProConnectManager::SendData(int nServerID, const char* pData, int nSize, bool blIsDelete)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is not exist.\n", nServerID));
		if(true == blIsDelete)
		{
			SAFE_DELETE_ARRAY(pData);
		}
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;

	ACE_Message_Block* pmblk = App_MessageBlockManager::instance()->Create(nSize);
	if(NULL == pmblk)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pmblk is NULL.\n", nServerID));
		if(true == blIsDelete)
		{
			SAFE_DELETE_ARRAY(pData);
		}
		return false;
	}

	ACE_OS::memcpy(pmblk->wr_ptr(), pData, nSize);
	pmblk->wr_ptr(nSize);

	if(true == blIsDelete)
	{
		SAFE_DELETE_ARRAY(pData);
	}

	//发送数据
	return pClientInfo->SendData(pmblk);
}

bool CClientProConnectManager::StartConnectTask(int nIntervalTime)
{
	CancelConnectTask();
	m_nTaskID = m_ActiveTimer.schedule(this, (void* )NULL, ACE_OS::gettimeofday() + ACE_Time_Value(nIntervalTime), ACE_Time_Value(nIntervalTime));
	if(m_nTaskID == -1)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::StartConnectTask].StartConnectTask is fail, time is (%d).\n", nIntervalTime));
		return false;
	}

	m_ActiveTimer.activate();
	return true;
}

void CClientProConnectManager::CancelConnectTask()
{
	if(m_nTaskID != -1)
	{
		//杀死之前的定时器，重新开启新的定时器
		m_ActiveTimer.cancel(m_nTaskID);
		m_nTaskID = -1;
	}
}

void CClientProConnectManager::Close()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

	//如果有定时器，关闭定时器
	CancelConnectTask();

	//关闭所有已存在的链接
	mapProactorClientInfo::iterator b = m_mapClientInfo.begin();
	mapProactorClientInfo::iterator e = m_mapClientInfo.end();
	for(b; b!= e; b++)
	{
		CProactorClientInfo* pClientInfo = (CProactorClientInfo* )b->second;
		pClientInfo->Close();
		SAFE_DELETE(pClientInfo);
	}

	m_ActiveTimer.deactivate();
	m_mapClientInfo.clear();
}

int CClientProConnectManager::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

	//OUR_DEBUG((LM_DEBUG, "[CClientProConnectManager::handle_timeout]Begin.\n"));
	if(m_ProAsynchConnect.GetConnectState() == true)
	{
		return 0;
	}

	mapProactorClientInfo::iterator b = m_mapClientInfo.begin();
	mapProactorClientInfo::iterator e = m_mapClientInfo.end();

	for(b; b!= e; b++)
	{
		int nServerID = (int)b->first;

		/*
		//测试代码
		if(GetConnectState(nServerID) == false)
		{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::handle_timeout]nServerID == false (%d).\n", nServerID));
		}
		else
		{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::handle_timeout]nServerID == true (%d).\n", nServerID));
		}
		*/

		CProactorClientInfo* pClientInfo = (CProactorClientInfo* )b->second;
		if(NULL == pClientInfo->GetProConnectClient())
		{
			//如果连接不存在，则重新建立连接
			pClientInfo->Run(m_blProactorFinish);

			//自动休眠0.1秒
			ACE_Time_Value tvSleep(0, m_u4ConnectServerTimeout);
			ACE_OS::sleep(tvSleep);
		}
	}
	return 0;
}

void CClientProConnectManager::GetConnectInfo(vecClientConnectInfo& VecClientConnectInfo)
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);

	VecClientConnectInfo.clear();

	mapProactorClientInfo::iterator b = m_mapClientInfo.begin();
	mapProactorClientInfo::iterator e = m_mapClientInfo.end();
	for(b; b!= e; b++)
	{
		CProactorClientInfo* pClientInfo = (CProactorClientInfo* )b->second;
		if(NULL != pClientInfo)
		{
			if(NULL != pClientInfo->GetProConnectClient())
			{
				//链接已经建立
				_ClientConnectInfo ClientConnectInfo = pClientInfo->GetProConnectClient()->GetClientConnectInfo();
				ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
				VecClientConnectInfo.push_back(ClientConnectInfo);
			}
			else
			{
				//连接未建立
				_ClientConnectInfo ClientConnectInfo;
				ClientConnectInfo.m_addrRemote = pClientInfo->GetServerAddr();
				ClientConnectInfo.m_blValid    = false;
				VecClientConnectInfo.push_back(ClientConnectInfo);
			}
		}
	}
}

bool CClientProConnectManager::CloseByClient(int nServerID)
{
	//如果是因为远程客户端断开，则只删除ProConnectClient的指针
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		OUR_DEBUG((LM_ERROR, "[CClientProConnectManager::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	pClientInfo->SetProConnectClient(NULL);
	pClientInfo->SetServerConnectState(SERVER_CONNECT_FAIL);

	return true;
}


bool CClientProConnectManager::GetConnectState(int nServerID)
{
	//检查当前连接是否是活跃的
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::GetConnectState]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::GetConnectState]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	if(NULL == pClientInfo->GetProConnectClient())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CClientProConnectManager::ReConnect( int nServerID )
{
	//检查当前连接是否是活跃的
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::Close]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}

	CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
	if(NULL == pClientInfo)
	{
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::Close]nServerID =(%d) pClientInfo is NULL.\n", nServerID));
		return false;
	}

	if(NULL == pClientInfo->GetProConnectClient())
	{
		//如果连接不存在，则重新建立连接
		pClientInfo->Run(m_blProactorFinish);

		//自动休眠0.1秒
		ACE_Time_Value tvSleep(0, m_u4ConnectServerTimeout);
		ACE_OS::sleep(tvSleep);

		return true;
	}
	else
	{
		return true;
	}
}

ACE_INET_Addr CClientProConnectManager::GetServerAddr(int nServerID)
{
	ACE_INET_Addr remote_addr;
	//检查当前连接是否是活跃的
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::Close]nServerID =(%d) is exist.\n", nServerID));
		return remote_addr;
	}
	else
	{
		CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
		remote_addr = pClientInfo->GetServerAddr();
		return remote_addr;
	}
}

bool CClientProConnectManager::SetServerConnectState(int nServerID, EM_Server_Connect_State objState)
{
	ACE_INET_Addr remote_addr;
	//检查当前连接是否是活跃的
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	mapProactorClientInfo::iterator f = m_mapClientInfo.find(nServerID);
	if(f == m_mapClientInfo.end())
	{
		//如果这个链接已经存在，则不创建新的链接
		//OUR_DEBUG((LM_ERROR, "[GetConnectState::Close]nServerID =(%d) is exist.\n", nServerID));
		return false;
	}
	else
	{
		CProactorClientInfo* pClientInfo = (CProactorClientInfo* )f->second;
		pClientInfo->SetServerConnectState(objState);
		return true;
	}
}

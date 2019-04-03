#include "zkClient.h"

void watcher(zhandle_t *zzh, int type, int state, const char *path,
             void* context)
{
	CZKClient* pZKClient = (CZKClient* )context;
	printf("[watcher]type=%d,state=%d.\n", type, state);
	
	if(NULL == zzh)
	{
		printf("[watcher]zzh is NULL.\n");
		return;
	}
	
	/*
	type
	#define CREATED_EVENT_DEF 1
	#define DELETED_EVENT_DEF 2
	#define CHANGED_EVENT_DEF 3
	#define CHILD_EVENT_DEF 4
	#define SESSION_EVENT_DEF -1
	#define NOTWATCHING_EVENT_DEF -2
	
	state
	#define EXPIRED_SESSION_STATE_DEF -112
	#define AUTH_FAILED_STATE_DEF -113
	#define CONNECTING_STATE_DEF 1
	#define ASSOCIATING_STATE_DEF 2
	#define CONNECTED_STATE_DEF 3
	#define NOTCONNECTED_STATE_DEF 999
	*/
	
	if(type == ZOO_SESSION_EVENT)
	{
		printf("[watcher]ZOO_SESSION_EVENT.\n");
	}
	else if(type == ZOO_CHANGED_EVENT)
	{
		//继续监控节点变化
		char szValue[50] = {'\0'};
		int nValue = sizeof(szValue);		
		
		pZKClient->Get_Node(path, szValue, nValue);
		
		printf("[watcher]path=%s,szValue=%s.\n", path, szValue);
	}
	else if(type == ZOO_CHILD_EVENT)
	{
		//子节点发生变化
		int nCount = pZKClient->Get_Node_Count("/freeeyes");
		printf("[watcher]path=%s,count=%d.\n", path, nCount);
	}
}

CZKClient::CZKClient()
{
	m_pZh  = NULL;
}

CZKClient::~CZKClient()
{
	Close();
}

bool CZKClient::Init(const char* pConnectInfo)
{
  //zoo_set_debug_level(ZOO_LOG_LEVEL_WARN);
  //zoo_deterministic_conn_order(1); // enable deterministic order
  m_pZh = zookeeper_init(pConnectInfo, watcher, 10000, 0, (void*)this, 0);
  if (m_pZh == NULL) 
  {
  	printf("[CZKClient::Init]errno=%s.\n", strerror(errno));
  	return false;
  }
	
	if(NULL != m_pZh)
	{
		printf("[CZKClient::Init]OK.\n");
	}
	return true;
}

void CZKClient::Close()
{
	if(NULL != m_pZh)
	{
		zookeeper_close(m_pZh);
	}
}

bool CZKClient::Create_Node(const char* pPath, const char* pData, int nLen)
{
	//同步方式创建节点
	if(NULL == m_pZh)
	{
		return false;
	}
	
	char path_buffer[64] = {'\0'};  
  int bufferlen = sizeof(path_buffer); 
	int nFlag = zoo_create(m_pZh, pPath, pData, nLen, &ZOO_OPEN_ACL_UNSAFE, 0, path_buffer, bufferlen);
	if(nFlag != ZOK)
	{
		printf("[CZKClient::Create_Node]Create_Node fail.\n");
	}
	
	printf("[CZKClient::Create_Node]Create pathname=%s.\n", path_buffer);
	return true;
}

bool CZKClient::Get_Node(const char* pPath, char* pData, int& nLen)
{
	//同步方式获得节点数据
	if(NULL == m_pZh)
	{
		return false;
	}	
	
	int nFlag = zoo_get(m_pZh, pPath, 1, pData, &nLen, NULL);
	if(nFlag != ZOK)
	{
		printf("[CZKClient::Get_Node]Get_Node fail.\n");
	}
	
	return true;
}

bool CZKClient::Set_Node(const char* pPath, char* pData, int& nLen)
{
	//同步方式设置节点数据
	if(NULL == m_pZh)
	{
		return false;
	}	
	
	int nFlag = zoo_set(m_pZh, pPath, pData, nLen, -1);
	if(nFlag != ZOK)
	{
		printf("[CZKClient::Get_Node]Get_Node fail.\n");
	}
	
	return true;	
}

int CZKClient::Get_Node_Count(const char* pPath)
{
	int nCount = 0;
	
	//同步获得子节点个数
	if(NULL == m_pZh)
	{
		return 0;
	}	
	
	String_vector strings;
	zoo_get_children(m_pZh, pPath, 1, &strings);
	
	for(int i = 0; i < strings.count; i++)
	{
		printf("[CZKClient::Get_Node_Count]Count=%d,Data=%s.\n", strings.count, strings.data[i]);
	}
	nCount = strings.count;
	
	return nCount;
}

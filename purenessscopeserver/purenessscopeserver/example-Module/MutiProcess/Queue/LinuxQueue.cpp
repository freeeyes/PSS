// 支持共享内存缓冲的登陆服务器插件
// 此插件演示了两块共享内存，用户验证和用户信息。
// 在不同模式下的缓冲接口，提供以共享内存为基础的先入先出环形结构
//add by freeeyes
//2013-10-25

#include "BaseCommand.h"
#include "IObject.h"

static const char *g_szDesc      = "Linux信息队列";       //模块的描述文字
static const char *g_szName      = "Linux信息队列";       //模块的名字
static const char *g_szModuleKey = "LinuxQueue";          //模块的Key

#ifdef WIN32
#ifdef TEST_TCP_BUILD_QUEUE_DLL
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#else
#define DECLDIR
#endif

extern "C"
{
	DECLDIR int LoadModuleData(CServerObject* pServerObject);
	DECLDIR int UnLoadModuleData();
	DECLDIR const char* GetDesc();
	DECLDIR const char* GetName();
	DECLDIR const char* GetModuleKey();
	DECLDIR int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
}

static CBaseCommand g_BaseCommand;
CServerObject*      g_pServerObject = NULL;

int LoadModuleData(CServerObject* pServerObject)
{
	g_pServerObject = pServerObject;
	OUR_DEBUG((LM_INFO, "[Base LoadModuleData] Begin.\n"));
	if(g_pServerObject != NULL)
	{
		g_BaseCommand.SetServerObject(pServerObject);	
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pServerObject is NULL.\n"));
	}

	IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();
	if(NULL != pMessageManager)
	{
		pMessageManager->AddClientCommand(COMMAND_LINUXQUEUE, &g_BaseCommand, g_szName);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
	}		

	bool blInit = g_BaseCommand.InitQueue();
	if(false == blInit)
	{
		OUR_DEBUG((LM_INFO, "[Base LoadModuleData] InitQueue is fail,this mudule is not support Windows!!!\n"));
	}

	OUR_DEBUG((LM_INFO, "[Base LoadModuleData] End.\n"));

	return 0;
}

int UnLoadModuleData()
{
	OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] Begin.\n"));
	if(g_pServerObject != NULL)
	{
		IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();
		if(NULL != pMessageManager)
		{
			pMessageManager->DelClientCommand(COMMAND_LINUXQUEUE, &g_BaseCommand);
			pMessageManager = NULL;
		}
	}

	OUR_DEBUG((LM_INFO, "[Base UnLoadModuleData] End.\n"));
	return 0;
}

const char* GetDesc()
{
	return g_szDesc;
}

const char* GetName()
{
	return g_szName;
}

const char* GetModuleKey()
{
	return g_szModuleKey;
}

//用于模块间的调用接口
int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket)
{
	return 0;
}


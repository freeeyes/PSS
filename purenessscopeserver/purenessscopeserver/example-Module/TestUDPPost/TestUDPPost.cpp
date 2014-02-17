// Base.cpp : 一个UDP的测试类
//用于PurenessScopeServer的测试和使用
//add by freeeyes
//2011-09-20

#include "BaseCommand.h"
#include "IObject.h"

static char *g_szDesc      = "UDPPost测试";       //模块的描述文字
static char *g_szName      = "UDPPost测试";       //模块的名字
static char *g_szModuleKey = "BaseUDPPost";       //模块的Key

#ifdef WIN32
#if defined TEST_UDPPOST_BUILD_DLL
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
		pMessageManager->AddClientCommand(COMMAND_BASE, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(CLIENT_LINK_CONNECT, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(CLIENT_LINK_CDISCONNET, &g_BaseCommand, g_szName);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
	}

	//在这里调用中间服务器链接初始化信息
	g_BaseCommand.InitServer();

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
			pMessageManager->DelClientCommand(COMMAND_BASE, &g_BaseCommand);
			pMessageManager->DelClientCommand(CLIENT_LINK_CONNECT, &g_BaseCommand);
			pMessageManager->DelClientCommand(CLIENT_LINK_CDISCONNET, &g_BaseCommand);
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


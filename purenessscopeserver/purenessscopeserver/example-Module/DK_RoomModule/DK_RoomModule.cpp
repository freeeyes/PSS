// Base.cpp : 一个TCP的测试类
//用于PurenessScopeServer的测试和使用
//add by freeeyes
//2011-09-20

#include "BaseCommand.h"
#include "IObject.h"

static const char *g_szDesc      = "DKPoker-GameCenter";          //模块的描述文字
static const char *g_szName      = "DKPoker-GameCenter";          //模块的名字
static const char *g_szModuleKey = "DKPoker-GameCenter";          //模块的Key

#ifdef WIN32
#ifdef DK_ROOMMODULE_BUILD_DLL
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
		pMessageManager->AddClientCommand(COMMAND_ENTERROOM, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_EXITROOM, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_BEGINROOM, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_ENDROOM, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_SETCARDROOM, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_OUTOLAYER, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_CHANGEROOM, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_CLEARROOM, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_INITROOM, &g_BaseCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_SETROOM, &g_BaseCommand, g_szName);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
	}		

	OUR_DEBUG((LM_INFO, "[Base LoadModuleData] End.\n"));

	//运行Command初始化
	g_BaseCommand.Init(pServerObject->GetTimerManager());

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
			pMessageManager->DelClientCommand(COMMAND_ENTERROOM, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_EXITROOM, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_BEGINROOM, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_ENDROOM, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_SETCARDROOM, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_OUTOLAYER, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_CHANGEROOM, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_CLEARROOM, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_INITROOM, &g_BaseCommand);
			pMessageManager->DelClientCommand(COMMAND_SETROOM, &g_BaseCommand);
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
	if(u2CommandID == COMMAND_MODULE_LOGIN)
	{
		if(NULL != pBuffPacket)
		{
			uint32 u4PlayerID  = PLAYER_INVALID_ID;
			uint32 u4ConnectID = PLAYER_INVALID_ID;
			uint32 u4Pos       = PLAYER_INVALID_ID;

			(*pBuffPacket) >> u4Pos;
			(*pBuffPacket) >> u4PlayerID;
			(*pBuffPacket) >> u4ConnectID;

			if(PLAYER_INVALID_ID != u4PlayerID && PLAYER_INVALID_ID != u4ConnectID && PLAYER_INVALID_ID != u4Pos)
			{
				g_BaseCommand.Module_Login(u4Pos, u4PlayerID, u4ConnectID);
			}
		}
	}
	else if(u2CommandID == COMMAND_MODULE_LOGOUT)
	{
		uint32 u4PlayerID  = PLAYER_INVALID_ID;
		uint32 u4ConnectID = 0;

		(*pBuffPacket) >> u4PlayerID;
		(*pBuffPacket) >> u4ConnectID;
		if(PLAYER_INVALID_ID != u4PlayerID)
		{
			g_BaseCommand.Module_Logout(u4PlayerID, u4ConnectID);
		}
	}
	return 0;
}


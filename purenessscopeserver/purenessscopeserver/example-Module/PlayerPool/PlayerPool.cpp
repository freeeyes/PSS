#include "PlayerPoolCommand.h"
#include "IObject.h"

static char *g_szDesc      = (char* )"PlayerPool";       //模块的描述文字
static char *g_szName      = (char* )"PlayerPool";       //模块的名字
static char *g_szModuleKey = (char* )"PlayerPool";       //模块的Key

#ifdef WIN32
#if defined PLAYERPOOL_BUILD_DLL
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif
#else
#define DECLDIR
#endif

//dllexport dllimport

extern "C"
{
	DECLDIR int LoadModuleData(CServerObject* pServerObject);
	DECLDIR int UnLoadModuleData();
	DECLDIR const char* GetDesc();
	DECLDIR const char* GetName();
	DECLDIR const char* GetModuleKey();
}

static CPlayerPoolCommand g_PlayerPoolCommand;
CServerObject*            g_pServerObject = NULL;

int LoadModuleData(CServerObject* pServerObject)
{
	g_pServerObject = pServerObject;
	OUR_DEBUG((LM_INFO, "[PlayerPool LoadModuleData] Begin.\n"));
	if(g_pServerObject != NULL)
	{
		g_PlayerPoolCommand.SetServerObject(pServerObject);	
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[PlayerPool LoadModuleData] pServerObject is NULL.\n"));
	}

	IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();
	if(NULL != pMessageManager)
	{
		pMessageManager->AddClientCommand(COMMAND_PLAYINSERT, &g_PlayerPoolCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_PLAYUPDATE, &g_PlayerPoolCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_PLAYDELETE, &g_PlayerPoolCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_PLAYSEACH,  &g_PlayerPoolCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_PLAYLOGIN,  &g_PlayerPoolCommand, g_szName);
		pMessageManager->AddClientCommand(COMMAND_PLAYLOGOFF, &g_PlayerPoolCommand, g_szName);
	}
	else
	{
		OUR_DEBUG((LM_INFO, "[PlayerPool LoadModuleData] pMessageManager = NULL.\n"));
	}

	OUR_DEBUG((LM_INFO, "[PlayerPool LoadModuleData] End.\n"));

	return 0;
}

int UnLoadModuleData()
{
	OUR_DEBUG((LM_INFO, "[PlayerPool UnLoadModuleData] Begin.\n"));
	if(g_pServerObject != NULL)
	{
		IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();
		if(NULL != pMessageManager)
		{
			pMessageManager->DelClientCommand(COMMAND_PLAYINSERT, &g_PlayerPoolCommand);
			pMessageManager->DelClientCommand(COMMAND_PLAYUPDATE, &g_PlayerPoolCommand);
			pMessageManager->DelClientCommand(COMMAND_PLAYDELETE, &g_PlayerPoolCommand);
			pMessageManager->DelClientCommand(COMMAND_PLAYSEACH,  &g_PlayerPoolCommand);
			pMessageManager->DelClientCommand(COMMAND_PLAYLOGIN,  &g_PlayerPoolCommand);
			pMessageManager->DelClientCommand(COMMAND_PLAYLOGOFF, &g_PlayerPoolCommand);
			pMessageManager = NULL;
		}
	}
	OUR_DEBUG((LM_INFO, "[PlayerPool UnLoadModuleData] End.\n"));
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


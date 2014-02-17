// Base.cpp : 一个TCP的测试类
//用于模拟一个类似FTP的小功能
//支持登陆，文件根目录列表，下载，上传
//add by freeeyes
//2013-10-15

#include "BaseCommand.h"
#include "IObject.h"

static const char *g_szDesc      = "样例FTP模块";       //模块的描述文字
static const char *g_szName      = "FTP测试";           //模块的名字
static const char *g_szModuleKey = "BaseFCP";           //模块的Key

#ifdef WIN32
#ifdef TEST_TCP_BUILD_DLL
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
    pMessageManager->AddClientCommand(COMMAND_LOGIN, &g_BaseCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_LOGOUT, &g_BaseCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_FILELIST, &g_BaseCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_FILE_DOWNLOAD, &g_BaseCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_FILE_UPLOAD, &g_BaseCommand, g_szName);
    pMessageManager->AddClientCommand(CLIENT_LINK_CONNECT, &g_BaseCommand, g_szName);
    pMessageManager->AddClientCommand(CLIENT_LINK_CDISCONNET, &g_BaseCommand, g_szName);
  }
  else
  {
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
  }		

  g_BaseCommand.InitUserList();
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
      pMessageManager->DelClientCommand(COMMAND_LOGIN, &g_BaseCommand);
	  pMessageManager->DelClientCommand(COMMAND_LOGOUT, &g_BaseCommand);
	  pMessageManager->DelClientCommand(COMMAND_FILELIST, &g_BaseCommand);
	  pMessageManager->DelClientCommand(COMMAND_FILE_DOWNLOAD, &g_BaseCommand);
	  pMessageManager->DelClientCommand(COMMAND_FILE_UPLOAD, &g_BaseCommand);
      pMessageManager->DelClientCommand(CLIENT_LINK_CONNECT, &g_BaseCommand);
      pMessageManager->DelClientCommand(CLIENT_LINK_CDISCONNET, &g_BaseCommand);
      pMessageManager = NULL;
    }
  }

  g_BaseCommand.ClearUserList();
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


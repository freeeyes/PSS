// Base.cpp : 一个TCP的测试类
//用于PurenessScopeServer的测试和使用
//add by freeeyes
//2011-09-20

#include "BaseCommand.h"
#include "IObject.h"

static const char *g_szDesc      = "PlugB模块，和PlugA互动"; //模块的描述文字
static const char *g_szName      = "PlugB模块";              //模块的名字
static const char *g_szModuleKey = "PlugB";                  //模块的Key

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
	  //纯服务模块，和A互动，一个都不注册。
  }
  else
  {
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
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
      //纯服务模块，和A互动，一个都不注册。
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
  if(u2CommandID == MODULE_COMMAND_PLUGB)
  {
	  g_BaseCommand.DoModuleMessage(pBuffPacket, pReturnBuffPacket);
  }
  else if(u2CommandID == MODULE_COMMAND_INIT)
  {
	  g_BaseCommand.DoModuleInit(pBuffPacket, pReturnBuffPacket);
  }
  
  return 0;
}


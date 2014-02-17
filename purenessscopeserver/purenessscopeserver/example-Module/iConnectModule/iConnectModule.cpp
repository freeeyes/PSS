
#include "iConnectCommand.h"
#include "IObject.h"

static const char *g_szDesc      = "iConnect模块";       //模块的描述文字
static const char *g_szName      = "iConnect模块";           //模块的名字
static const char *g_szModuleKey = "iConnect";           //模块的Key

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

static CiConnectCommand g_iConnectCommand;
CServerObject*      g_pServerObject = NULL;

int LoadModuleData(CServerObject* pServerObject)
{
  g_pServerObject = pServerObject;
  OUR_DEBUG((LM_INFO, "[Base LoadModuleData] Begin.\n"));
  if(g_pServerObject != NULL)
  {
    g_iConnectCommand.SetServerObject(pServerObject);	
  }
  else
  {
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pServerObject is NULL.\n"));
  }

  IMessageManager* pMessageManager = g_pServerObject->GetMessageManager();
  if(NULL != pMessageManager)
  {
    pMessageManager->AddClientCommand(COMMAND_REG, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(COMMAND_REG_RESPONSE, &g_iConnectCommand, g_szName);

    pMessageManager->AddClientCommand(COMMAND_KEEP_ALIVE, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(COMMAND_KEEP_ALIVE_RESPONSE, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(COMMAND_QUERY_APP, &g_iConnectCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_QUERY_APP_RESPONSE, &g_iConnectCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_SYN_APP, &g_iConnectCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_QUERY_SERVER, &g_iConnectCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_QUERY_SERVER_RESPONSE, &g_iConnectCommand, g_szName);

    pMessageManager->AddClientCommand(COMMAND_PACKET, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(COMMAND_PACKET_RECEIVED, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(COMMAND_PACKET_SERVERDOWN, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(COMMAND_PACKET_APPDOWN, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(COMMAND_PACKET_REJECT, &g_iConnectCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_PACKET_ERR, &g_iConnectCommand, g_szName);
	pMessageManager->AddClientCommand(COMMAND_PACKET_OUTTIME, &g_iConnectCommand, g_szName);

    pMessageManager->AddClientCommand(CLIENT_LINK_CONNECT, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(CLIENT_LINK_CDISCONNET, &g_iConnectCommand, g_szName);
    pMessageManager->AddClientCommand(CLIENT_LINK_SDISCONNET, &g_iConnectCommand, g_szName);
  }
  else
  {
    OUR_DEBUG((LM_INFO, "[Base LoadModuleData] pMessageManager = NULL.\n"));
  }		
  //初始化配置
  g_iConnectCommand.InitConfig();

  //在这里调用中间服务器链接初始化信息
  g_iConnectCommand.InitServer();

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
      pMessageManager->DelClientCommand(COMMAND_REG, &g_iConnectCommand);
      pMessageManager->DelClientCommand(COMMAND_REG_RESPONSE, &g_iConnectCommand);

      pMessageManager->DelClientCommand(COMMAND_KEEP_ALIVE, &g_iConnectCommand);
      pMessageManager->DelClientCommand(COMMAND_KEEP_ALIVE_RESPONSE, &g_iConnectCommand);
      pMessageManager->DelClientCommand(COMMAND_QUERY_APP, &g_iConnectCommand);
	  pMessageManager->DelClientCommand(COMMAND_QUERY_APP_RESPONSE, &g_iConnectCommand);
	  pMessageManager->DelClientCommand(COMMAND_SYN_APP, &g_iConnectCommand);
	  pMessageManager->DelClientCommand(COMMAND_QUERY_SERVER, &g_iConnectCommand);
	  pMessageManager->DelClientCommand(COMMAND_QUERY_SERVER_RESPONSE, &g_iConnectCommand);

      pMessageManager->DelClientCommand(COMMAND_PACKET, &g_iConnectCommand);
      pMessageManager->DelClientCommand(COMMAND_PACKET_RECEIVED, &g_iConnectCommand);
      pMessageManager->DelClientCommand(COMMAND_PACKET_SERVERDOWN, &g_iConnectCommand);
      pMessageManager->DelClientCommand(COMMAND_PACKET_APPDOWN, &g_iConnectCommand);
      pMessageManager->DelClientCommand(COMMAND_PACKET_REJECT, &g_iConnectCommand);
	  pMessageManager->DelClientCommand(COMMAND_PACKET_ERR, &g_iConnectCommand);
	  pMessageManager->DelClientCommand(COMMAND_PACKET_OUTTIME, &g_iConnectCommand);

      pMessageManager->DelClientCommand(CLIENT_LINK_CONNECT, &g_iConnectCommand);
      pMessageManager->DelClientCommand(CLIENT_LINK_CDISCONNET, &g_iConnectCommand);
      pMessageManager->DelClientCommand(CLIENT_LINK_SDISCONNET, &g_iConnectCommand);
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


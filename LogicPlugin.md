# 如何开发一个逻辑插件  
## （1）设计一个插件的入口  
> 首先，你需要新建一个插件入口的代码文件，这个文件的有几个函数是必须实现的，这样便于框架对插件的加载。  

```cpp
#include "BaseCommand.h"    
#include "IObject.h"  
#include "ace/svc_export.h"
  
static const char* g_szDesc      = "样例TCP模块";       //模块的描述文字  
static const char* g_szName      = "TCP测试";           //模块的名字  
static const char* g_szModuleKey = "BaseTCP";           //模块的Key  
```

> 在一开始，你必须声明你的插件的g_szModuleKey，g_szName和g_szDesc。  
> g_szModuleKey是插件的key，这个用于插件间或者框架加载使用。  
> g_szName是插件的名字信息。  
> g_szDesc是插件的描述信息。  

```cpp
extern "C"
{
    DECLDIR int LoadModuleData(CServerObject* pServerObject);
    DECLDIR int UnLoadModuleData();
    DECLDIR const char* GetDesc();
    DECLDIR const char* GetName();
    DECLDIR const char* GetModuleKey();
    DECLDIR int DoModuleMessage(uint16 u2CommandID, IBuffPacket* pBuffPacket, IBuffPacket* pReturnBuffPacket);
    DECLDIR bool GetModuleState(uint32& u4ErrorID);
}
```

> 这里是需要适时的添加你自己实现的代码。  
> LoadModuleData() 当插件第一次加载，或者重载的时候，框架会调用此方法。实现插件的初始化。  
> 在这里，你需要注册插件的信令，注册指令为  
> pMessageManager->AddClientCommand(COMMAND_BASE, g_BaseCommand, g_szName);  
> COMMAND_BASE为你要需要处理的信令ID。  
> g_BaseCommand为对应COMMAND_BASE处理的类函数。  
> 这里请注意，如果需要，一个g_BaseCommand类是可以实现多个信令的处理。  
> 在收到数据的时候，需要你自己在DoMessage()去分别处理。  
> UnLoadModuleData() 当插件卸载的时候，会调用这个接口，实现插件的卸载。  
> DoModuleMessage() 这个提供了插件间调用的接口。  
> 如果你的插件支持别的插件可以访问你的入口，在这里添加你的代码。  
> u2CommandID是其他插件传入的命令ID，你可以根据这个ID来决定你的插件行为。  
> pBuffPacket是别的插件调用你的参数入口，你可以根据你的需要解析这个IBuffPacket对象(具体看example代码)。  
> pReturnBuffPacket是你需要返回给其他插件的结果，你可以组织你的IBuffPacket对象数据格式。  
> GetModuleState()这个函数是框架定时调用接口，用于检查插件的运行状态，你在这里可以返回你的插件状态。  
> u4ErrorID就是你要返回的状态，如果是0则框架认为是正常的，是其他数值，框架会记录当前插件插件。  

## （2）设计一个信令  
> 你首先需要设计一个命令类，继承框架的信令类
> 例如： 

```cpp  
class CBaseCommand : public CClientCommand
{
public:
    CBaseCommand(void);
    ~CBaseCommand(void);

    int DoMessage(IMessage* pMessage, bool& bDeleteFlag);
    void SetServerObject(CServerObject* pServerObject);

    void ReadIniFile(const char* pIniFileName);

private:
    int Do_Connect(IMessage* pMessage);
    int Do_DisConnect(IMessage* pMessage);
    int Do_ClientSendTimeout(IMessage* pMessage);
    int Do_Base(IMessage* pMessage);
    int Do_ClientSendOk(IMessage* pMessage);


private:
    CServerObject* m_pServerObject;
    int            m_nCount;
};
```
> 在这里，你需要实现DoMessage()的内容，当框架收到指定的消息，会调用这里实现消息的处理。  

```cpp 
int CBaseCommand::DoMessage(IMessage* pMessage, bool& bDeleteFlag)
{
    __ENTER_FUNCTION();

    DO_TRACE("./", pMessage->GetMessageBase()->m_szTraceID);

    if(m_pServerObject == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] m_pServerObject is NULL(%d).\n", bDeleteFlag));
        return -1;
    }

    if(pMessage == NULL)
    {
        OUR_DEBUG((LM_ERROR, "[CBaseCommand::DoMessage] pMessage is NULL.\n"));
        return -1;
    }

    //处理链接建立信息
    MESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);
    MESSAGE_FUNCTION(COMMAND_BASE,            Do_Base,              pMessage);
    MESSAGE_FUNCTION_END;

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}
```  

> 在这里，MESSAGE_FUNCTION()实现对注册的命令映射。  
> 你只要实现Do_Base()函数即可。  
> 这里要说一下，m_pServerObject是框架对象的指针集合。  
> 在这里你可以使用框架的一系列指令。  
> 它们包括：


```cpp
    IMessageManager*       m_pIMessageManager;      //信令注册注销
    ILogManager*           m_pLogManager;           //日志的记录
    IConnectManager*       m_pConnectManager;       //客户端连接，用于数据的发送 
    IPacketManager*        m_pPacketManager;        //IBuffPacket管理类（对象池）
    IClientManager*        m_pClientManager;        //服务器间调用
    IUDPConnectManager*    m_pUDPConnectManager;    //UDP管理类，可以用于发送UDP数据
    ActiveTimer*           m_pTimerManager;         //定时器类 
    IModuleMessageManager* m_pModuleMessageManager; //提供模块间调用方法 
    IControlListen*        m_pContorlListen;        //可以让框架开启，关闭指定的监听TCP端口
    IModuleInfo*           m_pIModuleInfo;          //获得当前框架加载的模块信息
    IServerManager*        m_pIServerManager;       //控制框架启动和关闭
    IMessageBlockManager*  m_pMessageBlockManager;  //MessageBlock管理类(对象池)
    IFrameCommand*         m_pFrameCommand;         //支持控制框架的命令，具体可以参考PSSFrameCommand.md
```

具体使用方法，请参考对应的example实例。
# How to develop a logical plug-in  
## （1）Design a plug-in entry  
> First, you need to create a new plug-in entry code file, this file has several functions must be implemented, so that the framework to plug-in load.  

```cpp
#include "BaseCommand.h"    
#include "IObject.h"  
#include "ace/svc_export.h"
  
static const char* g_szDesc      = "Tcp desc";       //Module description text  
static const char* g_szName      = "TCP test";       //Name of the module  
static const char* g_szModuleKey = "BaseTCP";        //Module's key  
```

> In the beginning, you must declare your plug-in g_szModuleKey，g_szName and g_szDesc。  
> g_szmodulekey is the key of the plug - in, which is used between plug-ins or for frame loading.  
> g_szName is the name information of the plug-in.  
> g_szDesc is the plug-in description information.  

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

> here is the need to add your own implementation of the code in a timely manner.  
> loadmoduledata() this method is called by the framework when the plug-in is first loaded or reloaded. The initialization of the plug-in is realized.  
> here, you need to register the plug-in signaling, registration instructions for  
> pmessagemanager->addclientcommand(command_base, g_basecommand, g_szname);  
> command_base is the signaling id you need to process.  
> g_basecommand is a class function that corresponds to command _ base processing.  
> here, note that if desired, a g_basecommand class is a process that enables multiple signaling.  
> when you receive data, you need to deal with it separately in domessage ( ).  
> unloadmoduledata() when the plug-in uninstall, will call this interface, realize the plug-in uninstall.  
> InitModule() This event is designed to connect to a third-party interface, initialization needs to do, here is the plug-in after the completion of the load, need to complete the third-party server connection and validation, add code here.  
> domodulemessage() this provides an interface for calls between plug - ins.  
> if your plug-in supports other plug-ins to access your portal, add your code here.  
> u2command id is the command id passed in by other plug - ins, and you can determine your plug-in behavior based on this id.  
> pbuffpacket is another plug-in call your parameter entry, you can according to your needs to parse the ibuffpacket object ( see example code ).  
> return buffer is the result of you need to return to other plug - ins, you can organize your ibuffpacket object data format.  
> getmodulestate() this function is the frame timing call interface, used to check the plug-in running status, you can return to your plug-in status here.  
> U4 error id is the state you want to return, if it is 0, the framework is considered normal, is other values, the framework will record the current plug - in.   

## （2）Design a command  
> You first need to design a command class that inherits the framework 's command class
> example： 

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
> Here, you need to implement the content of the Domessage(), when the framework receives the specified message, will call here to implement the processing of the message.  

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

    //dispose message
    MESSAGE_FUNCTION_BEGIN(pMessage->GetMessageBase()->m_u2Cmd);
    MESSAGE_FUNCTION(COMMAND_BASE,            Do_Base,              pMessage);
    MESSAGE_FUNCTION_END;

    return 0;

    __LEAVE_FUNCTION_WITHRETURN(0);
}
```  

> here, message_function() implements a command mapping for registration.  
> you just implement the do_base() function.  
> here, m_pserverobject is a collection of pointers to frame objects.
> here you can use the framework of a series of instructions.
> they include:  

```cpp
    IMessageManager*       m_pIMessageManager;      //message registration deregistration
    ILogManager*           m_pLogManager;           //Log records
    IConnectManager*       m_pConnectManager;       //Client connection for data transmission 
    IPacketManager*        m_pPacketManager;        //IBuffPacket manager (object pool)  
    IClientManager*        m_pClientManager;        //Inter - server call
    IUDPConnectManager*    m_pUDPConnectManager;    //UDP manager，Can be used to send UDP data
    ActiveTimer*           m_pTimerManager;         //Timer
    IModuleMessageManager* m_pModuleMessageManager; //Provides an inter-module call method 
    IControlListen*        m_pContorlListen;        //Allows the framework to open and close the specified listening TCP port
    IModuleInfo*           m_pIModuleInfo;          //Gets the module information loaded by the current frame
    IServerManager*        m_pIServerManager;       //Control frame start and close
    IMessageBlockManager*  m_pMessageBlockManager;  //MessageBlock manager(object pool)
    IFrameCommand*         m_pFrameCommand;         //Support control framework commands, specific reference (here)[PSSFrameCommand.md]
```

For specific usage, refer to the corresponding example instance.
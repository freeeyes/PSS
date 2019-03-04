#pragma once

#include "ClientCommand.h"
#include "IObject.h"

#include <string>

#include "ListManager.h"

//定义客户端信令(TCP)
#define COMMAND_LOGIC_LG_LOGIN      0x2000      //LG注册 
#define COMMAND_LOGIC_ALIVE         0x2001      //LG心跳
#define COMMAND_LOGIC_LG_LIST       0x2002      //LG获得当前服务器列表 
#define COMMAND_LOGIC_LG_LOGIN_R    0xe000      //LG注册回应
#define COMMAND_LOGIC_LG_KEY_R      0xe001      //LG下发列表Key
#define COMMAND_LOGIC_LG_LIST_R     0xe002      //LG下发列表的相关信息    

#define MESSAGE_FUNCTION_BEGIN(x) switch(x) {
#define MESSAGE_FUNCTION(x,y,z) case x: { y(z); break; }
#define MESSAGE_FUNCTION_END }

using namespace std;

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
    int Do_Logic_LG_Login(IMessage* pMessage);                                            //处理LG登录功能
    int Do_Logic_LG_Alive(IMessage* pMessage);                                            //LG心跳
    int Do_Logic_All_LG_Key(IMessage* pMessage, uint16 u2CommandID);                      //群发所有的LG告知服务器列表更新消息
    int Do_Logic_LG_List(IMessage* pMessage);                                             //接收刷新List请求
    int Send_Logic_LG_List(uint32 u4ConnectID, const char* pSession, uint16 u2Version);   //LG获得整个LG的服务器列表

private:
    CServerObject* m_pServerObject;
    int            m_nCount;
    ClistManager   m_listManager;
};

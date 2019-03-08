#pragma once

#include "ClientCommand.h"
#include "IObject.h"

#include <string>

#include "ListManager.h"

//����ͻ�������(TCP)
#define COMMAND_LOGIC_LG_LOGIN      0x2000      //LGע�� 
#define COMMAND_LOGIC_ALIVE         0x2001      //LG����
#define COMMAND_LOGIC_LG_LIST       0x2002      //LG��õ�ǰ�������б� 
#define COMMAND_LOGIC_LG_LOGIN_R    0xe000      //LGע���Ӧ
#define COMMAND_LOGIC_LG_KEY_R      0xe001      //LG�·��б�Key
#define COMMAND_LOGIC_LG_LIST_R     0xe002      //LG�·��б�������Ϣ    

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
    int Do_Logic_LG_Login(IMessage* pMessage);                                            //����LG��¼����
    int Do_Logic_LG_Alive(IMessage* pMessage);                                            //LG����
    int Do_Logic_All_LG_Key(IMessage* pMessage, uint16 u2CommandID);                      //Ⱥ�����е�LG��֪�������б������Ϣ
    int Do_Logic_LG_List(IMessage* pMessage);                                             //����ˢ��List����
    int Send_Logic_LG_List(uint32 u4ConnectID, const char* pSession, uint16 u2Version);   //LG�������LG�ķ������б�

private:
    CServerObject* m_pServerObject;
    int            m_nCount;
    ClistManager   m_listManager;
};

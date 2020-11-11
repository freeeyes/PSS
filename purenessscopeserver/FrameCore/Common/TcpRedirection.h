#ifndef _TCPREDIRECTION_H
#define _TCPREDIRECTION_H

//负责数据透传转发的数据接口。
//这里可以做成一个框架功能
//add by freeeyes

#include "define.h"
#include "XmlConfig.h"
#include "BuffPacketManager.h"
#include "IMessageBlockManager.h"
#include "ClientCommand.h"
#include "IClientManager.h"
#include "IConnectManager.h"
#include "IDeviceHandler.h"
#include <unordered_map>
#include <string>
#include <sstream>

static const char FORWARD_XML[] = "forward.xml";

enum class ENUM_FORWARD_TYPE
{
    ENUM_FORWARD_TCP_UNKNOW = 0,    //初始化状态
    ENUM_FORWARD_TCP_CLINET,        //客户端接入
    ENUM_FORWARD_TCP_S2S,           //服务器间连接
    ENUM_FORWARD_TCP_TTY            //TTY连接
};

class CForwardInfo
{
public:
    CForwardInfo() {};

    string m_strSource;
    string m_strTarget;
};

class CForwardConnectInfo
{
public:
    CForwardConnectInfo()
    {
    };

    string            m_strSource;
    string            m_strTarget;
    uint32            m_u4ConnectID    = 0;
    ENUM_FORWARD_TYPE m_emForwardType  = ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_UNKNOW;
    uint8             m_u1ConnectState = 0;  //0是关闭，1是打开
    IDeviceHandler*   m_pDeviceHandler = NULL;  //驱动指针
};

class CForwardManager
{
public:
    CForwardManager();

    void Close();

    int Init();

    string ConnectRegedit(const char* pIP, int nPort, ENUM_FORWARD_TYPE em_type, IDeviceHandler* pDeviceHandler);

    string ConnectRegedit(const char* pName, ENUM_FORWARD_TYPE em_type, IDeviceHandler* pDeviceHandler);

    void DisConnectRegedit(const char* pIP, int nPort, ENUM_FORWARD_TYPE em_type);

    void DisConnectRegedit(const char* pName, ENUM_FORWARD_TYPE em_type);

    void SendData(string strTarget, ACE_Message_Block* pmb);

    void AddForward(string strSource, string strTarget);

private:
    string Check_Connect_IP(const char* pName, ENUM_FORWARD_TYPE em_type, int ConnectState, IDeviceHandler* pDeviceHandler = NULL);

    IDeviceHandler* Get_Device_Handler(string strTarget);

    typedef unordered_map<string, CForwardConnectInfo*> mapForwardConnectList;
    vector<CForwardInfo> m_vecForwardInfo;          //需要关注的转发接口信息
    mapForwardConnectList m_mapForwardConnectList;
    int m_nActive = 0;
};

typedef ACE_Singleton<CForwardManager, ACE_Null_Mutex> App_ForwardManager;

#endif

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
#include "IHandler.h"
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
    CForwardInfo() = default;

    string m_strSource;
    string m_strTarget;
};

class CForwardConnectInfo
{
public:
    CForwardConnectInfo() = default;

    string            m_strSource;
    string            m_strTarget;
    uint32            m_u4ConnectID    = 0;
    ENUM_FORWARD_TYPE m_emForwardType  = ENUM_FORWARD_TYPE::ENUM_FORWARD_TCP_UNKNOW;
    uint8             m_u1ConnectState = 0;  //0是关闭，1是打开
    IHandler*         m_pHandler       =  nullptr;  //驱动指针
};

class CForwardManager
{
public:
    CForwardManager() = default;

    void Close();

    int Init();

    string ConnectRegedit(const char* pIP, int nPort, ENUM_FORWARD_TYPE em_type, IHandler* pHandler);

    string ConnectRegedit(const char* pName, ENUM_FORWARD_TYPE em_type, IHandler* pHandler);

    void DisConnectRegedit(const char* pIP, int nPort, ENUM_FORWARD_TYPE em_type);

    void DisConnectRegedit(const char* pName, ENUM_FORWARD_TYPE em_type);

    void SendData(const string& strTarget, ACE_Message_Block* pmb);

    void AddForward(const string& strSource, const string& strTarget);

private:
    string Check_Connect_IP(const char* pName, ENUM_FORWARD_TYPE em_type, int ConnectState, IHandler* pHandler = nullptr);

    IHandler* Get_Device_Handler(const string& strTarget);

    using mapForwardConnectList = unordered_map<string, shared_ptr<CForwardConnectInfo>>;
    vector<CForwardInfo> m_vecForwardInfo;          //需要关注的转发接口信息
    mapForwardConnectList m_mapForwardConnectList;
    int m_nActive = 0;
};

using App_ForwardManager = PSS_singleton<CForwardManager>;

#endif

#ifndef _MESSAGE_H
#define _MESSAGE_H

#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "MessageBlockManager.h"
#include "BuffPacket.h"
#include "IMessage.h"
#include "ObjectPoolManager.h"
#include "IHandler.h"

//接收消息信息
class CMessage : public IMessage
{
public:
    CMessage(void) = default;

    ACE_Message_Block* GetMessageHead();
    ACE_Message_Block* GetMessageBody();

    _MessageBase* GetMessageBase() final;

    bool GetPacketHead(_PacketInfo& PacketInfo) final;
    bool GetPacketBody(_PacketInfo& PacketInfo) final;
    bool SetPacketHead(ACE_Message_Block* pmbHead) final;
    bool SetPacketBody(ACE_Message_Block* pmbBody) final;

private:
    _MessageBase  m_MessageBase;

    ACE_Message_Block* m_pmbHead          = nullptr;    //包头部分
    ACE_Message_Block* m_pmbBody          = nullptr;   //包体部分
};

enum class EM_WORKTHREAD_DIRECT
{
    EM_WORKTHREAD_DIRECT_INPUT = 0,
    EM_WORKTHREAD_DIRECT_OUTPUT,
};

//工作线程的处理消息结构
class CWorkThreadMessage
{
public:
    int                m_nHashID         = 0;                                                //Hash对象ID
	uint32             m_u4WorkThreadID  = 0;                                                //线程ID
	uint16             m_u2Cmd           = 0;                                                //命令ID
    uint32             m_u4PacketParseID = 0;                                                //与ConnectID对应的解析器ID
    uint32             m_u4ConnectID     = 0;                                                //ConnectID
    EM_PACKET_RESOURCE m_emResouceType   = EM_PACKET_RESOURCE::PACKET_RESOURCE_FROM_CLIENT;  //数据来源，服务器间数据包或者客户端数据包
    EM_CONNECT_IO_TYPE m_emPacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;               //消息类型，TCP还是UDP,还是TTY
    EM_WORKTHREAD_DIRECT m_emDirect      = EM_WORKTHREAD_DIRECT::EM_WORKTHREAD_DIRECT_INPUT;  //接收还是发送
    ACE_Message_Block* m_pmbRecvHead     = nullptr;                                          //接收包头部分
	ACE_Message_Block* m_pmbRecvBody     = nullptr;                                          //接收包体部分

    IHandler*          m_pHandler        = nullptr;                                          //Handler指针
    PSS_Time_Point     m_tvMessage;                                                          //消息处理时间
	ACE_INET_Addr      m_AddrRemote;                                                         //数据包的来源IP信息
	ACE_INET_Addr      m_AddrListen;                                                         //数据包来源监听IP信息
     
    CSendMessageInfo   m_SendMessageInfo;                                                    //发送数据内容           

    CWorkThreadMessage() = default;

    void SetHashID(int nHashID);

    int GetHashID() const;

    void Clear();

    void Close();
};

//一个工作线程消息数据集合
class CWorkThreadMessageList
{
public:
    CWorkThreadMessageList() = default;

    uint32 m_u4WorkThreadID = 0;
    vector<shared_ptr<CWorkThreadMessage>> m_vecList;
};

//线程内维护的Handler对象架构
class CWorkThread_Handler_info
{
public:
    IHandler*          m_pHandler = nullptr;                                             //Handler指针
    PSS_Time_Point     m_tvInput;                                                        //消息处理时间
    PSS_Time_Point     m_tvOutput;                                                       //消息处理时间
    std::string        m_strLocalIP;                                                     //监听IP
    uint16             m_u2LocalPort;                                                    //监听端口
	std::string        m_strRemoteIP;                                                    //客户端IP
    uint16             m_u2RemotePort;                                                   //客户端端口
	uint32             m_u4LocalPort;                                                    //监听端口
	uint32             m_InPacketCount  = 0;                                             //接收包个数
	uint32             m_OutPacketCount = 0;                                             //发送包个数
	uint32             m_RecvSize       = 0;                                             //接收字节数
	uint32             m_SendSize       = 0;                                             //发送字节数
    uint32             m_u4ConnectID    = 0;                                             //链接对象ID 
    EM_CONNECT_IO_TYPE m_emPacketType   = EM_CONNECT_IO_TYPE::CONNECT_IO_SERVER_TCP;     //链接类型
    int                m_nHashID        = 0;                                             //Hash对象ID

    void SetHashID(int nHashID)
    {
        m_nHashID = nHashID;
    };

    int GetHashID() const
    {
        return m_nHashID;
    };

	void Clear()
	{
		m_pHandler = nullptr;
		m_InPacketCount = 0;
		m_OutPacketCount = 0;
		m_RecvSize = 0;
		m_SendSize = 0;
	}
};

//CWorkThreadMessage对象池
class CDeviceHandlerPool : public CObjectPoolManager<CWorkThread_Handler_info, ACE_Recursive_Thread_Mutex>
{
public:
    CDeviceHandlerPool() = default;

	static void Init_Callback(int nIndex, CWorkThread_Handler_info* pHandler);
	static void Close_Callback(int nIndex, CWorkThread_Handler_info* pHandler);

    CWorkThread_Handler_info* Create();
	bool Delete(CWorkThread_Handler_info* pHandler);

	int GetUsedCount();
	int GetFreeCount();

	void GetCreateInfoList(vector<_Object_Create_Info>& objCreateList);
};

#endif

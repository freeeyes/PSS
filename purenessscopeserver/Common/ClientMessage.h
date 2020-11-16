#ifndef _ICLIENTMESSAGE_H
#define _ICLIENTMESSAGE_H

//这是一个接口类，负责将数据发送出去，并获得数据返回，提供链接错误的类
//方法包括RecvMessage()接收数据和ConnectError()发送错误三种方法。
//add by freeeyes 2010-12-31

#include "IMessageBlockManager.h"

//收到的数据包数据回路
enum class EM_PACKET_ROUTE
{
    PACKET_ROUTE_SELF = 0,   //数据包回路，需要m_pClientMessage自己去处理
    PACKET_ROUTE_CLIENT_MESSAHE,   //数据包回路，回应到CMessageService去处理
};

class IClientMessage
{
public:
    virtual ~IClientMessage() {}

    //是否需要框架调用发送组包函数
    virtual bool Need_Send_Format()                                                                                                                              = 0;
    //发送组包函数
    virtual bool Send_Format_data(char* pData, uint32 u4Len, IMessageBlockManager* pMessageBlockManager, ACE_Message_Block*& mbSend)                             = 0;    //处理数据包发送接口
    //接收组包函数
    virtual bool Recv_Format_data(ACE_Message_Block* mbRecv, IMessageBlockManager* pMessageBlockManager, uint16& u2CommandID,
                                  ACE_Message_Block*& mbFinishRecv, EM_PACKET_ROUTE& emPacketRoute)  = 0;    //处理数据包接收接口

    //链接重连成功回调函数
    virtual void ReConnect(int nServerID)                                                               = 0;    //连接重连成功接口
    //处理接收数据
    virtual bool RecvData(uint16 u2CommandID, ACE_Message_Block* mbRecv, const _ClientIPInfo& objServerIPInfo) = 0;    //接收数据的函数
    //链接异常回调函数
    virtual bool ConnectError(int nError, _ClientIPInfo const& objServerIPInfo)                                = 0;    //当出错的时候，调用此接口返回错误信息
};

#endif

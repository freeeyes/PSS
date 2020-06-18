#ifndef MAKEPACKET_H
#define MAKEPACKET_H

//专门处理数据解包操作
//把数据包的拼接和解包放在一起
//add by freeeyes
//2010-06-11
#pragma once

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "PacketParsePool.h"
#include "MessageService.h"
#include "ProfileTime.h"
#include "HashTable.h"

class _MakePacket
{
public:
    uint32              m_u4ConnectID     = 0;                                   //链接ID
    uint32              m_u4PacketParseID = 0;                                   //对应ConnectID的PacketParseID
    int32               m_nHashID         = 0;                                   //对应记录hash的ID
    CPacketParse*       m_pPacketParse    = NULL;                                //数据包数据指针
    EM_CONNECT_IO_TYPE  m_u1PacketType    = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;  //数据包类型
    uint8               m_u1Option        = 0;                                   //操作类型
    ACE_INET_Addr       m_AddrRemote;                                            //数据包的来源IP信息
    ACE_INET_Addr       m_AddrListen;                                            //数据包来源监听IP信息

    _MakePacket()
    {
    }

    void Clear()
    {
        m_u1PacketType      = EM_CONNECT_IO_TYPE::CONNECT_IO_TCP;  //0为TCP,1是UDP 默认是TCP
        m_u4ConnectID       = 0;
        m_u1Option          = 0;
        m_pPacketParse      = NULL;
        m_u4PacketParseID   = 0;
    }

    void SetHashID(int32 nHashID)
    {
        m_nHashID = nHashID;
    }

    int32 GetHashID() const
    {
        return m_nHashID;
    }
};

class CMakePacket
{
public:
    CMakePacket(void);

    bool Init() const;

    bool PutMessageBlock(const _MakePacket* pMakePacket, const ACE_Time_Value& tvNow) const;                                                     //处理消息数据包
    bool PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, const ACE_Time_Value& tvNow) const;                            //发送失败消息回调

private:
    void SetMessage(const _MakePacket* pMakePacket, CMessage* pMessage, const ACE_Time_Value& tvNow) const;                                //一般数据包消息
    void SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CMessage* pMessage, const ACE_Time_Value& tvNow) const;  //服务发送失败回调数据包消息

    ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;
};
typedef ACE_Singleton<CMakePacket, ACE_Null_Mutex> App_MakePacket;

#endif

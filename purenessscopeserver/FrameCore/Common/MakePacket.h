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
#include "MakePacket_Common.h"
#include "PerformanceCounter.h"

class CMakePacket
{
public:
    CMakePacket(void);

    bool Init() const;

    bool PutMessageBlock(_MakePacket const& objMakePacket, const ACE_Time_Value& tvNow) const;                                                      //处理消息数据包
    bool PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, const ACE_Time_Value& tvNow) const;                            //发送失败消息回调

private:
    void SetMessage(_MakePacket const& objMakePacket, CWorkThreadMessage* pWorkThreadMessage, const ACE_Time_Value& tvNow) const;                              //一般数据包消息
    void SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CWorkThreadMessage* pWorkThreadMessage, const ACE_Time_Value& tvNow) const;  //服务发送失败回调数据包消息

    ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;
    CPerformanceCounter            m_PerformanceCounter;
};
typedef ACE_Singleton<CMakePacket, ACE_Null_Mutex> App_MakePacket;

#endif

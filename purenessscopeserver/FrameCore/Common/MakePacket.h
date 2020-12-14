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

#include "PacketParse.h"
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

    bool PutMessageBlock(_MakePacket const& objMakePacket, const PSS_Time_Point& tvNow);                         //处理消息数据包
    bool PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, const PSS_Time_Point& tvNow);  //发送失败消息回调
    void CommitMessageList();          //提交到工作线程队列

private:
    void SetMessage(_MakePacket const& objMakePacket, shared_ptr<CWorkThreadMessage> pWorkThreadMessage, const PSS_Time_Point& tvNow) const;                              //一般数据包消息
    void SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, shared_ptr<CWorkThreadMessage> pWorkThreadMessage, const PSS_Time_Point& tvNow) const;  //服务发送失败回调数据包消息

    ACE_Recursive_Thread_Mutex         m_ThreadWriteLock;
    CPerformanceCounter                m_PerformanceCounter;
    shared_ptr<CWorkThreadMessageList> m_pMessageList;
};

#endif

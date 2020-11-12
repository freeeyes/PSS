#ifndef MAKEPACKET_H
#define MAKEPACKET_H

//ר�Ŵ������ݽ������
//�����ݰ���ƴ�Ӻͽ������һ��
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

    bool PutMessageBlock(_MakePacket const& objMakePacket, const ACE_Time_Value& tvNow) const;                                                      //������Ϣ���ݰ�
    bool PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, const ACE_Time_Value& tvNow) const;                            //����ʧ����Ϣ�ص�

private:
    void SetMessage(_MakePacket const& objMakePacket, CWorkThreadMessage* pWorkThreadMessage, const ACE_Time_Value& tvNow) const;                              //һ�����ݰ���Ϣ
    void SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, CWorkThreadMessage* pWorkThreadMessage, const ACE_Time_Value& tvNow) const;  //������ʧ�ܻص����ݰ���Ϣ

    ACE_Recursive_Thread_Mutex     m_ThreadWriteLock;
    CPerformanceCounter            m_PerformanceCounter;
};
typedef ACE_Singleton<CMakePacket, ACE_Null_Mutex> App_MakePacket;

#endif

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

    bool PutMessageBlock(_MakePacket const& objMakePacket, const PSS_Time_Point& tvNow);                         //������Ϣ���ݰ�
    bool PutSendErrorMessage(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, const PSS_Time_Point& tvNow);  //����ʧ����Ϣ�ص�
    void CommitMessageList();          //�ύ�������̶߳���

private:
    void SetMessage(_MakePacket const& objMakePacket, shared_ptr<CWorkThreadMessage> pWorkThreadMessage, const PSS_Time_Point& tvNow) const;                              //һ�����ݰ���Ϣ
    void SetMessageSendError(uint32 u4ConnectID, ACE_Message_Block* pBodyMessage, shared_ptr<CWorkThreadMessage> pWorkThreadMessage, const PSS_Time_Point& tvNow) const;  //������ʧ�ܻص����ݰ���Ϣ

    ACE_Recursive_Thread_Mutex         m_ThreadWriteLock;
    CPerformanceCounter                m_PerformanceCounter;
    shared_ptr<CWorkThreadMessageList> m_pMessageList;
};

#endif

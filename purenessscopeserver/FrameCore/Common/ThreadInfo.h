#ifndef _THREADINFO_H
#define _THREADINFO_H

#include "define.h"
#include "TimeStamp.hpp"

class _ThreadInfo
{
public:
    uint32              m_u4ThreadIndex     = 0;                            //ϵͳ�߳�ID
    THREADSTATE         m_u4State           = THREADSTATE::THREAD_INIT;     //0Ϊû���������ڴ���1Ϊ���ڴ���������ݡ�
    uint32              m_u4RecvPacketCount = 0;                            //���߳��½��հ��Ĵ������
    uint32              m_u4CurrPacketCount = 0;                            //��λʱ���ڵİ���������
    uint16              m_u2CommandID       = 0;                            //��ǰ���������ID
    uint16              m_u2PacketTime      = 0;                            //ƽ�����ݰ�����ʱ��
    ACE_thread_t        m_u4ThreadID        = 0;                            //�߳�ID
    PSS_Time_Point      m_tvUpdateTime      = CTimeStamp::Get_Time_Stamp(); //�߳���������ݵ�ʱ��
    PSS_Time_Point      m_tvCreateTime      = CTimeStamp::Get_Time_Stamp(); //�̴߳���ʱ��

    _ThreadInfo() = default;
};

#endif

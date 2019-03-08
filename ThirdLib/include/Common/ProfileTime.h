#ifndef _PROFILETIME_H
#define _PROFILETIME_H

//ͳ�ƹ���ִ��ʱ�������ָ�����
//��ǰ������ָ��ͳ��̫����ɢ����������һ����װ
//��windows�£�ACE������ָ����ƫ�������������һ���꣬�ֿ��Դ���
//add by freeeyes

#include "define.h"
#include "ace/Profile_Timer.h"

class CProfileTime
{
public:
    CProfileTime(void);
    ~CProfileTime(void);

    bool       Start();               //��ʼ��ʱ
    uint64     Stop();                //������ʱ�������ش���ʱ�䣬��λ������

private:
#ifndef __LINUX__
    LARGE_INTEGER m_liPerfFreq;    //CPU�ĵδ�ʱ�䣬������windowsʹ��
    LARGE_INTEGER m_liPerfStart;   //��ʼʱ��
    LARGE_INTEGER m_liPerfStop;    //����ʱ��
#else
    ACE_High_Res_Timer m_HighResTimer;
#endif

};
#endif


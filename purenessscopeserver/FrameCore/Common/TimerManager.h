// ConnectHandle.h
// ����������Ӧ���Ķ�ʱ���кܴ�������ԣ�����Reactor��Proactor��ʹ�ö�ʱ����ʱ���ǳ��鷳��
// ��Ȼ��ϵͳ֧������ģʽ�����Ըĵĺ�һЩ������ʱ������������������������з��������õ��Ķ�ʱ��������
// add by freeeyes
// 2009-08-25

#ifndef _TIMEMANAGER_H
#define _TIMEMANAGER_H

#include "define.h"
#include "ITimerManager.h"

using App_TimerManager = ACE_Singleton<ActiveTimer, ACE_Null_Mutex>;
#endif

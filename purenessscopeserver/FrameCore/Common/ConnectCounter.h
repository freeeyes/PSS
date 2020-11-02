#ifndef _PSS_CCONNECT_COUNTER_H
#define _PSS_CCONNECT_COUNTER_H

#include "define.h"

//ȫ�ּ�����
//�������в�ͬ���͵�Connect��id���ɣ���֤Ψһ��
//add by freeeyes

class CConnectCounter
{
public:
	uint32 CreateCounter();  //�õ�Ψһ����ID
	
private:
	uint32 m_u4CountIndex       = 0;
	ACE_Recursive_Thread_Mutex  m_ThreadWritrLock;
};

typedef ACE_Singleton<CConnectCounter, ACE_Recursive_Thread_Mutex> App_ConnectCounter;

#endif

#ifndef _PSS_CCONNECT_COUNTER_H
#define _PSS_CCONNECT_COUNTER_H

#include "define.h"

//全局计数器
//用于所有不同类型的Connect的id生成，保证唯一。
//add by freeeyes

class CConnectCounter
{
public:
	uint32 CreateCounter();  //得到唯一的新ID
	
private:
	uint32 m_u4CountIndex       = 0;
	ACE_Recursive_Thread_Mutex  m_ThreadWritrLock;
};

using App_ConnectCounter = PSS_singleton<CConnectCounter>;

#endif

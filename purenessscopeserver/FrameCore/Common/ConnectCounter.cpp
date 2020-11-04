#include "ConnectCounter.h"

uint32 CConnectCounter::CreateCounter()
{
	ACE_Guard<ACE_Recursive_Thread_Mutex> guard(m_ThreadWritrLock);
	return ++m_u4CountIndex;
}


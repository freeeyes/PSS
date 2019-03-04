#ifndef _BASETASK_H
#define _BASETASK_H

#include "define.h"
#include "ace/Task.h"
#include "ace/OS_Memory.h"

int Task_Common_CloseMsgQueue(ACE_Task<ACE_MT_SYNCH>* pTask, ACE_Condition<ACE_Thread_Mutex>& cond, ACE_Thread_Mutex& mutex);


#endif

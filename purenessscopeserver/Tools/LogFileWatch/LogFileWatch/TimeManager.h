#ifndef _TIMEMANAGER_H
#define _TIMEMANAGER_H

#include "ace/Log_Msg.h"
#include "ace/Event_Handler.h"
#include "ace/Singleton.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Timer_Heap_T.h"
#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"
#include "ace/Timer_Hash.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Queue_Adapters.h"

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> ActiveTimer;

typedef ACE_Singleton<ActiveTimer, ACE_Null_Mutex> App_TimerManager;

#endif
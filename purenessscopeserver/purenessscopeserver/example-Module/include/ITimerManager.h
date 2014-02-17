#ifndef _ITIMERMANAGER_H
#define _ITIMERMANAGER_H

//添加Timer的管理项，提供给插件内部使用。
//add by freeeyes

#include "ace/Timer_Queue.h"
#include "ace/Timer_Heap.h"
#include "ace/Timer_Wheel.h"
#include "ace/Timer_Hash.h"
#include "ace/Timer_List.h"
#include "ace/Timer_Queue_Adapters.h"

typedef ACE_Thread_Timer_Queue_Adapter<ACE_Timer_Heap> ActiveTimer;


#endif

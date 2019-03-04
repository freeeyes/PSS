// ConnectHandle.h
// 觉得依赖反应器的定时器有很大的限制性，比如Reactor和Proactor下使用定时器的时候会非常麻烦。
// 既然让系统支持两种模式，索性改的好一些，将定时器管理器抽象出来。负责所有服务器中用到的定时器操作。
// add by freeeyes
// 2009-08-25

#ifndef _TIMEMANAGER_H
#define _TIMEMANAGER_H

#include "define.h"
#include "ITimerManager.h"

typedef ACE_Singleton<ActiveTimer, ACE_Null_Mutex> App_TimerManager;
#endif

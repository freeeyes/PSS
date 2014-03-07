#ifndef _MESSAGESERVICE_H
#define _MESSAGESERVICE_H

#include "define.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Malloc_T.h"
#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ace/Date_Time.h"

#include "Message.h"
#include "MessageManager.h"
#include "LogManager.h"
#include "ThreadInfo.h"
#include "BuffPacket.h"
#include "MainConfig.h"
#include "TimerManager.h"
#include "RandomNumber.h"
#include "WorkThreadAI.h"

#ifdef WIN32
#include "ProConnectHandle.h"
#include "WindowsCPU.h"
#else
#include "ConnectHandler.h"
#include "LinuxCPU.h"
#endif

class CMessageService : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CMessageService(void);
	~CMessageService(void);

	virtual int open(void* args = 0);
	virtual int svc (void);
	int Close ();

	bool SaveThreadInfo();

	void Init(uint32 u4ThreadID, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK);

	bool Start();

	bool PutMessage(CMessage* pMessage);

	_ThreadInfo* GetThreadInfo();

private:
	bool IsRun();
	bool ProcessMessage(CMessage* pMessage, uint32 u4ThreadID);
	bool SaveThreadInfoData(); 

private:
	uint32                         m_u4ThreadID;          //当前线程ID
	uint32                         m_u4MaxQueue;          //线程中最大消息对象个数
	bool                           m_blRun;               //线程是否在运行
	uint32                         m_u4HighMask;
	uint32                         m_u4LowMask;
	uint16                         m_u2ThreadTimeOut;  
	uint16                         m_u2ThreadTimeCheck;
	uint64                         m_u8TimeCost;          //Put到队列信息的数据处理时间
	uint32                         m_u4Count;             //消息队列接受个数
	uint32                         m_u4WorkQueuePutTime;  //入队超时时间

	_ThreadInfo                    m_ThreadInfo;          //当前线程信息
	CWorkThreadAI                  m_WorkThreadAI;        //线程自我监控的AI逻辑  
};

//add by freeeyes
//添加线程管理，用户可以创建若干个ACE_Task，每个Task对应一个线程，一个Connectid只对应一个线程。
class CMessageServiceGroup : public ACE_Task<ACE_MT_SYNCH>
{
public:
	CMessageServiceGroup(void);
	~CMessageServiceGroup(void);

	virtual int handle_timeout(const ACE_Time_Value &tv, const void *arg);

	bool Init(uint32 u4ThreadCount = MAX_MSG_THREADCOUNT, uint32 u4MaxQueue = MAX_MSG_THREADQUEUE, uint32 u4LowMask = MAX_MSG_MASK, uint32 u4HighMask = MAX_MSG_MASK);
	bool PutMessage(CMessage* pMessage);
	void Close();

	bool Start();
	CThreadInfo* GetThreadInfo();

	uint32 GetWorkThreadCount();                    //得到当前工作线程的数量
	uint32 GetWorkThreadIDByIndex(uint32 u4Index);  //得到指定工作线程的线程ID    

private:
	bool StartTimer();
	bool KillTimer();

private:
	typedef vector<CMessageService*> vecMessageService;
	vecMessageService m_vecMessageService;

public:
	CThreadInfo                m_objAllThreadInfo;    //当前所有线程信息
	uint32                     m_u4TimerID;           //定时器ID
	uint16                     m_u2ThreadTimeCheck;   //线程自检时间
	uint32                     m_u4MaxQueue;          //线程中最大消息对象个数
	uint32                     m_u4HighMask;          //线程高水位
	uint32                     m_u4LowMask;           //线程低水位
	CRandomNumber              m_objRandomNumber;     //随机数，仅UDP使用
	//ACE_Recursive_Thread_Mutex m_ThreadWriteLock;     //当前线程锁
};


typedef ACE_Singleton<CMessageServiceGroup,ACE_Null_Mutex> App_MessageServiceGroup; 
#endif

#pragma once

#include "ITMService.h"

class CTMService : public ITMService
{
public:
    CTMService();

    int Init();

    void Close();

    int AddMessage(string strName, int nMessagePos, long sec, long usec, int _Message_id, void* _arg, Enum_Timer_Mode emTimerMode = Timer_Mode_Run_Once, IMessagePrecess* pMessagePrecess = NULL);
    void* DeleteMessage(string strName, int nMessagePos);
    bool GetRun();

private:
    CTimerManager              timer_events_;
    int                        m_nTimerMaxCount;
    int                        m_nActive;
    CHashTable<CTimerInfo>     m_HashTimerList;
    CThreadQueueManager        m_ThreadQueueManager;
    unordered_map<int, int>    m_T2MList;
    unordered_map<int, int>    m_M2TList;

};
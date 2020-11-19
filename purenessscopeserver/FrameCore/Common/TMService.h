#pragma once

#include "ITMService.h"
#include "XmlOpeation.h"
#include <unordered_map>

class CTMService : public ITMService
{
public:
    CTMService() = default;
    ~CTMService() final = default;

    int Init(int nNeedLoad = 0);

    void Close();

    int AddMessage(const char* pName, unsigned long long nMessagePos, long sec, long usec, int _Message_id, void* _arg, Enum_Timer_Mode emTimerMode = Timer_Mode_Run_Once, IMessagePrecess* pMessagePrecess = nullptr) final;
    void* DeleteMessage(const char* pName, unsigned long long nMessagePos) final;
    bool GetRun();

private:
    using hashmapPortAccount = unordered_map<string, shared_ptr<CTimerInfo>>;
    CTimerManager                      timer_events_;
    int                                m_nTimerMaxCount = 0;
    int                                m_nActive        = 0;
    hashmapPortAccount                 m_HashTimerList;
    CThreadQueueManager                m_ThreadQueueManager;
    unordered_map<int, int>            m_T2MList;
    unordered_map<int, int>            m_M2TList;

};
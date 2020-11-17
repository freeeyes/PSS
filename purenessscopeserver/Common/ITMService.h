#pragma once

#include <string>
#include "TSCommon.hpp"

class ITMService
{
public:
    virtual ~ITMService() {};

    virtual int AddMessage(const char* pName, unsigned long long nMessagePos, long sec, long usec, int _Message_id, void* _arg, Enum_Timer_Mode emTimerMode = Timer_Mode_Run_Once, IMessagePrecess* pMessagePrecess = nullptr) = 0;

    virtual void* DeleteMessage(const char* pName, unsigned long long nMessagePos) = 0;

    virtual bool GetRun() = 0;
};


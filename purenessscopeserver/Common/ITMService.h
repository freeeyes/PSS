#pragma once

#include <string>
#include "TSCommon.hpp"

class ITMService
{
public:
    virtual int AddMessage(std::string strName, int nMessagePos, long sec, long usec, int _Message_id, void* _arg, Enum_Timer_Mode emTimerMode = Timer_Mode_Run_Once, IMessagePrecess* pMessagePrecess = NULL) = 0;

    virtual void* DeleteMessage(std::string strName, int nMessagePos) = 0;

    virtual bool GetRun() = 0;
};


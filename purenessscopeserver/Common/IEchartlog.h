#ifndef _IECHERTLOG_H
#define _IECHERTLOG_H

#include "define.h"

class IEchartlog
{
public:
    virtual ~IEchartlog() {};

    virtual bool Writelog(const char* pPath, const char* pFileName, const char* pTitle, const char* pText) = 0;
};

#endif

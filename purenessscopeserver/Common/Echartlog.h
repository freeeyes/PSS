#ifndef ECHARTLOG_H
#define ECHARTLOG_H

#include "IEchartlog.h"

class CEchartlog : public IEchartlog
{
public:
    CEchartlog();
    virtual ~CEchartlog();

    virtual bool Writelog(const char* pPath, const char* pFileName, const char* pTitle, const char* pText);

};

typedef ACE_Singleton<CEchartlog, ACE_Null_Mutex> App_Echartlog;
#endif

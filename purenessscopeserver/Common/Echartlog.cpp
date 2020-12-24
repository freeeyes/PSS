#include "Echartlog.h"

CEchartlog::CEchartlog()
{

}

CEchartlog::~CEchartlog()
{

}

bool CEchartlog::Writelog(const char* pPath, const char* pFileName, const char* pTitle, const char* pText)
{
    //open file
    char szFileName[MAX_BUFF_200] = { '\0' };
    char szLogText[MAX_BUFF_500]  = { '\0' };

    if(pPath[ACE_OS::strlen(pPath) - 1] == '\\' || pPath[ACE_OS::strlen(pPath) - 1] == '/')
    {
        sprintf_safe(szFileName, MAX_BUFF_200, "%s%s", pPath, pFileName);
    }
    else
    {
        sprintf_safe(szFileName, MAX_BUFF_200, "%s\\%s", pPath, pFileName);
    }

    FILE* pFile = ACE_OS::fopen(szFileName, "a+");

    if (nullptr == pFile)
    {
        PSS_LOGGER_DEBUG("[CEchartlog::Writelog]open file error({0}).", szFileName);
        return false;
    }

    time_t tNow = time(nullptr);
    struct tm* tmNow = ACE_OS::localtime(&tNow);

    sprintf_safe(szLogText, MAX_BUFF_500, "%s|%04d-%02d-%02d %02d:%02d:%2d|%s",
                 pTitle,
                 tmNow->tm_year + 1900,
                 tmNow->tm_mon + 1,
                 tmNow->tm_mday,
                 tmNow->tm_hour,
                 tmNow->tm_min,
                 tmNow->tm_sec,
                 pText);


    //write file
    ACE_OS::fwrite(szLogText, sizeof(char), ACE_OS::strlen(szLogText), pFile);

    ACE_OS::fclose(pFile);
    return true;
}

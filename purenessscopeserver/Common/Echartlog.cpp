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
        OUR_DEBUG((LM_ERROR, "[CEchartlog::Writelog]1.\n", szFileName));
        sprintf_safe(szFileName, MAX_BUFF_200, "%s%s", pPath, pFileName);
    }
    else
    {
        OUR_DEBUG((LM_ERROR, "[CEchartlog::Writelog]2.\n", szFileName));
        sprintf_safe(szFileName, MAX_BUFF_200, "%s\\%s", pPath, pFileName);
    }

    FILE* pFile = ACE_OS::fopen(szFileName, "a+");

    if (NULL == pFile)
    {
        OUR_DEBUG((LM_ERROR, "[CEchartlog::Writelog]open file error(%s).\n", szFileName));
        return false;
    }

    time_t tNow = time(NULL);
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

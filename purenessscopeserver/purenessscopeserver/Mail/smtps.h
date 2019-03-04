#ifndef _SMTPS_H
#define _SMTPS_H

#ifdef __THREE_LIBRARY_CURL__
#include <string.h>
#include <curl/curl.h>

#define MULTI_PERFORM_HANG_TIMEOUT 60 * 1000

struct WriteThis
{
    int counter;
    char** pText;

    WriteThis()
    {
        counter = 0;
        pText   = 0;
    }
};

#endif

//使用ssl发送smtp邮件
int Send_Mail_From_Ssl(const char* pUser, const char* pPass, const char* pFrom, const char* pTo, const char* pUrl, const char* pTitle, const char* pData);

#endif



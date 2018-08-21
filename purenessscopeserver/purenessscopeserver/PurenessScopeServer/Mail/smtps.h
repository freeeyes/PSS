#ifndef _SMTPS_H
#define _SMTPS_H

#ifndef WIN32
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

static size_t read_callback(void* ptr, size_t size, size_t nmemb, void* userp)
{
    struct WriteThis* pooh = (struct WriteThis*)userp;
    const char* data;

    if (size*nmemb < 1)
    {
        return 0;
    }

    data = pooh->pText[pooh->counter];

    if (data)
    {
        size_t len = strlen(data);
        memcpy(ptr, data, len);
        pooh->counter++; /* advance pointer */
        return len;
    }

    return 0;
}

static struct timeval tvnow(void)
{
    struct timeval now;
    now.tv_sec = (long)time(NULL);
    now.tv_usec = 0;
    return now;
}

static long tvdiff(struct timeval newer, struct timeval older)
{
    return (newer.tv_sec - older.tv_sec) * 1000 +
           (newer.tv_usec - older.tv_usec) / 1000;
}

#endif

//使用ssl发送smtp邮件
int Send_Mail_From_Ssl(const char* pUser, const char* pPass, const char* pFrom, const char* pTo, const char* pUrl, const char* pTitle, const char* pData);

#endif


